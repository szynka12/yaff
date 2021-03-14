#ifndef ZIPPER_H
#define ZIPPER_H

#include <type_traits>
#include <vector>
#include <tuple>

// Source: https://committhis.github.io/2020/10/14/zip-iterator.html

template <typename T>
using select_iterator_for 
  = std::conditional_t<
    std::is_const_v<std::remove_reference_t<T>>,
    typename std::decay_t<T>::const_iterator, 
    typename std::decay_t<T>::iterator 
  >;

template <typename Iter>
using select_access_type_for 
  = std::conditional_t<
        std::is_same_v<Iter, std::vector<bool>::iterator> 
    ||  std::is_same_v<Iter, std::vector<bool>::const_iterator>,
    typename std::iterator_traits<Iter>::value_type,
    typename std::iterator_traits<Iter>::reference
  >;

/*  The index sequence is only used to deduce the Index sequence in the template
    declaration. It uses a fold expression which is applied to the indexes,
    using each expanded value to compare tuple value at that index. If any of
    the tuple elements are equal, the function will return true. */
template <typename ... Args, std::size_t ... Index>
auto any_match_impl(std::tuple<Args...> const & lhs,
        std::tuple<Args...> const & rhs,
        std::index_sequence<Index...>) -> bool {
    return (... | (std::get<Index>(lhs) == std::get<Index>(rhs)));
}


/*  User function for finding any elementwise match in two tuples. Forwards to
    to the implementation the two tuples along with a generated index sequence
    which will have the same length as the tuples. */
template <typename ... Args>
auto any_match(std::tuple<Args...> const & lhs, 
        std::tuple<Args...> const & rhs) -> bool 
{
    return any_match_impl(lhs, rhs, std::index_sequence_for<Args...>{});
}


template <typename ... Iters>
class zip_iterator
{
public:
    using value_type = std::tuple<
        select_access_type_for<Iters>...
    >;

    zip_iterator() = delete;

    zip_iterator(Iters && ... iters)
        : m_iters {std::forward<Iters>(iters)...}
    {
    }

    /* Increment all the underlying iterators */
    auto operator++() -> zip_iterator& {
        std::apply([](auto & ... args){
                ((args += 1), ...);
            }, m_iters);
        return *this;
    }

    auto operator++(int) -> zip_iterator {
        auto tmp = *this;
        ++*this;
        return tmp;
    }

    auto operator!=(zip_iterator const & other){
        return !(*this == other);
    }

    /*  Elementwise compare another iterator of this type to detect if any
        of the elements match. */
    auto operator==(zip_iterator const & other){
        return any_match(m_iters, other.m_iters);
    }

    /*  Dereference operator that constructs a tuple (`value_type`) by
        expanding and dereferencing each of the underlying iterators in */
    auto operator*() -> value_type {
        return std::apply([](auto && ... args){ 
                return value_type(*args...); 
            }, m_iters);
    }

private:
    std::tuple<Iters...> m_iters;
};

template <typename ... T>
class zipper
{
public:
    using zip_type = zip_iterator<select_iterator_for<T> ...>;

    /*  std::forward is used to preserve the value category of the
        containers */
    template <typename ... Args>
    zipper(Args && ... args)
        : m_args{std::forward<Args>(args)...}
    {
    }

    auto begin() -> zip_type {
        return std::apply([](auto && ... args){ 
                return zip_type(std::begin(args)...); 
            }, m_args);
    }

    auto end() -> zip_type {
        return std::apply([](auto && ... args){ 
                return zip_type(std::end(args)...); 
            }, m_args);
    }

private:
    std::tuple<T ...> m_args;
};

template <typename ... T>
auto zip(T && ... t) {
    return zipper<T ...>{std::forward<T>(t)...};
}

#endif /* ZIPPER_H */
