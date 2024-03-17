#pragma once

#include <cstddef>
#include <type_traits>
/*
  This namespace contains utilities for working with type lists.
*/
namespace type_list {

/*
  Represents a list of types and provides the size of the list as a static
  member 'size'.
*/
template <typename... Args>
struct type_list {
  static constexpr size_t size = sizeof...(Args);
};

namespace detail {
/*
  Extracts the first type from the type list.
*/
template <typename>
struct head;

/*
  Specialization of head to extract the first type from the type list.
*/
template <typename T, typename... Remains>
struct head<type_list<T, Remains...>> {
  using type = T;
};

/*
  Extracts the type list without the first type.
*/
template <typename>
struct tail;

/*
  Specialization of tail to extract the type list without the first type.
*/
template <typename T, typename... Remains>
struct tail<type_list<T, Remains...>> {
  using type = type_list<Remains...>;
};

/*
  Retrieves the type at a specified index in the type list.
*/
template <typename, size_t>
struct nth;

/*
  Specialization of nth to retrieve the type at index 0 in the type list.
*/
template <typename T, typename... Remains>
struct nth<type_list<T, Remains...>, 0> {
  using type = T;
};

/*
  Specialization of nth to retrieve the type at a specific index in the type
  list.
*/
template <typename T, typename... Remains, size_t N>
struct nth<type_list<T, Remains...>, N> {
  static_assert(N > 0);
  static_assert(N < 1 + sizeof...(Remains));
  using type = typename nth<type_list<Remains...>, N - 1>::type;
};

template <typename TypeList, template <typename> typename F, size_t N>
struct count;

template <typename T, typename... Remains, template <typename> typename F>
struct count<type_list<T, Remains...>, F, 0> {
  static constexpr size_t value = F<T>::value ? 1 : 0;
};

template <typename T, typename... Remains, template <typename> typename F,
          size_t N>
struct count<type_list<T, Remains...>, F, N> {
  static constexpr size_t value =
      (F<T>::value ? 1 : 0) + count<type_list<Remains...>, F, N - 1>::value;
};

template <typename, typename>
struct concat;

template <typename... Args1, typename... Args2>
struct concat<type_list<Args1...>, type_list<Args2...>> {
  using type = type_list<Args1..., Args2...>;
};

template <typename... Args1, typename T>
struct concat<type_list<Args1...>, T> {
  using type = type_list<Args1..., T>;
};

template <typename T, typename... Args>
struct concat<T, type_list<Args...>> {
  using type = type_list<T, Args...>;
};

template <typename, template <typename> typename>
struct map;

template <typename... Args, template <typename> typename F>
struct map<type_list<Args...>, F> {
  using type = type_list<typename F<Args>::type...>;
};

template <typename, template <typename> typename>
struct filter;

template <template <typename> typename F>
struct filter<type_list<>, F> {
  using type = type_list<>;
};

template <typename T, typename... Remains, template <typename> typename F>
struct filter<type_list<T, Remains...>, F> {
  using type = std::conditional_t<
      F<T>::value,
      typename concat<T, typename filter<type_list<Remains...>, F>::type>::type,
      typename filter<type_list<Remains...>, F>::type>;
};

}  // namespace detail

/*
  Alias for extracting the first type from a type list.
*/
template <typename TypeList>
using type_list_head_t = typename detail::tail<TypeList>::type;

/*
  Alias for extracting the type list without the first type.
*/
template <typename TypeList>
using type_list_tail_t = typename detail::tail<TypeList>::type;

/*
  Alias for retrieving the type at a specified index in the type list.
*/
template <typename TypeList, size_t Idx>
using type_list_nth_elemt_t = typename detail::nth<TypeList, Idx>::type;

template <typename TypeList, template <typename> typename F>
static constexpr size_t type_list_is_type_count_v =
    detail::count<TypeList, F, TypeList::size - 1>::value;

template <typename T1, typename T2>
using type_list_concat_t = typename detail::concat<T1, T2>::type;

template <typename TypeList, template <typename> typename F>
using type_list_map_t = typename detail::map<TypeList, F>::type;

template <typename TypeList, template <typename> typename F>
using type_list_filter_t = typename detail::filter<TypeList, F>::type;

}  // namespace type_list