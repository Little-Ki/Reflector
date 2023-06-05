#pragma once
#include <type_traits>

namespace refl {

  namespace type {

    template <typename T>
    struct function_traits : function_traits< decltype(&T::operator()) > {};

    template<typename R, typename... Args>
    struct function_traits<R (Args...)>
    {
        static constexpr size_t arg_count = sizeof...(Args);
        using return_type   = R;
        using arg_types     = std::tuple<Args...>;
    };

    template<typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...)> : function_traits<R (Args...)> { using class_type = C; };

    template<typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...) const> : function_traits<R (Args...)> { using class_type = C; };

    template<typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...) volatile> : function_traits<R (Args...)> { using class_type = C; };

    template<typename R, typename C, typename... Args>
    struct function_traits<R (C::*)(Args...) const volatile> : function_traits<R (Args...)> {using class_type = C; };
    
  }

}