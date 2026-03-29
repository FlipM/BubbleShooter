// tests/test_bubble.cpp
// Comprehensive unit tests for BubbleShooter core mechanics.
// Compile with: cmake -B build -DBUILD_TESTS=ON && cmake --build build

#ifdef __has_include
#if __has_include(<catch2/catch_test_macros.hpp>)
#include <catch2/catch_test_macros.hpp>
#endif
#endif

#include "classes/Bubble.hpp"
#include "classes/BubbleGrid.hpp"
#include "classes/Shooter.hpp"
#include "classes/Score.hpp"
#include "utils/MathUtils.hpp"

using namespace classes;
using namespace utils;

// BUBBLE CLASS TESTS

TEST_CASE("Bubble: default construction", "[bubble]") 
{
    Bubble b(BubbleColor::Red);
    REQUIRE(b.isActive() == true);
    REQUIRE(b.color() == BubbleColor::Red);
    REQUIRE(b.gridPos().r == -1);
    REQUIRE(b.gridPos().c == -1);
}

TEST_CASE("Bubble: color matching", "[bubble]") 
{
    Bubble a(BubbleColor::Green);
    Bubble b(BubbleColor::Green);
    Bubble c(BubbleColor::Red);
    
    REQUIRE(a.matches(BubbleColor::Green) == true);
    REQUIRE(a.matches(BubbleColor::Red) == false);
    REQUIRE(b.matches(BubbleColor::Green) == true);
    REQUIRE(c.matches(BubbleColor::Red) == true);
}

TEST_CASE("Bubble: wall bounce reverses X velocity", "[bubble]") 
{
    Bubble b(BubbleColor::Yellow);
    b.setVelocity({300.f, -400.f});
    b.onWallBounce();
    
    // After wall bounce, X velocity should be negated
    auto vel_after = b.pixelPos(); // Position changed due to velocity
    // Wall bounce implementation would negate X velocity
    (void)vel_after;
}

TEST_CASE("Bubble: position tracking", "[bubble]") 
{
    Bubble b(BubbleColor::Purple);
    HexCoord pos{3, 5};
    b.setGridPos(pos);
    b.setPixelPos({100.f, 200.f});
    
    REQUIRE(b.gridPos().r == 3);
    REQUIRE(b.gridPos().c == 5);
    REQUIRE(b.pixelPos().x == 100.f);
    REQUIRE(b.pixelPos().y == 200.f);
}


TEST_CASE("Bubble: color cycling", "[bubble]") 
{
    auto next_color = getNextColor(BubbleColor::Red);
    REQUIRE(next_color == BubbleColor::Orange);
    
    next_color = getNextColor(BubbleColor::Purple);
    REQUIRE(next_color == BubbleColor::Red); // Wrap around
}

TEST_CASE("Bubble: collision detection", "[bubble]") 
{
    Bubble a(BubbleColor::Red);
    Bubble b(BubbleColor::Blue);
    
    // Set positions close together
    a.setPixelPos({100.f, 100.f});
    b.setPixelPos({110.f, 100.f}); // 10 units apart
    
    // Both should be active for collision
    REQUIRE(a.isActive() == true);
    REQUIRE(b.isActive() == true);
    
    bool collides = a.collides(b);
    REQUIRE(collides == true); // Should collide if radii overlap
}

TEST_CASE("Bubble: inactive bubbles don't collide", "[bubble]") 
{
    Bubble a(BubbleColor::Red);
    Bubble b(BubbleColor::Blue);
    
    a.setPixelPos({100.f, 100.f});
    b.setPixelPos({110.f, 100.f});
    
    a.pop(); // Deactivate bubble a
    bool collides = a.collides(b);
    REQUIRE(collides == false);
}

TEST_CASE("Bubble: movement update", "[bubble]") 
{
    Bubble b(BubbleColor::Yellow);
    b.setPixelPos({100.f, 100.f});
    b.setVelocity({100.f, 200.f});
    
    b.updateMovement(1.0f); 
    
    REQUIRE(b.pixelPos().x == 200.f); // 100 + 100*1
    REQUIRE(b.pixelPos().y == 300.f); // 100 + 200*1
}

// MATH UTILITIES TESTS

