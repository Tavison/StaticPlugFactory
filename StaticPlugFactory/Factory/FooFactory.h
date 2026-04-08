#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "MakerTemplate.h"

// This is a sample class that makes the base for a whole family of Foos.
// This class can also define other creation methods.
template<typename Foo, typename Params>
class FooFactory : public ProductFactory<Foo, FooFactory<Foo, Params>, Params>
{
protected:
    using base_type = ProductFactory<Foo, FooFactory<Foo, Params>, Params>;
    using pointer = std::shared_ptr<Foo>;

    friend base_type;

    explicit FooFactory(std::string_view className)
    {
        this->RegisterClass(className, this);
    }

    // Return shared_ptrs to force users to take ownership of the allocated memory.
    //
    // shared_ptr is preserved here to match the original design. If the larger
    // system now wants single-owner semantics, this could be changed to unique_ptr,
    // but that would be an intentional design change.
    [[nodiscard]] virtual pointer MakeProduct() const
    {
        return nullptr;
    }

    [[nodiscard]] virtual pointer MakeProduct(Params) const
    {
        return nullptr;
    }

public:
    FooFactory()
    {
        this->RegisterClass("FooFactory", this);
    }

    virtual ~FooFactory() = default;
};

