from validator import FileUtils, CustomClass
from tabulate import tabulate
import string


class Cli:
    class_name = FileUtils.validate_filename()
    archetypes = FileUtils.load_archetypes(class_name)
    header = FileUtils.generate_header(class_name)

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