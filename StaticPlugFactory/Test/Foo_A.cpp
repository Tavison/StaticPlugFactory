/*
MIT License

Copyright (c) 2026 James Edwards

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the “Software”), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

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
    // This is the only time it is created, which is why the
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