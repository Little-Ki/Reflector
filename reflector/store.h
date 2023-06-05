#pragma once
#include <string>
#include <unordered_map>
#include <memory>

#include "typemeta.hpp"
#include "hash.hpp"

namespace refl {
  class store {
  public:
    static void set(const std::string& name, const type_meta& meta);

    static bool exists(const std::string& name);

    static const type_meta& get(const std::string& name);

  protected:

    static std::shared_ptr<std::unordered_map<std::string, type_meta>> metadata;
    
    static void init();

  };
}