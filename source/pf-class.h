#pragma once

#include <vector>
#include <string>
#include <assert.h>
#include "archetype.h"

// Class definitions
class PfClass {
  public:
    const std::vector<std::string> features;
    const std::vector<Archetype> archetypes;

    PfClass(std::vector<std::string>, std::vector<Archetype>);
};
