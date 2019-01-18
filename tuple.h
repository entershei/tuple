#ifndef TUPLE_TUPLE_H
#define TUPLE_TUPLE_H

#include <cstddef>
#include <utility>

namespace my {
    template<typename ...Ttuple>
    struct Tuple;

    template<>
    struct Tuple<> {};

    template<typename Tfirst>
    struct Tuple<Tfirst> {
        template<typename Tfirst1, typename = typename std::enable_if
                <!std::is_same<typename std::remove_reference<Tfirst1>::type,
                        Tuple<Tfirst> >::value>::type>
        explicit Tuple(Tfirst1&&  x) :  first(std::forward<Tfirst1>(x)) {}

        constexpr Tuple() = default;

        template<size_t ind, typename Ttuple>
        friend struct index_tuple;

        template<typename T, typename Ttuple>
        friend struct type_tuple;
    private:
        Tfirst first;
    };

    template<typename Tfirst, typename... Trest>
    struct Tuple<Tfirst, Trest...> : public Tuple<Trest...> {
        template<typename Tfirst1, typename ...Trest1>
        constexpr explicit Tuple(Tfirst1 &&first, Trest1&&... rest) : Tuple<Trest...>(std::forward<Trest1>(rest)...),
                                                                      first(std::forward<Tfirst1>(first)) {}

        constexpr Tuple() = default;

        template<size_t ind, typename Ttuple>
        friend struct index_tuple;

        template<typename T, typename Ttuple>
        friend struct type_tuple;

    private:
        Tfirst first;
    };

    template<size_t ind, typename Ttuple>
    struct index_tuple;

    template<typename Tfirst, typename ...Trest>
    struct index_tuple<0, Tuple<Tfirst, Trest...>> {
        constexpr auto& get_value(Tuple<Tfirst, Trest...> &tuple) {
            return tuple.first;
        }

        constexpr auto const& get_value(Tuple<Tfirst, Trest...> const &tuple) {
            return tuple.first;
        }
    };

    template<size_t ind, typename Tfirst, typename ...Trest>
    struct index_tuple<ind, Tuple<Tfirst, Trest...>> {
        constexpr auto& get_value(Tuple<Tfirst, Trest...> &tupl) {
            return index_tuple<ind - 1, Tuple<Trest...>>{}.get_value(tupl);
        }

        constexpr auto const& get_value(Tuple<Tfirst, Trest...> const &tupl) {
            return index_tuple<ind - 1, Tuple<Trest...>>{}.get_value(tupl);
        }
    };

    template<typename ...Ttuple>
    struct cnt_obj;

    template<>
    struct cnt_obj<> {
        constexpr static size_t cnt = 0;
    };

    template<typename Tfirst, typename ...Trest>
    struct cnt_obj<Tfirst, Trest...> {
        constexpr static size_t cnt = 1 + cnt_obj<Trest...>::cnt;
    };

    template<size_t ind, typename... Ttuple>
    constexpr auto& get(Tuple<Ttuple...>& tuple) {
        static_assert(cnt_obj<Ttuple...>::cnt > ind, "Bud call");
        return index_tuple<ind, Tuple<Ttuple...>>{}.get_value(tuple);
    }

    template<size_t ind, typename... Ttuple>
    constexpr auto const& get(Tuple<Ttuple...> const &tuple) {
        static_assert(cnt_obj<Ttuple...>::cnt > ind, "Bud call");
        return index_tuple<ind, Tuple<Ttuple...>>{}.get_value(tuple);
    }

    template<typename T, typename Ttuple>
    struct type_tuple;

    template<typename T, typename Tfirst, typename ...Trest>
    struct type_tuple<T, Tuple<Tfirst, Trest...>> {
        constexpr auto& get_value(Tuple<Tfirst, Trest...> &tupl) {
            return type_tuple<T, Tuple<Trest...>>{}.get_value(tupl);
        }

        constexpr auto const& get_value(Tuple<Tfirst, Trest...> const &tupl) {
            return type_tuple<T, Tuple<Trest...>>{}.get_value(tupl);
        }
    };

    template<typename T, typename ...Trest>
    struct type_tuple<T, Tuple<T, Trest...>> {
        constexpr auto& get_value(Tuple<T, Trest...> &tuple) {
            return tuple.first;
        }

        constexpr auto const& get_value(Tuple<T, Trest...> const &tuple) {
            return tuple.first;
        }
    };

    template<typename T, typename ...Ttuple>
    struct cnt_type;

    template<typename T>
    struct cnt_type<T> {
        constexpr static size_t cnt = 0;
    };

    template<typename T, typename Tfirst, typename ...Trest>
    struct cnt_type<T, Tfirst, Trest...> {
        constexpr static size_t cnt = std::is_same<T, Tfirst>::value + cnt_type<T, Trest...>::cnt;
    };

    template<typename T, typename ...Ttuple>
    constexpr auto& get(Tuple<Ttuple...> &tuple) {
        static_assert(cnt_type<T, Ttuple...>::cnt == 1, "Bud call");
        return type_tuple<T, Tuple<Ttuple...>>{}.get_value(tuple);
    }

    template<typename T, typename ...Ttuple>
    constexpr auto const& get(Tuple<Ttuple...> const &tuple) {
        static_assert(cnt_type<T, Ttuple...>::cnt == 1, "Bud call");
        return type_tuple<T, Tuple<Ttuple...>>{}.get_value(tuple);
    }
} // end my

#endif //TUPLE_TUPLE_H
