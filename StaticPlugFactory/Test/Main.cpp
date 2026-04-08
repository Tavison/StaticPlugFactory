#include "Foo.h"
#include "../Factory/FooFactory.h"

#include <iostream>
#include <memory>
#include <string>

int main()
{
    FooFactory<Foo, const std::string&> ff;
    std::shared_ptr<Foo> foo = ff.NewObject("Foo_A", std::string{ "Blah" });

    if (!foo)
    {
        std::cerr << "Failed to create Foo_A.\n";
        return 1;
    }

    std::cout << foo->Test();

    return 0;
}

