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