import re
import csv
import os
from inspect import getsourcefile


class FileUtils:
    @staticmethod
    def set_cwd():
        path = os.path.dirname(getsourcefile(lambda:0))
        os.chdir(path)

    @staticmethod
    def validate_filename():
        no_archetypes = ["unchained barbarian", "unchained rogue"]
        file_list = [x.split('.')[0] for x in os.listdir("data")]
        user_input = input("Give a class name\n> ")
        while True:
            user_input = user_input.lower()
            if user_input in no_archetypes:
                user_input = input(f"{user_input} has no archetypes.\n> ")
            elif user_input == "vampire hunter":
                user_input = input("What's wrong with you?\n> ")
            elif user_input == "unchained summoner":
                print("Unchained Summoner archetypes have been included in summoner. Launching Summoner...")
                user_input = "summoner"
                break
            elif user_input not in file_list:
                user_input = input(f"{user_input} is not a known class.\n> ")
            else:
                break
        return user_input

    @classmethod
    def load_archetypes(cls, class_name):
        path = os.path.join("data", class_name + ".csv")
        with open(path) as csvfile:
            file_reader = csv.DictReader(csvfile, delimiter=',')
            return [x for x in file_reader]

    @classmethod
    def generate_header(cls, class_name):
        path = os.path.join("data", class_name + ".csv")
        with open(path) as csvfile:
            file_reader = csv.DictReader(csvfile, delimiter=",")
            header = {}
            handled = []
            for field in file_reader.fieldnames:
                pattern = r"(.+) (\+?\d+)"
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

    @classmethod
    def find_iterative(cls, columns):
        iterative_fields = []
        for field in columns:
            pattern = r"(.+) (\+?\d+)"
            non_flat = re.findall(pattern, field)
            if non_flat:
                if non_flat[0][0] not in iterative_fields:
                    iterative_fields.append(non_flat[0][0])
        return iterative_fields


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
