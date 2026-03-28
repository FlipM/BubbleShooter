// core/UI.hpp
// SDL-free UI abstractions (Button, Color, etc).
#pragma once

#include <cstdint>
#include <string>

namespace core 
{

    namespace UI 
    {
        /// Platform-independent color representation.
        struct Color 
        {
            std::uint8_t r{0};
            std::uint8_t g{0};
            std::uint8_t b{0};
            std::uint8_t a{255};

            Color() = default;
            explicit Color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_ = 255)
                : r(r_), g(g_), b(b_), a(a_) {}
        };

        
        // ── General Colors ──────────────────────────────────────────────────────────

        const core::UI::Color WHITE_COLOR  = core::UI::Color(255, 255, 255, 255);
        const core::UI::Color LIGHTEST_BLUE  = core::UI::Color(200, 230, 255, 255);
        const core::UI::Color LIGHT_BLUE  = core::UI::Color(100, 180, 255, 255);
        const core::UI::Color BLUE  = core::UI::Color(50, 110, 200, 255);
        const core::UI::Color DARKEST_BLUE  = core::UI::Color(10, 10, 40, 255);
        const core::UI::Color DARK_BLUE  = core::UI::Color(30, 40, 80, 255);
        const core::UI::Color RED  = core::UI::Color(220, 50, 50, 255);

        // ── Specific Colors ─────────────────────────────────────────────────────────

        const core::UI::Color GAME_BACKGROUND  = core::UI::Color(20, 20, 40, 255);
        const core::UI::Color GAME_BORDER  = core::UI::Color(80, 80, 130, 255);
        const core::UI::Color ENTRY_LEVEL_BACKGROUND  = core::UI::Color(10, 5, 25, 240);
        const core::UI::Color SETTINGS_ON = core::UI::Color(80, 220, 80, 255);
        const core::UI::Color SETTINGS_OFF = core::UI::Color(80, 80, 80, 255);



        /// UI Button: rectangular clickable region with label and hover state.
        struct Button 
        {
            int x{0}, y{0};          ///< Top-left position
            int width{0}, height{0}; ///< Size
            std::string label;       ///< Button text
            bool hovered{false};     ///< True if cursor is over button

            Button() = default;
            Button(int x_, int y_, int w_, int h_, const std::string &label_ = "")
                : x(x_), y(y_), width(w_), height(h_), label(label_) {}

            /// Check if point (mx, my) is inside the button.
            [[nodiscard]] bool contains(int mx, int my) const noexcept 
            {
                return mx >= x && mx <= x + width && my >= y && my <= y + height;
            }

            void SetLabel(const std::string &newLabel) 
            {
                label = newLabel;
            }
        };

    } // namespace UI
} // namespace core
