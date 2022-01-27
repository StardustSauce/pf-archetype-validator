import csv

import re, os
from tabulate import tabulate
from validator import FileUtils


class InsertCli:
    FileUtils.set_cwd()
    class_name = FileUtils.validate_filename()
    archetypes = FileUtils.load_archetypes(class_name)
    header = FileUtils.generate_header(class_name)
    iterative_fields = FileUtils.find_iterative(header)
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
        user_input = input("Choose a unique archetype name (Case Sensitive!)\n> ")
        def lower_input(): user_input.lower()
        while lower_input() in [x["Archetype Name"].lower() for x in cls.archetypes] or lower_input() in exit_function:
            user_input = input("Name must not already exist or be \"finish\", \"restart\", or \"quit\". Try again\n> ")
        new_archetype["Archetype Name"] = user_input

        while user_input not in exit_function:
            print(tabulate(cls.new_archetypes, headers=cls.header))
            print("\nSelect a feature to change. Follow it with ' ', X, (X), or C. Submit archetypes with \"finish\".")
            user_input = input("Type \"restart\" to create another archetype, or \"quit\" to discard changes.\n> ")
            while True:
                match = re.search(r"^(.+) ([ XCxc]|(?:\([Xx]\)))$", user_input)
                if match:
                    feature_input = match.group(1).lower()
                    feature_found = False
                    for feature in cls.header:
                        if feature_input == feature.lower() and feature != "Archetype Name":
                            new_archetype[feature] = match.group(2).upper()
                            feature_found = True
                            break
                    else:
                        for iterative in cls.iterative_fields:
                            if feature_input == iterative.lower():
                                for feature in cls.header:
                                    if feature.startswith(iterative):
                                        new_archetype[feature] = match.group(2).upper()
                                feature_found = True
                    if feature_found:
                        break
                    else:
                        user_input = input(f"{feature_input} is not a class feature. Try again\n> ")
                elif user_input in exit_function:
                    break
                else:
                    user_input = input("Could not understand input, please try again\n> ")
        exit_function[user_input]()

    @classmethod
    def save_file(cls):
        path = os.path.join("data", cls.class_name + ".csv")
        with open(path, "w", newline='') as csvfile:
            writer = csv.DictWriter(csvfile, [x for x in cls.header], delimiter=",")
            cls.archetypes += cls.new_archetypes
            cls.archetypes.sort(key=lambda e: e["Archetype Name"])
            writer.writeheader()
            for archetype in cls.archetypes:
                writer.writerow(archetype)


InsertCli.run_loop()