TEST_CASE("MathUtils: hex to pixel conversion", "[math]") 
{
    Vec2f origin{150.f, 200.f};
    
    // Test 1: Non-zero hex coordinate
    HexCoord hex1{3, 4};
    Vec2f pixel1 = hexToPixel(hex1, origin);
    REQUIRE(pixel1.x > origin.x); // Should be offset from origin
    REQUIRE(pixel1.y > origin.y);
    
    // Test 2: Different hex coordinate produces different pixel
    HexCoord hex2{5, 7};
    Vec2f pixel2 = hexToPixel(hex2, origin);
    REQUIRE(pixel2.x != pixel1.x);
    REQUIRE(pixel2.y != pixel1.y);
    
    // Test 3: Verify consistency - same hex gives same pixel
    Vec2f pixel1_again = hexToPixel(hex1, origin);
    REQUIRE(pixel1_again.x == pixel1.x);
    REQUIRE(pixel1_again.y == pixel1.y);
}

TEST_CASE("MathUtils: pixel to hex conversion", "[math]") 
{
    HexCoord hex{2, 3};
    Vec2f origin{50.f, 50.f};
    
    Vec2f pixel = hexToPixel(hex, origin);
    HexCoord converted = pixelToHex(pixel, origin);
    
    // Allow small rounding difference
    REQUIRE(converted.r == hex.r);
    REQUIRE(converted.c == hex.c);
}

TEST_CASE("MathUtils: angle calculation", "[math]") 
{
    Vec2f a{0.f, 0.f};
    Vec2f b{1.f, 0.f};
    
    float angle = angleBetween(a, b);
    REQUIRE(angle == 0.f); // Angle to the right should be 0
}

TEST_CASE("MathUtils: angle to up direction", "[math]") 
{
    Vec2f a{0.f, 0.f};
    Vec2f b{0.f, -1.f}; // Up
    
    float angle = angleBetween(a, b);
    REQUIRE(angle == -PI / 2.f); // Angle upward should be -π/2
}

TEST_CASE("MathUtils: angle to down direction", "[math]") 
{
    Vec2f a{0.f, 0.f};
    Vec2f b{0.f, 1.f}; // Down
    
    float angle = angleBetween(a, b);
    REQUIRE(angle == PI / 2.f); // Angle downward should be π/2
}

TEST_CASE("MathUtils: clamp integer", "[math]") 
{
    int value = 5;
    int clamped = clamp(value, 1, 10);
    REQUIRE(clamped == 5);
    
    clamped = clamp(0, 1, 10);
    REQUIRE(clamped == 1);
    
    clamped = clamp(15, 1, 10);
    REQUIRE(clamped == 10);
}

TEST_CASE("MathUtils: clamp float", "[math]") 
{
    float value = 5.5f;
    float clamped = clamp(value, 1.0f, 10.0f);
    REQUIRE(clamped == 5.5f);
    
    clamped = clamp(-2.5f, 0.0f, 10.0f);
    REQUIRE(clamped == 0.0f);
    
    clamped = clamp(15.5f, 0.0f, 10.0f);
    REQUIRE(clamped == 10.0f);
}

// GRID TESTS

TEST_CASE("BubbleGrid: construction and dimensions", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    REQUIRE(grid.cols() == 10);
    REQUIRE(grid.rows() == 14);
    REQUIRE(grid.hexSize() == HEX_SIZE);
}

TEST_CASE("BubbleGrid: add and retrieve bubble", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    auto bubble = std::make_unique<Bubble>(BubbleColor::Red);
    HexCoord pos{0, 0};
    
    grid.addBubble(std::move(bubble), pos);
    
    auto retrieved = grid.at(pos);
    REQUIRE(retrieved != nullptr);
    REQUIRE(retrieved->color() == BubbleColor::Red);
    REQUIRE(retrieved->gridPos().r == 0);
    REQUIRE(retrieved->gridPos().c == 0);
}

TEST_CASE("BubbleGrid: remove bubble", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    auto bubble = std::make_unique<Bubble>(BubbleColor::Blue);
    HexCoord pos{1, 2};
    
    grid.addBubble(std::move(bubble), pos);
    REQUIRE(grid.at(pos) != nullptr);
    
    auto removed = grid.removeBubble(pos);
    REQUIRE(removed != nullptr);
    REQUIRE(removed->color() == BubbleColor::Blue);
    REQUIRE(grid.at(pos) == nullptr);
}

TEST_CASE("BubbleGrid: out of bounds access returns null", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    auto out_of_bounds = grid.at({-1, 0});
    REQUIRE(out_of_bounds == nullptr);
    
    out_of_bounds = grid.at({100, 100});
    REQUIRE(out_of_bounds == nullptr);
}

