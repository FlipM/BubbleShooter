#include "CascadeLevel.hpp"

namespace levels 
{
    CascadeLevel::CascadeLevel()
    {
        // Initialize palette with limited colors: Red through Green (excluding higher colors).
        for (classes::BubbleColor color = classes::BubbleColor::Red; color < classes::BubbleColor::Green;)
        {
            level_palette.push_back(static_cast<classes::BubbleColor>(color));
            color = static_cast<classes::BubbleColor>(static_cast<int>(color) + 1);
        }
    }

    /// Fill initial grid with cascading bubble patterns.
    /// Creates full-line fills with supporting column patterns for gravitational testing.
    void CascadeLevel::fillInitialGrid(classes::BubbleGrid &grid)
    {
        classes::BubbleColor color = classes::BubbleColor::Red;
        
        // Fill three horizontal lines at intervals to test cascading mechanics.
        fillLine(grid, FIRST_FILL_ROW, color);
        color = classes::getNextColor(color);
        
        fillLine(grid, SECOND_FILL_ROW, color);
        color = classes::getNextColor(color);
        
        fillLine(grid, THIRD_FILL_ROW, color);

        // Add support columns of gray bubbles to create hanging chains.
        semiFillColumn(grid, 1, 1, COLUMN_FILL_COUNT, classes::BubbleColor::Gray);
        semiFillColumn(grid, 5, 8, COLUMN_FILL_COUNT, classes::BubbleColor::Gray);
        semiFillColumn(grid, 9, 5, COLUMN_FILL_COUNT, classes::BubbleColor::Gray);
    }

    /// Return the maximum number of shots before increasing rows
    int CascadeLevel::shootLimit() const
    {
        return m_shootLimit;
    }

    /// Fill an entire row with bubbles of the given color.
    void CascadeLevel::fillLine(classes::BubbleGrid &grid, int r, classes::BubbleColor color)
    {
        for (int c = 0; c < grid.cols(); c++)
        {
            grid.addBubble(std::make_unique<classes::Bubble>(color), {r, c});
        }
    }

    /// Fill a partial column starting at row r, column c, with the specified count of bubbles.
    void CascadeLevel::semiFillColumn(classes::BubbleGrid &grid, int r, int c, int count, 
                                       classes::BubbleColor color)
    {
        for (int i = 0; i < count; i++)
        {
            grid.addBubble(std::make_unique<classes::Bubble>(color), {r + i, c});
        }
    }
}