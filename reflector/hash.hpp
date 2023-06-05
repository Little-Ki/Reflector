#pragma once
#include <type_traits>

namespace refl {
  namespace hash {
    template <typename T>
    constexpr size_t hash_type()
    {
        size_t result{};
        #ifdef _MSC_VER 
          #define F __FUNCSIG__
        #else
          #define F __PRETTY_FUNCTION__
        #endif
        for (const auto &c : F)
            (result ^= c) <<= 1;
        return result;
    };

    template<typename ...Args>
    constexpr size_t hash_types() {
      size_t result{};
      constexpr size_t hash[] = {
         hash_type<Args>() 
         ... 
      };
      for (const auto &c : hash)
        (result ^= c) <<= 1;
      return result;
    };

    template<>
    constexpr size_t hash_types() {
      return hash_type<void>();
    }
  }
}