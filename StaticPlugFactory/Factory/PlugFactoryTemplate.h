//-------------------------------------------------------------------------------------------------
//  FILE:   PlugFactoryTemplate.h
//  AUTHOR: James Edwards
//  DESC:   This provides the base for a pluggable factory system.
//
//          Add this into a factory base to get the benefits of a static
//          self-registering system.
//-------------------------------------------------------------------------------------------------

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

//-------------------------------------------------------------------------------------------------
// This provides the base for pluggable factories.
// Derived product families provide the actual creation methods.
//-------------------------------------------------------------------------------------------------
template<typename Factory, typename Key = std::string>
class PlugFactory
{
public:
    using factory_type = Factory;
    using key_type = Key;
    using registry_type = std::unordered_map<key_type, const factory_type*>;
    using iterator = typename registry_type::iterator;
    using const_iterator = typename registry_type::const_iterator;

    // Return the factory desired.
    [[nodiscard]] const factory_type* GetFactory(std::string_view className) const noexcept
    {
        const auto& registry = GetRegistry();
        const auto  iter = registry.find(key_type{ className });

        if (iter == registry.end())
        {
            return nullptr;
        }

        return iter->second;
    }

    // Register the names of the products that can be produced.
    //
    // If the same name is registered more than once, the first registration
    // is preserved and later attempts are ignored. This matches the behavior
    // of the original insert-based implementation.
    void RegisterClass(std::string_view factoryName, const factory_type* factory)
    {
        GetRegistry().try_emplace(key_type{ factoryName }, factory);
    }

    PlugFactory() = default;
    ~PlugFactory() = default;

    PlugFactory(const PlugFactory&) = default;
    PlugFactory& operator=(const PlugFactory&) = default;
    PlugFactory(PlugFactory&&) noexcept = default;
    PlugFactory& operator=(PlugFactory&&) noexcept = default;

private:
    // This is the actual shared registry for a given Factory/Key specialization.
    //
    // It is intentionally private. Users of the pattern are meant to work
    // through the factory interface, not by manipulating the registry directly.
    //
    // A function-local static is used so the registry is created on first use.
    // In modern C++, this initialization is thread-safe.
    static registry_type& GetRegistry() noexcept
    {
        static registry_type registry;
        return registry;
    }
};

