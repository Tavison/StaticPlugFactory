#include "../Factory/FooFactory.h"
#include "Foo.h"

#include <memory>
#include <string>


class Foo_A : public Foo
{
public:
    explicit Foo_A(const std::string&)
    {
    }

    [[nodiscard]] std::string Test() override
    {
        return "This is Foo_A!";
    }
};


//-------------------------------------------------------------------------------------------------
// CLASS:   Foo_A_Maker
//
// This creates a simple Foo object.
//-------------------------------------------------------------------------------------------------
class Foo_A_Maker : public FooFactory<Foo, const std::string&>
{
private:
    // Register this as a type using a static instance of itself.
    // This is really the only time it is created, which is why the
    // constructor is private.
    Foo_A_Maker()
        : FooFactory("Foo_A")
    {
    }

    static const Foo_A_Maker registerThis;

public:
    [[nodiscard]] std::shared_ptr<Foo> MakeProduct(const std::string& params) const override;
};


//-------------------------------------------------------------------------------------------------
// Static member variable to register the object.
//-------------------------------------------------------------------------------------------------
const Foo_A_Maker Foo_A_Maker::registerThis;


//-------------------------------------------------------------------------------------------------
// Parameterized constructor.
//-------------------------------------------------------------------------------------------------
[[nodiscard]] std::shared_ptr<Foo> Foo_A_Maker::MakeProduct(const std::string& params) const
{
    return std::make_shared<Foo_A>(params);
}