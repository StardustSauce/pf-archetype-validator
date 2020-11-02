import re
import csv
from os import listdir


class FileUtils:
    @staticmethod
    def validate_filename():
        file_list = [x.split('.')[0] for x in listdir("data")]
        user_input = input("Give a class name\n> ").lower()
        while user_input not in file_list:
            user_input = input("Please pick a class that exists.\n> ")
        return user_input

    @classmethod
    def load_archetypes(cls, class_name):
        with open(f"data\\{class_name}.csv") as csvfile:
            file_reader = csv.DictReader(csvfile, delimiter=',')
            return [x for x in file_reader]

    @classmethod
    def generate_header(cls, class_name):
        with open(f"data\\{class_name}.csv") as csvfile:
            file_reader = csv.DictReader(csvfile, delimiter=",")
            header = {}
            handled = []
            for field in file_reader.fieldnames:
                pattern = r"(.+) (\d+)"
                non_flat = re.findall(pattern, field)
                if non_flat:
                    if non_flat[0][0] not in handled:
                        header[field] = field
                        handled.append(non_flat[0][0])
                    else:
                        header[field] = non_flat[0][1]
                else:
                    header[field] = field
            return header


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
                    if value == "(X)" and not self.features[key]:
                        self.features[key] = value
                    elif value != "(X)":
                        self.features[key] = value
                        new_archetypes = [x for x in self.compatible_archetypes if not x[key] or x[key] == "(X)"]
                        self.compatible_archetypes = new_archetypes
            return True
        else:
            return False
