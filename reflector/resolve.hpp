#pragma once
#include <string>
#include <any>
#include "hash.hpp"
#include "store.h"

namespace refl {

  class resolve {
  public:

    resolve(const std::string& cls_name) : m_cls_name(cls_name) {};

    template<typename T, typename ...Args>
    T* new_instance(Args ...args) {
      constexpr auto hash = refl::hash::hash_types<std::remove_cv_t<Args>...>();

      if (!refl::store::exists(m_cls_name)) return nullptr;

      auto meta = refl::store::get(m_cls_name);

      if (meta.m_ctors.find(hash) == meta.m_ctors.end()) return nullptr;

      auto fn  = std::any_cast<std::function<T*(Args...)>>(meta.m_ctors[hash]);

      return fn(args...);
    }

    template<typename T>
    T* new_instance() {
      constexpr auto hash = refl::hash::hash_types<>();

      if (!refl::store::exists(m_cls_name)) return nullptr;

      auto meta = refl::store::get(m_cls_name);

      if (meta.m_ctors.find(hash) == meta.m_ctors.end()) return nullptr;

      auto fn  = std::any_cast<std::function<T*()>>(meta.m_ctors[hash]);

      return fn();
    }


    template<typename T>
    T get_prop(void* ptr, const std::string name) {
      if (!refl::store::exists(m_cls_name)) return T {};

      auto& meta = refl::store::get(m_cls_name);

      if (meta.m_props.find(name) == meta.m_props.end()) return T {};

      auto tuple = meta.m_props[name];


      if (refl::hash::hash_type<std::remove_cv_t<T>>() == std::get<0>(tuple)) {
        auto fn = std::any_cast<std::function<std::any (void*)>>(std::get<1>(tuple));

        return std::any_cast<T>(fn(ptr));
      }

      return T {};
    }

    template<typename T>
    void set_prop(void* ptr, const std::string name, T val) {
      if (!refl::store::exists(m_cls_name)) return;
      
      auto meta = refl::store::get(m_cls_name);

      if (meta.m_props.find(name) == meta.m_props.end()) return;

      auto tuple = meta.m_props[name];

      if (refl::hash::hash_type<std::remove_cv_t<T>>() == std::get<0>(tuple)) {
        auto fn = std::any_cast<std::function<void (void*, std::any)>>(std::get<2>(tuple));
        fn(ptr, val);
      }
    }

  private:
    std::string m_cls_name;

  };
}
