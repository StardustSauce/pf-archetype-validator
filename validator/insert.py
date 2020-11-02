import csv
import string

from tabulate import tabulate
import re
from validator import FileUtils


class InsertCli:
    class_name = FileUtils.validate_filename()
    archetypes = FileUtils.load_archetypes(class_name)
    header = FileUtils.generate_header(class_name)
    new_archetypes = []

    @classmethod
    def run_loop(cls):
        exit_function = {
            "restart": cls.run_loop,
            "finish": cls.save_file,
            "quit": lambda: print("Cancelling all additions.")
        }

        new_archetype = {x: "" for x in cls.header}
        cls.new_archetypes.append(new_archetype)
        user_input = string.capwords(input("Choose a unique archetype name\n> "))
        while user_input in [x["Archetype Name"] for x in cls.archetypes] or user_input.lower() in exit_function:
            user_input = input("Name must not already exist or be \"finish\", \"restart\", or \"quit\". Try again\n> ")
        new_archetype["Archetype Name"] = user_input

        while user_input not in exit_function:
            print(tabulate(cls.new_archetypes, headers=cls.header))
            print("\nSelect a feature to change. Follow it with ' ', X, (X), or C. Submit archetypes with \"finish\".")
            user_input = input("Type \"restart\" to create another archetype, or \"quit\" to discard changes.\n> ")
            while True:
                match = re.search(r"^(.+) ([ XCxc]|(?:\([Xx]\)))$", user_input)
                if match:
                    feature_name = string.capwords(match.group(1))
                    if feature_name in cls.header:
                        new_archetype[feature_name] = match.group(2).upper()
                        break
                    else:
                        user_input = input(f"{feature_name} is not a class feature. Try again\n> ")
                elif user_input in exit_function:
                    break
                else:
                    user_input = input("Could not understand input, please try again\n> ")
        exit_function[user_input]()

    @classmethod
    def save_file(cls):
        with open(f"data\\{cls.class_name}.csv", "w") as csvfile:
            writer = csv.DictWriter(csvfile, [x for x in cls.header], delimiter=",")
            cls.archetypes += cls.new_archetypes
            cls.archetypes.sort(key=lambda e: e["Archetype Name"])
            writer.writeheader()
            for archetype in cls.archetypes:
                writer.writerow(archetype)


InsertCli.run_loop()