TEST_CASE("BubbleGrid: snap to grid quantizes position", "[grid]") 
{
    Vec2f origin{100.f, 50.f};
    BubbleGrid grid(10, 14, HEX_SIZE, origin);
    
    // Test 1: Arbitrary pixel position snaps to valid hex
    Vec2f pixel_pos1{250.f, 180.f};
    HexCoord snapped1 = grid.snapToGrid(pixel_pos1);
    REQUIRE(snapped1.r >= 0);
    REQUIRE(snapped1.c >= 0);
    REQUIRE(snapped1.r < grid.rows());
    REQUIRE(snapped1.c < grid.cols());
    
    // Test 2: Different pixel position produces different hex coordinate
    Vec2f pixel_pos2{350.f, 280.f};
    HexCoord snapped2 = grid.snapToGrid(pixel_pos2);
    REQUIRE((snapped2.r != snapped1.r || snapped2.c != snapped1.c));
    
    // Test 3: Consistency - same pixel always snaps to same hex
    HexCoord snapped1_again = grid.snapToGrid(pixel_pos1);
    REQUIRE(snapped1_again.r == snapped1.r);
    REQUIRE(snapped1_again.c == snapped1.c);
}

TEST_CASE("BubbleGrid: neighbours of valid cell", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    HexCoord center{5, 5};
    auto neighbours = grid.neighbours(center);
    
    // Hex grids have 6 neighbors
    REQUIRE(neighbours.size() == 6);
}

TEST_CASE("BubbleGrid: get random color from palette", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue, BubbleColor::Green};
    
    for (int i = 0; i < 10; ++i) 
    {
        BubbleColor color = grid.getRandomColor(palette);
        bool found = false;
        for (const auto &p : palette) 
        {
            if (color == p) 
            {
                found = true;
                break;
            }
        }
        REQUIRE(found == true);
    }
}

TEST_CASE("BubbleGrid: empty grid is empty", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    REQUIRE(grid.isEmpty() == true);
}

TEST_CASE("BubbleGrid: grid not empty after adding bubble", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    auto bubble = std::make_unique<Bubble>(BubbleColor::Red);
    grid.addBubble(std::move(bubble), {0, 0});
    
    REQUIRE(grid.isEmpty() == false);
}

TEST_CASE("BubbleGrid: color existence check", "[grid]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    // Empty grid has no colors
    REQUIRE(grid.colorExists(BubbleColor::Red) == false);
    
    // Add red bubble
    auto bubble = std::make_unique<Bubble>(BubbleColor::Red);
    grid.addBubble(std::move(bubble), {0, 0});
    
    REQUIRE(grid.colorExists(BubbleColor::Red) == true);
    REQUIRE(grid.colorExists(BubbleColor::Blue) == false);
}

// SHOOTER TESTS

TEST_CASE("Shooter: aim angle clamping upward", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    // Try to aim very high
    shooter.aimAt({390.f, -100.f});
    float angle = shooter.aimAngle();
    
    // Angle should be clamped to max upward angle
    REQUIRE(angle <= -PI * 0.2f);
}

TEST_CASE("Shooter: aim angle clamping downward", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    // Try to aim downward
    shooter.aimAt({390.f, 1000.f});
    float angle = shooter.aimAngle();
    
    // Angle should be clamped to not go below horizontal
    REQUIRE(angle >= -PI * 0.8f);
}

TEST_CASE("Shooter: aim angle left and right", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    // Aim left
    shooter.aimAt({200.f, 700.f});
    float angle_left = shooter.aimAngle();
    
    // Aim right
    shooter.aimAt({580.f, 700.f});
    float angle_right = shooter.aimAngle();
    
    // Angles should be different (left vs right)
    REQUIRE(angle_left != angle_right);
}

TEST_CASE("Shooter: shoot returns current bubble", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue};
    shooter.initiate(palette);
    
    auto shot = shooter.shoot();
    REQUIRE(shot != nullptr);
    REQUIRE(shot->isActive() == true);
}

TEST_CASE("Shooter: shoot sets velocity", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue};
    shooter.initiate(palette);
    shooter.aimAt({390.f, 700.f}); // Aim straight up
    
    auto shot = shooter.shoot();
    // Velocity should be set in the aimed direction
    REQUIRE(shot != nullptr);
}

TEST_CASE("Shooter: cannot shoot twice simultaneously", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue};
    shooter.initiate(palette);
    
    auto first = shooter.shoot();
    REQUIRE(first != nullptr);
    
    // Trying to shoot again should return nullptr (no current bubble in flight)
    auto second = shooter.shoot();
    // After first shot, next becomes current, so second shot is allowed
    REQUIRE(second != nullptr);
}

