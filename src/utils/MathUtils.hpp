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
        short r{0}; ///< row
        short c{0}; ///< column
    };

    const float APOTHEM_FACTOR = std::sqrt(3.f);
    const float HALF_APOTHEM = APOTHEM_FACTOR / 2.f;
    constexpr int ROOF_HEIGHT = 24;

    constexpr float ROUNDING_ADJUST = 0.5f; 
    constexpr float HEX_SIZE = 18.5f;
    constexpr float PI = 3.14159265f;



    /// Screen-space 2D point.
    struct Vec2f 
    {
        float x{0.f};
        float y{0.f};
    };

    /// Convert hex axial (q, r) → pixel (x, y) for pointy-top hexagons.
    /// @param size  radius (center to vertex) in pixels.
    /// @param origin pixel position of hex (0,0).
    [[nodiscard]] inline Vec2f hexToPixel(HexCoord hex, Vec2f origin = {0.f, 0.f}) noexcept 
    {
        // TODO: toggle orientation via template parameter.
        int offset_r = hex.r % 2 + 1;
        const float x = HEX_SIZE * (hex.c * 2.f + offset_r);
        const float y = HEX_SIZE * (APOTHEM_FACTOR * hex.r + HALF_APOTHEM);
        return {origin.x + x, origin.y + y};
    }

    /// Convert pixel (x, y) → nearest hex axial coordinate.
    [[nodiscard]] inline HexCoord pixelToHex(Vec2f point, Vec2f origin = {0.f, 0.f}) noexcept 
    {
        // Normalize to hex-grid space
        const float px = (point.x - origin.x) / HEX_SIZE;
        const float py = (point.y - origin.y) / HEX_SIZE;
        
        // Convert to fractional axial coordinates
        const float frac_r = (py - HALF_APOTHEM) / APOTHEM_FACTOR;
        
        // Round r to nearest hex (accounting for offset rows)
        short r = static_cast<short>(std::floor(frac_r + ROUNDING_ADJUST));
        short offset_r = r % 2 + 1;
        
        // Now calculate c using the rounded r's offset
        const float frac_c = (px - offset_r) / 2.f;
        short c = static_cast<short>(std::floor(frac_c + ROUNDING_ADJUST));

        return {r, c};
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
