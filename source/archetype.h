#pragma once

#include <vector>
#include <string>
#include "replacement-status.h"

class Archetype {
  // variable declaration
  public:
  private:
    const std::string name;
    const std::vector<ReplacementStatus> statuses;

  // function declaration
  public:
    Archetype(std::string, std::vector<ReplacementStatus>);
    static std::string StatusToString(ReplacementStatus);
    std::string GetName() const;
    ReplacementStatus GetStatus(int) const;
    int size() const;
  private:
};
