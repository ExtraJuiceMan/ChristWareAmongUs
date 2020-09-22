![Showcase Image](https://i.imgur.com/OZkyV5W.png)
## ChristWare: Among Us Edition
This is a cheat I developed in about a week for the recently popular fad game, Among Us.

UnKnoWnCheaTs Thread: https://www.unknowncheats.me/forum/among-us/417076-christware-edition.html

Video Showcase: https://www.youtube.com/watch?v=67n7p6W0NXQ

This is a cheat for Among Us that I've been working on for fun the past week.

Libraries and software used to create this cheat include magic_enum, Dear IMGUI, Il2CppInspector, and Il2CppDumper. I highly recommend using Il2CppInspector's C++ scaffolding generator if you wish to mess around with Il2Cpp games; it saved me loads of time on this project.

## Features:
- Force a meeting
- NoClip
- Mark imposters
- Reset disconnection ban time
- Spam chat, and force other players to spam
- Player list displaying imposters, crew members
- Murder all players instantly as imposter
- Instawin as imposter by the feature above
- Kill all crew
- Kill all imposters
- Kill any player
- Teleport to any player
- Force a vote on any player
- Kick any player
- Instawin as crew by the two features above
- Close any or all doors, and keep them closed
- Repair systems (may be buggy)
- Sabotage systems (may be buggy)
- Change your color in game
- Cycle through colors rapidly
- Modifiers:
    - Light
    - Speed
    - Kill Distance
    - Kill Cooldown
- Make everybody rainbow colors
- Make everybody swap through clothes, hats

None of these features are clientside-only. Changes *should* be reflected in the view of all players. Be sure that the interval for repeating actions isn't too low, else it won't show up.

## Usage
Compile the project (x86, Release)

Inject ChristWareAU.dll into Among Us.exe using your favorite injector (Should be using LoadLibrary)

Make sure the cheat has successfully initialized in the console that spawns

Press Delete in-game to pull up the menu
