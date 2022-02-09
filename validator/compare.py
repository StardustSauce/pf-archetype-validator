from validator import FileUtils

FileUtils.set_cwd()
class_name = FileUtils.validate_filename()
stored_names = [x["Archetype Name"] for x in FileUtils.load_archetypes(class_name)]

input_names = []
print("Add all archetypes from the official list, one at a time")
while True:
  user_input = input("> ")
  if user_input == "done":
    break
  input_names.append(user_input.strip())

missing_names = []
for name in input_names:
  if name not in stored_names:
    missing_names.append(name)

if missing_names:
  print("Missing Names:")
  for name in missing_names:
    print("\t" + name)
else:
  print("This class is complete!")
