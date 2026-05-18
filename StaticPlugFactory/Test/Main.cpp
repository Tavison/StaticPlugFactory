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

#include "Foo.h"
#include "FooFactory.h"

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>


//-------------------------------------------------------------------------------------------------
// Minimal test runner
//-------------------------------------------------------------------------------------------------

static int s_passed = 0;
static int s_failed = 0;

#define REQUIRE(expr) \
    do { \
        if (!(expr)) \
            throw std::runtime_error("REQUIRE(" #expr ") failed"); \
    } while (false)

static void run(const char* name, void(*test)())
{
    try
    {
        test();
        std::cout << "[PASS] " << name << "\n";
        ++s_passed;
    }
    catch (const std::exception& e)
    {
        std::cout << "[FAIL] " << name << " -- " << e.what() << "\n";
        ++s_failed;
    }
}


//-------------------------------------------------------------------------------------------------
// Test fixture: a maker that deliberately omits MakeProduct override.
// Used to verify the base implementation throws rather than silently returning nullptr.
//-------------------------------------------------------------------------------------------------
class FooUnoverridden_Maker : public FooFactory<Foo, const std::string&>
{
    FooUnoverridden_Maker() : FooFactory("FooUnoverridden") {}
    static const FooUnoverridden_Maker registerThis;
};

const FooUnoverridden_Maker FooUnoverridden_Maker::registerThis;


//-------------------------------------------------------------------------------------------------
// Tests
//-------------------------------------------------------------------------------------------------

static void test_foo_a_creates_successfully()
{
    FooFactory<Foo, const std::string&> ff;
    auto foo = ff.NewObject("Foo_A", std::string{ "param" });
    REQUIRE(foo != nullptr);
    REQUIRE(foo->Test() == "This is Foo_A!");
}

static void test_foo_b_creates_successfully()
{
    FooFactory<Foo, const std::string&> ff;
    auto foo = ff.NewObject("Foo_B", std::string{ "param" });
    REQUIRE(foo != nullptr);
    REQUIRE(foo->Test() == "This is Foo_B!");
}

static void test_foo_c_creates_successfully()
{
    FooFactory<Foo, const std::string&> ff;
    auto foo = ff.NewObject("Foo_C", std::string{ "param" });
    REQUIRE(foo != nullptr);
    REQUIRE(foo->Test() == "This is Foo_C!");
}

static void test_foo_d_creates_successfully()
{
    FooFactory<Foo, const std::string&> ff;
    auto foo = ff.NewObject("Foo_D", std::string{ "param" });
    REQUIRE(foo != nullptr);
    REQUIRE(foo->Test() == "This is Foo_D!");
}

static void test_unknown_name_returns_nullptr()
{
    FooFactory<Foo, const std::string&> ff;
    auto foo = ff.NewObject("NoSuchType", std::string{ "param" });
    REQUIRE(foo == nullptr);
}

static void test_unoverridden_make_product_throws()
{
    FooFactory<Foo, const std::string&> ff;
    bool threw = false;
    try
    {
        ff.NewObject("FooUnoverridden", std::string{ "param" });
    }
    catch (const std::logic_error&)
    {
        threw = true;
    }
    REQUIRE(threw);
}


//-------------------------------------------------------------------------------------------------

int main()
{
    run("Foo_A creates and returns correct string",     test_foo_a_creates_successfully);
    run("Foo_B creates and returns correct string",     test_foo_b_creates_successfully);
    run("Foo_C creates and returns correct string",     test_foo_c_creates_successfully);
    run("Foo_D creates and returns correct string",     test_foo_d_creates_successfully);
    run("Unknown name returns nullptr",                 test_unknown_name_returns_nullptr);
    run("Unoverridden MakeProduct throws logic_error",  test_unoverridden_make_product_throws);

    std::cout << "\n" << s_passed << " passed, " << s_failed << " failed.\n";
    return s_failed == 0 ? 0 : 1;
}