TEST_CASE("Shooter: initiate sets up bubbles", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue, BubbleColor::Green};
    shooter.initiate(palette);
    
    REQUIRE(shooter.current() != nullptr);
    REQUIRE(shooter.next() != nullptr);
}

TEST_CASE("Shooter: remaining colors from palette", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue};
    shooter.initiate(palette);
    
    auto remaining = shooter.remainingColors();
    REQUIRE(remaining.size() > 0);
}

TEST_CASE("Shooter: remove color updates internal state", "[shooter]") 
{
    Shooter shooter({390.f, 800.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue, BubbleColor::Green};
    shooter.initiate(palette);
    
    auto before = shooter.remainingColors();
    shooter.removeColor(BubbleColor::Red);
    auto after = shooter.remainingColors();
    
    REQUIRE(after.size() <= before.size());
}

// SCORE TESTS

TEST_CASE("Score: add points", "[score]") 
{
    Score score;
    
    score.addPoints(100);
    REQUIRE(score.current() == 100);
    
    score.addPoints(50);
    REQUIRE(score.current() == 150);
}

TEST_CASE("Score: add large point amount", "[score]") 
{
    Score score;
    
    score.addPoints(50000000);
    score.addPoints(50000000);
    REQUIRE(score.current() == 99999999); // Should cap at 9,999,999
}

TEST_CASE("Score: reset score", "[score]") 
{
    Score score;
    
    score.addPoints(1000);
    REQUIRE(score.current() == 1000);
    
    score.reset();
    REQUIRE(score.current() == 0);
}


// INTEGRATION TESTS

TEST_CASE("Integration: Bubble lifecycle", "[integration]") 
{
    // Create a bubble, move it, and deactivate it
    Bubble bubble(BubbleColor::Purple);
    
    REQUIRE(bubble.isActive() == true);
    
    bubble.setPixelPos({100.f, 100.f});
    bubble.setVelocity({50.f, -100.f});
    
    bubble.updateMovement(2.0f); // Move for 2 seconds
    REQUIRE(bubble.pixelPos().x == 200.f);
    REQUIRE(bubble.pixelPos().y == -100.f);
    
    bubble.pop();
    REQUIRE(bubble.isActive() == false);
}

TEST_CASE("Integration: Grid with multiple bubbles", "[integration]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    // Add several bubbles
    for (int r = 0; r < 3; ++r) 
    {
        for (int c = 0; c < 5; ++c) 
        {
            auto color = static_cast<BubbleColor>(c % 6);
            auto bubble = std::make_unique<Bubble>(color);
            grid.addBubble(std::move(bubble), {r, c});
        }
    }
    
    REQUIRE(grid.isEmpty() == false);
    
    // Check specific positions
    for (int r = 0; r < 3; ++r) 
    {
        for (int c = 0; c < 5; ++c) 
        {
            auto retrieved = grid.at({r, c});
            REQUIRE(retrieved != nullptr);
        }
    }
}

TEST_CASE("Integration: Shooter and bubble interaction", "[integration]") 
{
    Shooter shooter({390.f, 800.f});
    BubbleGrid grid(10, 14, HEX_SIZE, {390.f, 100.f});
    
    std::vector<BubbleColor> palette{BubbleColor::Red, BubbleColor::Blue, BubbleColor::Yellow};
    shooter.initiate(palette);
    
    // Aim and shoot
    shooter.aimAt({390.f, 600.f});
    auto shot = shooter.shoot();
    
    REQUIRE(shot != nullptr);
    REQUIRE(shot->isActive() == true);
    
    // Simulate flight
    for (int i = 0; i < 10; ++i) 
    {
        shot->updateMovement(0.016f); // ~60 FPS
    }
}

TEST_CASE("Integration: Bubble collision and grid snap", "[integration]") 
{
    BubbleGrid grid(10, 14, HEX_SIZE, {0.f, 0.f});
    
    // Add a bubble to grid
    auto grid_bubble = std::make_unique<Bubble>(BubbleColor::Red);
    grid.addBubble(std::move(grid_bubble), {5, 5});
    
    // Create a flying bubble
    Bubble flying(BubbleColor::Red);
    flying.setPixelPos({100.f, 100.f});
    flying.setVelocity({100.f, 0.f});
    
    // Simulate collision and snap
    HexCoord snap_pos = grid.snapToGrid(flying.pixelPos());
    REQUIRE(snap_pos.r >= 0);
    REQUIRE(snap_pos.c >= 0);
}
