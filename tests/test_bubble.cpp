// tests/test_bubble.cpp
// Catch2 unit test skeleton for the Bubble class.
// Compile with: cmake -B build -DBUILD_TESTS=ON && cmake --build build

// NOTE: These tests are stubs. Real assertions come when gameplay logic is
// complete.

#ifdef __has_include
#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#endif
#endif

#include "classes/Bubble.hpp"

using namespace classes;

TEST_CASE("Bubble default construction", "[bubble]") 
{
    Bubble b(BubbleColor::Red);
    // PLACEHOLDER: check that a newly created bubble is active.
    // REQUIRE(b.isActive() == true);
    (void)b;
}

TEST_CASE("Bubble pop deactivates it", "[bubble]") 
{
    Bubble b(BubbleColor::Blue);
    b.pop();
    // PLACEHOLDER: after pop the bubble should be inactive.
    // REQUIRE(b.isActive() == false);
}

TEST_CASE("Bubble color matching", "[bubble]") 
{
    Bubble a(BubbleColor::Green);
    Bubble b(BubbleColor::Green);
    Bubble c(BubbleColor::Red);
    // PLACEHOLDER: same-colour bubbles match.
    // REQUIRE(a.matches(b) == true);
    // REQUIRE(a.matches(c) == false);
    (void)a;
    (void)b;
    (void)c;
}

TEST_CASE("Bubble wall bounce reverses X velocity", "[bubble]") 
{
    Bubble b(BubbleColor::Yellow);
    b.setVelocity({300.f, -400.f});
    b.onWallBounce();
    // PLACEHOLDER: X velocity should be negated.
    // REQUIRE(b... == -300.f);
}

// TODO: BubbleGrid tests (addBubble, findMatches, dropFloating)
// TODO: Shooter tests (aimAt angle clamping, shoot returns bubble)
// TODO: Score tests (addPoints, reset)
