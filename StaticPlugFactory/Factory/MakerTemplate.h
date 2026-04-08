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

#include "PlugFactoryTemplate.h"

// Manages the creation of objects.
//
// BaseType         is the virtual base class for the family to be created.
// BaseFactoryType  is the base class for the factory that makes the family.
// ParamsType       is the object used in the BaseType's constructor.
// KeyType          is the lookup key for the system.
template<
    class BaseType,
    class BaseFactoryType,
    class ParamsType,
    class KeyType = std::string>
class ProductFactory
{
public:
    using base_type = BaseType;
    using base_factory_type = BaseFactoryType;
    using params_type = ParamsType;
    using key_type = KeyType;
    using pointer = std::shared_ptr<base_type>;

    // Make a new object with no parameters.
    [[nodiscard]] pointer NewObject(std::string_view name) const
    {
        // Get the factory.
        const base_factory_type* factory = m_Registry.GetFactory(name);
        if (factory)
        {
            // Return the product.
            return factory->MakeProduct();
        }

        // nullptr indicates no product.
        // This could very easily throw an exception instead, depending on policy.
        return nullptr;
    }

    // Make a new object based on some parameter object.
    [[nodiscard]] pointer NewObject(std::string_view name, params_type params) const
    {
        // Get the factory.
        const base_factory_type* factory = m_Registry.GetFactory(name);
        if (factory)
        {
            // Return the product.
            return factory->MakeProduct(params);
        }

        // nullptr indicates no product.
        // This could very easily throw an exception instead, depending on policy.
        return nullptr;
    }

protected:
    // Protected so only derived objects can create one.
    ProductFactory() = default;
    ~ProductFactory() = default;

    void RegisterClass(std::string_view className, const base_factory_type* factory)
    {
        m_Registry.RegisterClass(className, factory);
    }

private:
    // This object provides access to the shared static registry.
    // The actual registry storage is private to PlugFactory.
    PlugFactory<base_factory_type, key_type> m_Registry;
};