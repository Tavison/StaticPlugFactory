/*
MIT License

Copyright (c) 2026 James Edwards

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "FooFactory.h"
#include "Foo.h"

#include <memory>
#include <string>


class Foo_D : public Foo
{
public:
    explicit Foo_D(const std::string&) {}

    [[nodiscard]] std::string Test() override
    {
        return "This is Foo_D!";
    }
};


class Foo_D_Maker : public FooFactory<Foo, const std::string&>
{
    Foo_D_Maker() : FooFactory("Foo_D") {}
    static const Foo_D_Maker registerThis;

public:
    [[nodiscard]] std::shared_ptr<Foo> MakeProduct(const std::string& params) const override
    {
        return std::make_shared<Foo_D>(params);
    }
};

const Foo_D_Maker Foo_D_Maker::registerThis;
