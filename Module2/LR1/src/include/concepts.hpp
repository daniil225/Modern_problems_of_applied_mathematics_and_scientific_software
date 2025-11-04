#pragma once 

#include <type_traits>
#include <concepts>


template <typename T>
concept floating_point = std::is_floating_point_v<T>;

template <typename T>
concept floating_point_ref =
    std::is_lvalue_reference_v<T> &&
    floating_point<std::remove_reference_t<T>>;

template <typename T>
concept floating_point_const_ref =
    std::is_lvalue_reference_v<T> &&
    std::is_const_v<std::remove_reference_t<T>> &&
    floating_point<std::remove_cvref_t<T>>;

template <typename F, typename T>
concept lose_function_t =
    floating_point<T> &&
    std::invocable<F, T, T> &&
    std::same_as<std::invoke_result_t<F, T, T>, T>;

template <typename C>
concept realization_container =
    requires(C &obj, const C &c_obj, std::size_t i) {
        requires floating_point<typename C::value_type>;

        { c_obj.size() } -> std::integral;

        { obj[i] } -> floating_point_ref;
        { c_obj[i] } -> floating_point_const_ref;

        { obj.begin() };
        { obj.end() };

        { obj.cbegin() };
        { obj.cend() };

        { c_obj.cbegin() };
        { c_obj.cend() };
    };

