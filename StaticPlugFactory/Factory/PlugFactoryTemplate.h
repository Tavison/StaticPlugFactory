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

//-------------------------------------------------------------------------------------------------
//  FILE:   PlugFactoryTemplate.h
//  AUTHOR: James Edwards
//  DESC:   This provides the base for a pluggable factory system.
//
//          Add this into a factory base to get the benefits of a static
//          self-registering system.
//-------------------------------------------------------------------------------------------------

#pragma once

#include <concepts>
#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>

template<typename T>
concept HashableKey =
    requires(const T & a, const T & b)
{
    { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
    { a == b } -> std::convertible_to<bool>;
};

//-------------------------------------------------------------------------------------------------
// This provides the base for pluggable factories.
// Derived product families provide the actual creation methods.
//-------------------------------------------------------------------------------------------------
template<typename Factory, HashableKey Key = std::string>
class PlugFactory
{
public:
    using factory_type = Factory;
    using key_type = Key;
    using registry_type = std::unordered_map<key_type, const factory_type*>;
    using iterator = typename registry_type::iterator;
    using const_iterator = typename registry_type::const_iterator;

    // Return the factory desired.
    [[nodiscard]] const factory_type* GetFactory(const key_type& className) const noexcept
    {
        const auto& registry = GetRegistry();
        const auto iter = registry.find(className);

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
    void RegisterClass(const key_type& factoryName, const factory_type* factory)
    {
        GetRegistry().try_emplace(factoryName, factory);
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

