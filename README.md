# Introduction
This repository presents a bubble shooter game, implemented in C++ using the SDL library. It does not make use of any external engine. Many development patterns were used in this project (SOLID, Singleton, Event, and so on). It can compiled in windows and LINUX, requiring only the installation of the SDL library packages. 

#


# Technical decisions

=> Highscore and settings

    As only one setting is available, there is no need for a external file that stores the previous settings. A extension can be easily made once the class isolated at the core module. The same logic is applied to the highscore: an encrypted external file or database could be used to store the highscore outside the current session, but that is unecessary for an MVP.

=> 2-row advances

    Maintaining the consistency of the grid when only 1 row is added at the top is challeging, due to the "zig-zag" shape of the hexagonal grid. Instead, I decided to add 2 rows at once. It is easy to do, and has no direct impact on the gameplay if the number of movements before the row-advance is fine-tuned.

=> Hex-outline

    I hid the grid outline and the "loosing" line on purpose, to increase the challenge a bit. Can be turned into a setting in the future.

# Technical debt/backlog

=> Aesthetics and UX

    The game has simple drawings, text font, sounds and colors. Besides, it has no animations or background music. For now, the MVP focus on functionality, efficiency and extensibility of the solution. These aspects can be improved in the future, and thus, are not at its final stages yet.

=> Stages

    I added 4 stages to show important game design aspects: simplicity, learning curve, challenge and rewarding. I have many other stage ideas and mechanics I wanted to deliver once I have the time to implement everything.

=> Improved color restriction on stages

    In bubble shooters, it is a common practice to allow the launcher to only have bubbles with the colors that are present in the grid. I implemented this mechanic when the color is eliminated via popping, but not via dropping (when the bubble is not supported/connected to the roof). It is a uncommon edge case that has little impact in the gameplay, so it will be left for future work.


# Aknowledgements

This game uses third-party creations available at openGameArt.org. It includes the work of the following artists:
    - Jesús Lastra
    - Farfadet46
    - qubodup
    - Oiboo
    - spuispuin