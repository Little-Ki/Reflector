#pragma once
#include "hash.hpp"
#include "typemeta.hpp"
#include "type.hpp"
#include "store.h"

namespace refl
{

  template<typename T>
  class registrar {
  public:
    registrar(std::string name) : m_name(name) {
    }

    ~registrar() {
      refl::store::set(m_name, m_meta);
    }

    template<typename ...Args>
    registrar& constructor() {
      static_assert(std::is_constructible<T, Args...>());
      static std::function<T*(Args...)> fn = [](Args&&... args) {
        return new T (std::forward<Args>(args) ...);
      };
      size_t hash = refl::hash::hash_types<std::remove_cv_t<Args>...>();
      m_meta.m_ctors[hash] = fn;
      return *this;
    }

    template<typename Getter, typename Setter>
    registrar& property(const std::string&name, Getter getter, Setter setter) {
      using namespace refl::type;

      static_assert(std::is_member_function_pointer_v<Getter>);
      static_assert(std::is_member_function_pointer_v<Setter>);
      static_assert(function_traits<Getter>::arg_count == 0);
      static_assert(function_traits<Setter>::arg_count == 1); 

      using ret_type = typename function_traits<Getter>::return_type;
      using arg_type = typename std::tuple_element<0, typename function_traits<Setter>::arg_types>::type;
      using cls_type = typename function_traits<Getter>::class_type;

      static_assert(std::is_same_v<ret_type, arg_type>);

      static Getter s_getter = getter;
      static Setter s_setter = setter;

      static std::function<std::any (void*)> getter_fn 
      = [&](void* cls) {
        ret_type val = ((*(cls_type*)cls).*s_getter)();
        return std::any((ret_type)val);
      };

      static std::function<void(void*, std::any)> setter_fn 
      = [&](void* cls, const std::any any) {
        ((*(cls_type*)cls).*s_setter)(
          std::any_cast<arg_type>(any)
        );
      };

      m_meta.m_props[name]
      = std::make_tuple(
        refl::hash::hash_type<std::remove_cv_t<arg_type>>(),
        getter_fn, setter_fn
      );

      return *this;
    }

  private:
    type_meta m_meta;

    std::string m_name;

  };
} 
