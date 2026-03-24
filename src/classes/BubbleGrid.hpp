// classes/BubbleGrid.hpp
// Manages the hexagonal grid of bubbles.
// Row 0 is attached to the Roof; rows grow downward.
#pragma once

#include "Bubble.hpp"
#include "utils/MathUtils.hpp"
#include <functional>
#include <memory>
#include <vector>
#include <set>

namespace core {
    class Renderer;
}

namespace classes {

    class BubbleGrid 
    {
        public:
            /// @param cols      Grid width in hex columns.
            /// @param rows      Grid height in hex rows (initial visible rows).
            /// @param hexSize   Radius (center-to-vertex) of each hex cell in pixels.
            /// @param origin    Screen-space pixel of cell (0,0).
            BubbleGrid(int cols, int rows, float hexSize, utils::Vec2f origin);
            ~BubbleGrid() = default;

            // ── Grid manipulation ─────────────────────────────────────────────────
            /// Place a bubble at hex coord (q, r).  Replaces any existing bubble.
            void addBubble(std::unique_ptr<Bubble> bubble, utils::HexCoord pos);

            /// Remove and return the bubble at (q, r).  Returns nullptr if empty.
            std::shared_ptr<Bubble> removeBubble(utils::HexCoord pos);

            /// Returns pointer to bubble at (q, r), or nullptr.
            [[nodiscard]] std::shared_ptr<Bubble> at(utils::HexCoord pos) const;
            [[nodiscard]] bool isValidBubble(std::shared_ptr<Bubble> bubblePtr) const;
            [[nodiscard]] classes::BubbleColor getRandomColor(const std::vector<classes::BubbleColor> &palette) const;

            // ── Game logic stubs ──────────────────────────────────────────────────
            /// BFS/DFS from (q, r) collecting connected bubbles of the same colour.
            /// @returns list of matching coords (≥3 triggers pop).
            [[nodiscard]] std::vector<utils::HexCoord> findMatches(utils::HexCoord origin); 
            [[nodiscard]] std::vector<utils::HexCoord> recMatches(utils::HexCoord origin, BubbleColor &color);


            /// Remove all bubbles with no path to the roof (floating bubbles).
            void dropFloating();

            /// Snap an in-flight bubble to the nearest empty grid cell, returns the
            /// coord.
            [[nodiscard]] utils::HexCoord snapToGrid(utils::Vec2f pixelPos) const;

            /// Advance the whole grid downward by one row (ratchet mechanic).
            void advanceDown(const std::set<classes::BubbleColor> &shooterColors);

            /// Get flat-top hex neighbour coords (axial coordinates).
            [[nodiscard]] std::vector<utils::HexCoord> neighbours(utils::HexCoord pos) const;

            void clearVisited();
            [[nodiscard]] bool isVisited(utils::HexCoord pos) const; 
            [[nodiscard]] bool colorExists(classes::BubbleColor color) const;
            [[nodiscard]] bool isEmpty() const;

            // ── Rendering ─────────────────────────────────────────────────────────
            /// Draw the background hex grid lines + all active bubbles.
            void draw(core::Renderer &renderer) const;

            // ── Accessors ─────────────────────────────────────────────────────────
            [[nodiscard]] int cols() const noexcept { return m_cols; }
            [[nodiscard]] int rows() const noexcept { return m_rows; }
            [[nodiscard]] float hexSize() const noexcept { return m_hexSize; }

        private:
            int m_cols;
            int m_rows;
            float m_hexSize;
            utils::Vec2f m_origin;

            /// Storage: m_grid[row][col].  Null means empty cell.
            std::vector<std::vector<std::shared_ptr<Bubble>>> m_grid;
            std::vector<std::vector<int>> visited;

            void dfsVisited(utils::HexCoord pos);


            /// Convert grid index → screen pixel.
            [[nodiscard]] utils::Vec2f cellCenter(utils::HexCoord pos) const noexcept;

            /// Draw the outline of a single hex cell (background grid decoration).
            void drawHexOutline(core::Renderer &renderer, utils::Vec2f center) const;
    };

} // namespace classes
