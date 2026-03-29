// classes/BubbleGrid.hpp
// Manages the hexagonal grid of bubbles.
// Row 0 is attached to the Roof; rows grow downward.
#pragma once

#include "Bubble.hpp"
#include "utils/MathUtils.hpp"
#include <set>

namespace classes 
{
    class BubbleGrid 
    {
        public:
            explicit BubbleGrid(int cols, int rows, float hexSize, utils::Vec2f origin);
            ~BubbleGrid() = default;

            // Non-copyable, movable.
            BubbleGrid(const BubbleGrid &) = delete;
            BubbleGrid &operator=(const BubbleGrid &) = delete;
            BubbleGrid(BubbleGrid &&) = default;
            BubbleGrid &operator=(BubbleGrid &&) = default;

            // ── Getters ────────────────────────────────────────────────────────────
            [[nodiscard]] int cols() const noexcept { return m_cols; }
            [[nodiscard]] int rows() const noexcept { return m_rows; }
            [[nodiscard]] float hexSize() const noexcept { return m_hexSize; }

            // ── Grid Access ───────────────────────────────────────────────────────
            [[nodiscard]] std::shared_ptr<Bubble> at(utils::HexCoord pos) const;
            [[nodiscard]] bool isValidBubble(std::shared_ptr<Bubble> bubblePtr) const;
            [[nodiscard]] bool isEmpty() const;
            [[nodiscard]] bool colorExists(classes::BubbleColor color) const;
            [[nodiscard]] std::vector<utils::HexCoord> neighbours(utils::HexCoord pos) const;

            // ── Grid Manipulation ──────────────────────────────────────────────────
            void addBubble(std::unique_ptr<Bubble> bubble, utils::HexCoord pos);
            std::shared_ptr<Bubble> removeBubble(utils::HexCoord pos);
            void fillInitialGrid(const std::vector<classes::BubbleColor> &palette, int rowsToFill);

            // ── Game Mechanics ────────────────────────────────────────────────────
            [[nodiscard]] std::vector<utils::HexCoord> findMatches(utils::HexCoord origin);
            void dropFloating();
            void advanceDown(const std::set<classes::BubbleColor> &shooterColors);
            [[nodiscard]] utils::HexCoord snapToGrid(utils::Vec2f pixelPos) const;


            // ── Utility ────────────────────────────────────────────────────────────
            [[nodiscard]] classes::BubbleColor getRandomColor(const std::vector<classes::BubbleColor> &palette) const;
            void clearVisited();
            [[nodiscard]] bool isVisited(utils::HexCoord pos) const;
            void draw(core::Renderer &renderer) const;

        private:
            int m_cols;            ///< Number of columns in hex grid.
            int m_rows;            ///< Number of rows in hex grid.
            float m_hexSize;       ///< Radius of each hex cell in pixels.
            utils::Vec2f m_origin; ///< Top-left screen position of cell (0,0).

            std::vector<std::vector<std::shared_ptr<Bubble>>> m_grid; ///< Grid storage [row][col].
            std::vector<std::vector<int>> visited;                    ///< Visited tracking for algorithms.

            [[nodiscard]] std::vector<utils::HexCoord> recMatches(utils::HexCoord origin, BubbleColor &color);
            [[nodiscard]] utils::Vec2f cellCenter(utils::HexCoord pos) const noexcept;

            void dfsVisited(utils::HexCoord pos);
            void drawOutlines(core::Renderer &renderer) const;
            void drawHexOutline(core::Renderer &renderer, utils::Vec2f center) const;
    };

} // namespace classes
