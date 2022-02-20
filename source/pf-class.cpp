#include "pf-class.h"

PfClass::PfClass(std::vector<std::string> features, std::vector<Archetype> archetypes) : features(features), archetypes(archetypes) {
  for (size_t i = 0; i < archetypes.size(); i++) {
    assert(archetypes[i].size() == features.size());
  }
}