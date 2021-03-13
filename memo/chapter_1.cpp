#include <iostream>

template <typename T>
constexpr size_t SizeOf = sizeof(T);

template <typename T, size_t size>
constexpr bool SizeMatch = SizeOf<T> == size;


template<int32_t N>
struct Identity_{
    static constexpr int32_t value = N;
};


// `conditional_t` is similar to triple operator `a?b:c`, but the choices can only be type

template <int32_t N, int32_t M>
struct commonDivs_{
    static constexpr int32_t value = std::conditional_t<M==0, Identity_<N>, commonDivs_<M, M==0?0:N%M>>::value;
};

template <int32_t N, int32_t M>
constexpr int32_t commonDivs = commonDivs_<N,M>::value;

template <typename T>
struct has_type_{
    private:
        template <typename A, typename B>
        struct helper{using type = B;};
        
        template <typename  C> static std::false_type foo(float); 
        template <typename  C> static typename helper<typename C::type, std::true_type>::type foo(int);
    public:
        constexpr static bool value = decltype(foo<T>(0))::value;
};

struct StructHasType{
    using type = int32_t;
};

struct StructNoType{};

template <int32_t ...>
struct if_exist_one_;

template <int32_t Head, int32_t ...Tails>
struct if_exist_one_<Head, Tails...>{
    private:
        template <bool Cur, typename NextT>
        constexpr static bool AndValue = true;
        template <typename NextT>
        constexpr static bool AndValue<false, NextT> = NextT::value;
    public: 
        constexpr static bool value = AndValue<Head == 1, if_exist_one_<Tails...>>;
};

template <>
struct if_exist_one_<>{
    constexpr static bool value = false;
};

template<int32_t ...Ints>
constexpr bool if_exist_one = if_exist_one_<Ints...>::value;

template <typename ...Args>
struct variadic_to_tuple_{
    using Tuple = std::tuple<Args...>; // pack template parameters into a tuple
    static constexpr size_t Size = sizeof...(Args); 
    template <size_t N>
        using Nth = typename std::tuple_element<N, Tuple>::type;
    using First = Nth<0>;
};

template <typename ...Args>
using convert_to_size = std::integer_sequence<size_t, sizeof(Args)...>;

//template <typename Head, typename ...Tails>
//struct convert_to_size_<Head, Tails>{
//    private:
//        using tail_sizes_seq = convert_to_size_<Tails...>;
//        using head_sizes_seq = std::integer_sequence<size_t, sizeof(Head)>;
//    public:
//        using sizes = std::integer_sequence<size_t, sizeof())
//};
//
//template <>
//struct convert_to_size_<>{
//    using sizes = std::integer_sequence<size_t>; 
//}

// debugging aid
template<typename T, T... ints>
void print_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '),...);
    std::cout << '\n';
}

int main(){
    std::cout << "size of int is " << SizeOf<int> << std::endl;
    std::cout << "Assertion: `size of int is 3` is " << std::boolalpha << SizeMatch<int, 3> << std::endl;
    std::cout << "The largest common division of 8 and 6 is " << commonDivs<8, 6> << std::endl;
    std::cout << "Does `StructNoType` have `type`? " << std::boolalpha << has_type_<StructNoType>::value << std::endl;
    std::cout << "Does `StructHasType` have `type`? " << std::boolalpha << has_type_<StructHasType>::value << std::endl;
    std::cout << "Does List `(1,2,3)` have `1`? " << std::boolalpha << if_exist_one<1,2,3> << std::endl;
    std::cout << "Does List `(0,2,3)` have `1`? " << std::boolalpha << if_exist_one<0,2,3> << std::endl;
    std::cout << "size of list `(int, float, double, int64_t)` is ";
    convert_to_size<int, float, double, int64_t> sizes_seq;
    print_sequence(sizes_seq);
}
