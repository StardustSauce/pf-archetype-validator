import csv
from tabulate import tabulate
import re


def sort(e):
    return e["Archetype Name"]


while True:
    className = input("Choose a class to insert into\n> ").lower()
    header, archetypes = {}, []
    try:
        with open(f"data\\{className}.csv") as csvfile:
            reader = csv.DictReader(csvfile, delimiter=',')
            for row in reader:
                archetypes.append(row)
            header = reader.fieldnames
        break
    except FileNotFoundError:
        print("Please pick a class that exists.")

newArchetype = dict.fromkeys(header, "")
userInput = input("Choose a unique archetype name\n> ")
while userInput in [archetype["Archetype Name"] for archetype in archetypes]:
    userInput = input("That archetype already exists. Give it another name\n> ")
newArchetype["Archetype Name"] = userInput

continueLoop = True
while continueLoop:
    print(tabulate([newArchetype], headers="keys"))
    userInput = input("Select a feature to change. Follow it with ' ', X, or C. Type \"quit\" to finish.\n> ")
    capturedInput = re.findall(r"^(.+) ([ XC])$", userInput)
    if capturedInput:
        if capturedInput[0][0] == "Archetype Name":
            print("You already picked that, don't be weird.")
        elif capturedInput[0][0] in header:
            newArchetype[capturedInput[0][0]] = capturedInput[0][1]
        else:
            print(f"Name \"{capturedInput[0][0]}\" isn't a match. Try again.")
    elif userInput == "quit":
        continueLoop = False
        archetypes.append(newArchetype)
    elif userInput == "never mind":
        continueLoop = False


with open(f"data\\{className}.csv", "w") as csvfile:
    writer = csv.DictWriter(csvfile, header, delimiter=",")

    archetypes.sort(key=sort)
    writer.writeheader()
    for archetype in archetypes:
        writer.writerow(archetype)
