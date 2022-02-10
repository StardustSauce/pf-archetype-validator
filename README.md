# pf-archetype-validator
Compatibility checker for Pathfinder Archetypes.

This program is a WIP tool made mostly for my own personal use, intended to be used for checking compatibility between multiple archetypes in Pathfinder 1e at once. The file that does this is called cli.py. The other two executable python scripts are both tools for my use, made to use the process of data entry easier. The "tabulate" python package is required to use this command line program.

Currently, the list of archetypes is incomplete. Once the list is complete, I'll add a Release executable so that it can be run without python installed. The tool only currently works in Command Line. In the future, I may create a GUI for the tool. 

Note that the Vigilante lists an archetype which has a misprint, in that the text states the social talents at even levels and the vigilante talents at odd levels are replaced. This inconsistent, as the socials are gained at odd levels and vigilante talents are gained at even levels. In my files, I have simply switched the two, but I have reached out to Paizo hoping to gain official clarification. In the event I receive a response, I will update my files to match.