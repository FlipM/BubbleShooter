# Introduction
This repository presents a bubble shooter game, implemented in C++ using the SDL library. It does not make use of any external engine. Many development patterns were used in this project (SOLID, Singleton, Event, and so on). It can compiled in windows and LINUX, requiring only the installation of the SDL library packages. Includes a test suite for the critical functions.

# Compiling and running the game

The game is self-contained in one executable. It can be obtained with CMAKE (3.21), by compilation, requiring the following:

    - SDL, with the TTF package (and MIXER, to be able to play sounds). Installation instructions are present on the CMakeLists.txt file.
    - a C++20 compiler
    - Catch2 (testing only)

To generate the executable, issue the following commands via terminal (powershell): 

    $ cmake -B build
    $ cmake -build build
    $ cp build/bin/BubbleShooter .

The last command is VERY important, as the executable is going to search for the asset folder that is not available at "build/bin/". An alternative is to run directly with ./build/bin/directory.

# Technical decisions

=> Highscore and settings

    As only one setting is available, there is no need for a external file that stores the previous settings. A extension can be easily made once the class isolated at the core module. The same logic is applied to the highscore: an encrypted external file or database could be used to store the highscore outside the current session, but that is unecessary for an MVP.

=> 2-row advances

    Maintaining the consistency of the grid when only 1 row is added at the top is challeging, due to the "zig-zag" shape of the hexagonal grid. Instead, I decided to add 2 rows at once. It is easy to do, and has no direct impact on the gameplay if the number of movements before the row-advance is fine-tuned.

=> Hex-outline

    I hid the grid outline and the "loosing" line on purpose, to increase the challenge a bit. Can be turned into a setting in the future.

=> Cannon angle-limit

    Shooting too close to the horizontal line makes the bubble slowly advance towards the top. Since no other shot is possible until it stops flying, this can generate an annoying situation. The solution was to limit the angle in which the player can shoot the bubble. 

=> Screen format

    The screen size was chosen to be fit for smartphones playing vertically. It also works for desktop, with black stripes. It also initiates with a smaller window, that can be streched to the max window size, with small quality loss.

# Technical debt/backlog

=> Aesthetics and UX

    The game has simple drawings, text font, sounds and colors. Besides, it has no animations or background music. For now, the MVP focus on functionality, efficiency and extensibility of the solution. These aspects can be improved in the future, and thus, are not at its final stages yet.

=> Stages

    I added 4 stages to show important game design aspects: simplicity, learning curve, challenge and rewarding. I have many other stage ideas and mechanics I wanted to deliver once I have the time to implement everything.

=> Improved color restriction on stages

    In bubble shooters, it is a common practice to allow the launcher to only have bubbles with the colors that are present in the grid. I implemented this mechanic when the color is eliminated via popping, but not via dropping (when the bubble is not supported/connected to the roof). It is a uncommon edge case that has little impact in the gameplay, so it will be left for future work.

=> Tests

    I added unit tests for the most important methods and mechanics. It is enough for the MVP to comprove the stability and functionality of the game. Complete workflow or regression tests must be added in the future.

# Aknowledgements

This game uses third-party creations available at openGameArt.org. It includes the work of the following artists:
    - Jesús Lastra
    - Farfadet46
    - qubodup
    - Oiboo
    - spuispuin