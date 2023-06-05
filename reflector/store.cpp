#include "store.h"

using namespace refl;

std::shared_ptr<std::unordered_map<std::string, type_meta>> store::metadata;

void  refl::store::set(const std::string& name, const type_meta & meta)
{
  init();
  (*metadata)[name] = meta;
}

const type_meta& refl::store::get(const std::string& name)
{
  init();
  return (*metadata)[name];
}

bool refl::store::exists(const std::string& name) {
  init();
  return metadata->find(name) != metadata->end();
}

void refl::store::init() {
  if (metadata == nullptr) {
    metadata = std::make_unique<std::unordered_map<std::string, type_meta>>();
  }
}