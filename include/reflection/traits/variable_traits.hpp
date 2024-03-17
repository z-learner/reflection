#pragma once

#include <type_traits>

namespace variable_traits {

namespace detail {

template <typename T>
struct variable_type {
  using type = T;
};

template <typename Class, typename T>
struct variable_type<T Class::*> {
  using type = T;
};

template <typename T>
using variable_type_t =
    typename detail::variable_type<std::remove_cv_t<T>>::type;

template <typename T>
struct basic_variable_type {
  using type = variable_type_t<T>;
  static constexpr bool is_member = std::is_member_pointer_v<T>;
};

}  // namespace detail

template <typename T>
struct variable_traits;

template <typename T>
struct variable_traits<T*> : detail::basic_variable_type<T> {
  using pointer_type = T*;
};

template <typename Class, typename T>
struct variable_traits<T Class::*> : detail::basic_variable_type<T> {
  using class_type = Class;
  using pointer_type = T Class::*;
};

}  // namespace variable_traits
