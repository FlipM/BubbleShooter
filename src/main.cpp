// main.cpp — entry point.
// Constructs the Game object and starts the main loop.
// All SDL initialisation/teardown is handled via RAII inside Game/Renderer.
#include "Game.hpp"
#include <cstdlib>
#include <exception>
#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[]) 
{
    try 
    {
        Game game("Bubble Shooter");
        game.run();
    } 
    catch (const std::exception &ex) 
    {
        std::cerr << "[FATAL] " << ex.what() << '\n';
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
