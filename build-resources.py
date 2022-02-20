import os, string

def convert_to_name(filename):
  if filename.endswith(".csv"):  
    name = file[:-4]
    name = name.replace(" ", "_")
    name = name.upper()
    return name
  else:
    return None

def strip_iterative(feature_list):
  def split_on_last(text):
    split_index = text.rfind(" ")
    if split_index != -1:
      return text[:split_index], text[split_index + 1:]
    else:
      return text, None
  valid_suffixes = ["I", "II", "III", "IV", "V", "VI", "VII", "VIII", "IX", "Gate"] + [str(x) for x in range(1, 20)]
  corrected = [feature_list[0]]
  for i, raw_feature in enumerate(feature_list[1:]):
    previous_feature, prev_suffix = split_on_last(feature_list[i])
    feature, suffix = split_on_last(raw_feature)
    if previous_feature == feature and prev_suffix in valid_suffixes:
      corrected.append(suffix)
    else:
      corrected.append(raw_feature)
  return corrected

file_list = os.listdir("resources")
file_list.sort()


h = [r"""#pragma once

#include <string>
#include <unordered_map>
#include "pf-class.h"
#include "archetype.h"
#include "replacement-status.h"

namespace PfData {
  const std::unordered_map<std::string, PfClass> CLASSES = {"""]

classes = []
for file in file_list:
    class_string = ["    {" + f"\"{string.capwords(file[:-4])}\", PfClass("]

    with open(os.path.join("resources", file), "r") as f:
      content = f.readlines();
    features = [f"{x.strip()}" for x in content[0].split(",")[1:]]
    features = strip_iterative(features)
    features = [f"\"{x}\"" for x in features]
    features = ", ".join(features)
    class_string.append("      std::vector<std::string> { " + features + " },")

    archetypes = []
    ReplacementStatus = {"" : "NONE", "(X)" : "OPTIONALLY_REPLACED", "C" : "CHANGED", "X" : "REPLACED"}
    for line in content[1:]:
      name, statuses = line.split(",", 1)
      name = f"\"{name}\""
      statuses = [f"{ReplacementStatus[x.strip()]}" for x in statuses.split(",")]
      statuses = f"{'{'} {', '.join(statuses)} {'}'}"
      archetypes.append(f"        Archetype({name}, {statuses})")
    archetypes = ",\n".join(archetypes)
    class_string[-1] += " {\n" + archetypes + "\n      }\n    )}"
    # class_string.append("      std::vector<std::vector<Archetype::ReplacementStatus>> {\n        " + archetypes + "\n      })\n    }")
    classes.append("\n".join(class_string))

h.append(",\n".join(classes))

h.append("  };\n};\n")
h = "\n".join(h)

with open(os.path.join("source", "pf-data.h"), "w") as f:
  f.write(h)
