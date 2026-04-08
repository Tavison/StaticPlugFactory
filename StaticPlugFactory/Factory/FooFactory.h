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

