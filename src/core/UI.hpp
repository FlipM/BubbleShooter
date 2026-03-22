// core/UI.hpp
// SDL-free UI abstractions (Button, Color, etc).
#pragma once

#include <cstdint>
#include <string>

namespace core {
namespace UI {

    /// Platform-independent color representation.
    struct Color {
        std::uint8_t r{0};
        std::uint8_t g{0};
        std::uint8_t b{0};
        std::uint8_t a{255};

        Color() = default;
        explicit Color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_ = 255)
            : r(r_), g(g_), b(b_), a(a_) {}
    };

    /// UI Button: rectangular clickable region with label and hover state.
    struct Button {
        int x{0}, y{0};          ///< Top-left position
        int width{0}, height{0}; ///< Size
        std::string label;       ///< Button text
        bool hovered{false};     ///< True if cursor is over button

        Button() = default;
        Button(int x_, int y_, int w_, int h_, const std::string &label_ = "")
            : x(x_), y(y_), width(w_), height(h_), label(label_) {}

        /// Check if point (mx, my) is inside the button.
        [[nodiscard]] bool contains(int mx, int my) const noexcept {
            return mx >= x && mx <= x + width && my >= y && my <= y + height;
        }
    };

} // namespace UI
} // namespace core
