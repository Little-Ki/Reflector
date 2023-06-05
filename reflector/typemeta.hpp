#pragma once
#include <string>
#include <vector>
#include <functional>
#include <any>
#include <tuple>
#include <unordered_map>

namespace refl {
  
  struct type_meta {
    std::unordered_map<size_t, std::any> m_ctors;

    std::unordered_map<std::string, std::tuple<size_t, std::any, std::any>> m_props;
  };

}