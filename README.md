# StaticPlugFactory

A C++20 header-only framework for self-registering plug-in factories.

Each concrete product type registers itself automatically at program startup via a static instance — no central list to maintain, no manual wiring between factories and their products.

## The Problem

Traditional factory patterns require a central dispatch table that must be updated every time a new product type is added:

```cpp
// Every new type requires editing this function
std::shared_ptr<Foo> CreateFoo(const std::string& name) {
    if (name == "Foo_A") return std::make_shared<Foo_A>();
    if (name == "Foo_B") return std::make_shared<Foo_B>();
    // ...
}
```

This creates coupling between product types and the factory, and is easy to forget.

## The Solution

With StaticPlugFactory, each concrete type brings its own registration. Adding a new product is entirely self-contained in its own `.cpp` file — no header required, and the factory doesn't need to change.

Because the concrete class has no header file, callers can never `#include` it directly. The only way to interact with a product is through the abstract base interface, making downcasting impossible by construction.

```cpp
// Foo_A.cpp — the entire file, nothing else needed

#include "FooFactory.h"
#include "Foo.h"

#include <memory>
#include <string>

// some example class
class Foo_A : public Foo
{
public:
    explicit Foo_A(const std::string& params) { /* ... */ }

    // some method overrides
    std::string Test() override { return "This is Foo_A!"; }
};

//-----------------------------------------------------------------------------
// Factory registration — this is all it takes to plug Foo_A into the system.
//-----------------------------------------------------------------------------
class Foo_A_Maker : public FooFactory<Foo, const std::string&>
{
    Foo_A_Maker() : FooFactory("Foo_A") {}      // registers on construction
    static const Foo_A_Maker registerThis;      // triggers registration at startup

public:
    [[nodiscard]] std::shared_ptr<Foo> MakeProduct(const std::string& params) const override
    {
        return std::make_shared<Foo_A>(params);
    }
};

const Foo_A_Maker Foo_A_Maker::registerThis;   // the one static instance
```

## How It Works

Three templates compose the system:

| Template | Role |
|---|---|
| `PlugFactory<Factory, Key>` | Owns the static registry; maps keys to factory pointers |
| `ProductFactory<Base, BaseFactory, Key, Args...>` | Adds `NewObject(name, args...)` on top of the registry |
| Your factory class (e.g. `FooFactory`) | Defines the product family and the `MakeProduct` interface |

The registry lives in a function-local static inside `PlugFactory`, which guarantees thread-safe initialization. The first registration for a given key wins; duplicate registrations are silently ignored.

## Usage

### 1. Define your product base class

```cpp
// Foo.h
class Foo {
public:
    virtual std::string Test() = 0;
    virtual ~Foo() = default;
};
```

### 2. Define the product family factory

```cpp
// FooFactory.h
#include "MakerTemplate.h"

template<typename Foo, typename... Args>
class FooFactory : public ProductFactory<Foo, FooFactory<Foo, Args...>, std::string, Args...>
{
protected:
    using base_type = ProductFactory<Foo, FooFactory<Foo, Args...>, std::string, Args...>;
    friend base_type;

    explicit FooFactory(const std::string& className) {
        this->RegisterClass(className, this);
    }

    [[nodiscard]] virtual std::shared_ptr<Foo> MakeProduct(Args... args) const {
        return nullptr;
    }

public:
    FooFactory() = default;
    virtual ~FooFactory() = default;
};
```

### 3. Implement a concrete product and its maker

```cpp
// Foo_A.cpp
class Foo_A : public Foo {
public:
    explicit Foo_A(const std::string&) {}
    std::string Test() override { return "This is Foo_A!"; }
};

class Foo_A_Maker : public FooFactory<Foo, const std::string&>
{
    Foo_A_Maker() : FooFactory("Foo_A") {}
    static const Foo_A_Maker registerThis;

public:
    [[nodiscard]] std::shared_ptr<Foo> MakeProduct(const std::string& params) const override {
        return std::make_shared<Foo_A>(params);
    }
};

const Foo_A_Maker Foo_A_Maker::registerThis;
```

### 4. Create products by name

The caller only needs the abstract interface and the factory — there is no `Foo_A.h` to include because `Foo_A` has no header. The concrete type is completely hidden; the caller cannot name it, hold a typed pointer to it, or downcast to it.

```cpp
// Main.cpp — note: no #include "Foo_A.h" — it doesn't exist

#include "FooFactory.h"
#include "Foo.h"

#include <iostream>
#include <memory>
#include <string>

int main()
{
    FooFactory<Foo, const std::string&> ff;

    // Request a product by name — no knowledge of Foo_A required
    std::shared_ptr<Foo> foo = ff.NewObject("Foo_A", std::string{"Blah"});

    if (!foo) {
        std::cerr << "Unknown product type.\n";
        return 1;
    }

    std::cout << foo->Test();   // "This is Foo_A!"
    return 0;
}
```

## API Reference

### `PlugFactory<Factory, Key>`

| Member | Description |
|---|---|
| `GetFactory(key)` | Returns a `const Factory*` for the given key, or `nullptr` if not found |
| `RegisterClass(key, factory)` | Registers a factory pointer; first registration for a key wins |

`Key` must satisfy the `HashableKey` concept: hashable via `std::hash` and equality-comparable.

### `ProductFactory<BaseType, BaseFactoryType, KeyType, Args...>`

| Member | Description |
|---|---|
| `NewObject(name, args...)` | Looks up the factory by name and calls `MakeProduct(args...)`; returns `nullptr` if the name is not registered |
| `RegisterClass(name, factory)` | Protected; called by derived factory constructors to self-register |

## Requirements

- C++20 (uses concepts)
- MSVC 19.29+, GCC 10+, or Clang 12+
- Header-only — copy `PlugFactoryTemplate.h` and `MakerTemplate.h` into your project

## License

MIT License — Copyright (c) 2026 James Edwards
