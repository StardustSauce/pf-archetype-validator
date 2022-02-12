from collections import defaultdict
from validator import FileUtils
import os, csv

FileUtils.set_cwd()

count = 0
for file in os.listdir("data"):
  name = file.split(".")[0]
  header = [x for x in FileUtils.generate_header(name).keys()]
  archetypes = FileUtils.load_archetypes(name)

  iterative_fields = { x:[y for y in header if y.startswith(x)] for x in FileUtils.find_iterative(header) }
  for field_name, fields in iterative_fields.items():
    all_same = []
    for archetype in archetypes:
      archetype_iteratives = [archetype[x] for x in fields]
      all_same.append(all(x == archetype_iteratives[0] for x in archetype_iteratives))
    if all(x for x in all_same) and field_name != "Bonus Feat":
      index = header.index(fields[0])
      header[index] = field_name
      for archetype in archetypes:
        archetype[field_name] = archetype[fields[0]] 
      del(archetype[fields[0]])
      for field in fields[1:]:
        header.remove(field)
  
  archetypes = [{x:y for x, y in archetype.items() if x in header} for archetype in archetypes]

  checked_features = ["Class Skills", "Skill Ranks Per Level", "BAB", "Hit Die", "Weapon & Armor", "Spells"]
  for feature in checked_features:
    if feature not in header:
      continue
    all_null = True
    for archetype in archetypes:
      if archetype[feature] != "":
        all_null = False
        break
    if all_null:
      print(f"{name}'s feature {feature} is all blank!")
      header.remove(feature)
      for archetype in archetypes:
        del archetype[feature]
  
  path = os.path.join("data", name + ".csv")
  with open(path, "w", newline='') as csvfile:
      writer = csv.DictWriter(csvfile, [x for x in header], delimiter=",")
      writer.writeheader()
      for archetype in archetypes:
          writer.writerow(archetype)
    