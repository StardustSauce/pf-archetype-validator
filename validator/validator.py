import csv
import re
import string
from tabulate import tabulate


class CustomClass:
    def __init__(self, archetype_list):
        self.features = {}
        for key in archetype_list[0]:
            self.features[key] = ""
        self.features["Archetype Name"] = "Custom Class"
        self.compatible_archetypes = archetype_list.copy()
        self.archetypes = []

    def add_archetype(self, name):
        def strip(test_string):
            match = re.search(r"^([^(]+\w)", test_string)
            if match:
                return match.group(1).lower()
            else:
                return name.lower()

        archetype = next((x for x in self.compatible_archetypes if strip(x["Archetype Name"]) == strip(name)), None)
        if archetype:
            name = archetype["Archetype Name"]
            self.archetypes.append(name)
            self.compatible_archetypes = [x for x in self.compatible_archetypes if not x["Archetype Name"] == name]
            for key, value in archetype.items():
                if value and key != "Archetype Name":
                    self.features[key] = value
                    self.compatible_archetypes = [x for x in self.compatible_archetypes if not x[key]]
            return True
        else:
            return False


class Utils:
    @classmethod
    def load_archetypes(cls):
        archetypes = []
        while True:
            class_name = input("Give a class name\n> ")
            try:
                with open(f"data\\{class_name}.csv") as csvfile:
                    reader = csv.DictReader(csvfile, delimiter=',')
                    for row in reader:
                        archetypes.append(row)
                break
            except FileNotFoundError:
                print("Please pick a class that exists.")
        return archetypes

    @classmethod
    def generate_header(cls, sample_archetype):
        header = {}
        handled = []
        for field in sample_archetype:
            print(field)
            pattern = r"(.+) (\d+)"
            nonFlat = re.findall(pattern, field)
            if nonFlat:
                if nonFlat[0][0] not in handled:
                    header[field] = field
                    handled.append(nonFlat[0][0])
                else:
                    header[field] = nonFlat[0][1]
            else:
                header[field] = field
        return header


class Cli:
    archetypes = Utils.load_archetypes()
    header = Utils.generate_header(archetypes[0])

    @classmethod
    def run_loop(cls):
        user_input = "Restart"
        while user_input == "Restart":
            user_input = ""
            custom_class = CustomClass(cls.archetypes)
            while user_input != "Quit" and user_input != "Restart":
                print(tabulate(custom_class.compatible_archetypes + [custom_class.features], headers=cls.header))
                print("\nCustom Class has the archetypes:")
                for archetype in custom_class.archetypes:
                    print(f"\t{archetype}")
                found = None
                while not found and user_input != "Quit" and user_input != "Restart":
                    iteration_text = {
                        None: "\nSelect an archetype to add to your class, or type quit or restart\n> ",
                        False: "Archetype was not found. Try again\n> "
                    }
                    user_input = string.capwords(input(iteration_text[found]))
                    found = custom_class.add_archetype(user_input)


Cli.run_loop()
