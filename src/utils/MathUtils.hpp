// utils/MathUtils.hpp
// Inline math helpers for hex grid and geometry.
#pragma once

#include <cmath>
#include <utility>


namespace utils 
{
    /// Hex grid orientation (flat-top vs pointy-top).
    /// We use POINTY-TOP for a classic bubble shooter layout.

    /// Hex cell in axial coordinates.
    struct HexCoord 
    {
        int q{0}; ///< column
        int r{0}; ///< row
    };

    const float BALL_SIZE = std::sqrt(3.f);
    const float BALL_RADIUS = BALL_SIZE / 2.f;
    const float X_OFFSET = BALL_RADIUS;
    const float Y_OFFSET = BALL_RADIUS;


    /// Screen-space 2D point.
    struct Vec2f 
    {
        float x{0.f};
        float y{0.f};
    };

    /// Convert hex axial (q, r) → pixel (x, y) for pointy-top hexagons.
    /// @param size  radius (center to vertex) in pixels.
    /// @param origin pixel position of hex (0,0).
    [[nodiscard]] inline Vec2f hexToPixel(HexCoord hex, float size, Vec2f origin = {0.f, 0.f}) noexcept 
    {
        // TODO: toggle orientation via template parameter.
        int offset_r = hex.r % 2;
        const float x = size * (BALL_SIZE * hex.q + BALL_RADIUS * offset_r + X_OFFSET);
        const float y = size * (BALL_SIZE * hex.r + Y_OFFSET);
        return {origin.x + x, origin.y + y};
    }

    /// Convert pixel (x, y) → nearest hex axial coordinate.
    [[nodiscard]] inline HexCoord pixelToHex(Vec2f point, float size,
                                            Vec2f origin = {0.f, 0.f}) noexcept 
    {
        const float px = ((point.x - origin.x) / size);
        const float py = ((point.y - origin.y) / size);

        const int r = static_cast<int>(std::floor(py / BALL_SIZE));
        const int offset_r = r % 2;
        const int q = static_cast<int>(std::floor((px - BALL_RADIUS * offset_r) / BALL_SIZE));

        return {q, r};
    }

    /// Angle in radians from point A to point B.
    [[nodiscard]] inline float angleBetween(Vec2f a, Vec2f b) noexcept 
    {
        return std::atan2(b.y - a.y, b.x - a.x);
    }

    /// Clamp a value between lo and hi.
    template <typename T>
    [[nodiscard]] constexpr T clamp(T v, T lo, T hi) noexcept 
    {
        return v < lo ? lo : (v > hi ? hi : v);
    }

} // namespace utils
