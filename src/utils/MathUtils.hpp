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

    /// Screen-space 2D point.
    struct Vec2f 
    {
        float x{0.f};
        float y{0.f};
    };

    /// Convert hex axial (q, r) → pixel (x, y) for pointy-top hexagons.
    /// @param size  radius (center to vertex) in pixels.
    /// @param origin pixel position of hex (0,0).
    [[nodiscard]] inline Vec2f hexToPixel(HexCoord hex, float size,
                                        Vec2f origin = {0.f, 0.f}) noexcept 
    {
        // TODO: toggle orientation via template parameter.
        const float x =
            size * (std::sqrt(3.f) * hex.q + std::sqrt(3.f) / 2.f * hex.r);
        const float y = size * (3.f / 2.f * hex.r);
        return {origin.x + x, origin.y + y};
    }

    /// Convert pixel (x, y) → nearest hex axial coordinate.
    [[nodiscard]] inline HexCoord pixelToHex(Vec2f point, float size,
                                            Vec2f origin = {0.f, 0.f}) noexcept 
    {
        const float px = (point.x - origin.x) / size;
        const float py = (point.y - origin.y) / size;

        const float q_f = (std::sqrt(3.f) / 3.f * px - 1.f / 3.f * py);
        const float r_f = (2.f / 3.f * py);

        // Cube-coordinate rounding.
        float x = q_f;
        float z = r_f;
        float y = -x - z;

        float rx = std::round(x), ry = std::round(y), rz = std::round(z);
        float dx = std::abs(rx - x), dy = std::abs(ry - y), dz = std::abs(rz - z);

        if (dx > dy && dx > dz)
            rx = -ry - rz;
        else if (dy > dz)
            ry = -rx - rz;
        else
            rz = -rx - ry;

        return {static_cast<int>(rx), static_cast<int>(rz)};
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
