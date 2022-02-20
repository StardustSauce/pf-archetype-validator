#include "archetype.h"

Archetype::Archetype(std::string name, std::vector<ReplacementStatus> list) : name(name), statuses(list) { }

std::string Archetype::GetName() const { return name; }

ReplacementStatus Archetype::GetStatus(int position) const { return statuses[position]; }

int Archetype::size() const {return statuses.size(); }

std::string Archetype::StatusToString(ReplacementStatus status) {
  std::string translation;
  switch (status) {
    case NONE:
      translation = "";
      break;
    case OPTIONALLY_REPLACED:
      translation = "(X)";
      break;
    case CHANGED:
      translation = "C";
      break;
    case REPLACED:
      translation = "X";
      break;
    default:
      throw "Archetype Data was not a valid option";
  }
  return translation;
}