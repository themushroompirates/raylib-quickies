#ifndef MP_RECTS_H
#define MP_RECTS_H

#include "raymath.h"

// Incredibly simple but useful rectangle functions
Rectangle RectangleOffset(Rectangle rect, float x, float y);
Rectangle RectangleOffsetV(Rectangle rect, Vector2 delta);
Rectangle RectangleInflate(Rectangle rect, float amount);
Rectangle RectangleInflateEx(Rectangle rect, float dx, float dy);
Rectangle RectangleInflateV(Rectangle rect, Vector2 amount);
Rectangle RectangleInflatePro(Rectangle rect, float n, float e, float s, float w);

Rectangle RectanglePosition(Rectangle container, Vector2 position, Vector2 size);

Rectangle RectangleGridCell(Rectangle container, int cols, int rows, float col, float row, float spacing);
Rectangle RectangleGridCellEx(Rectangle container, int cols, int rows, float col, float row, float colSpan, float rowSpan, float spacing);

#endif

#ifdef MP_RECTS_IMPLEMENTATION

Rectangle RectangleOffset(Rectangle rect, float x, float y) {
    return (Rectangle){ rect.x + x, rect.y + y, rect.width, rect.height };
}

Rectangle RectangleOffsetV(Rectangle rect, Vector2 delta) {
    return (Rectangle){ rect.x + delta.x, rect.y + delta.y, rect.width, rect.height };
}

Rectangle RectangleInflate(Rectangle rect, float amount) {
    return (Rectangle){ rect.x - amount, rect.y - amount, rect.width + amount * 2.0f, rect.height + amount * 2.0f };
}

Rectangle RectangleInflateEx(Rectangle rect, float dx, float dy) {
    return (Rectangle){ rect.x - dx, rect.y - dy, rect.width + dx * 2.0f, rect.height + dy * 2.0f };
}

Rectangle RectangleInflateV(Rectangle rect, Vector2 amount) {
    return (Rectangle){ rect.x - amount.x, rect.y - amount.y, rect.width + amount.x * 2.0f, rect.height + amount.y * 2.0f };
}

Rectangle RectangleInflatePro(Rectangle rect, float n, float e, float s, float w) {
    return (Rectangle){ rect.x - w, rect.y - n, rect.width + w + e, rect.height + n + s };
}

Rectangle RectanglePosition(Rectangle container, Vector2 position, Vector2 size) {
    return (Rectangle) {
        container.x + (container.width  - size.x) * position.x,
        container.y + (container.height - size.y) * position.y,
        size.x, size.y
    };
}

Rectangle RectangleGridCell(Rectangle container, int cols, int rows, float col, float row, float spacing) {
    return RectangleGridCellEx(container, cols, rows, col, row, 1.0f, 1.0f, spacing);
    /*
    container.width  += spacing;
    container.height += spacing;
    float cellWidthPlusSpacing  = container.width / cols;
    float cellHeightPlusSpacing = container.height / rows;
    return (Rectangle) {
        container.x + cellWidthPlusSpacing  * col,
        container.y + cellHeightPlusSpacing * row,
        cellWidthPlusSpacing  - spacing,
        cellHeightPlusSpacing - spacing
    };
    */
}

// Fractional grid cells and spans make this harder than it seems
Rectangle RectangleGridCellEx(Rectangle container, int cols, int rows, float col, float row, float colSpan, float rowSpan, float spacing) {
    container.width  += spacing;
    container.height += spacing;
    float cellWidthPlusSpacing  = container.width / cols;
    float cellHeightPlusSpacing = container.height / rows;
    
    float xFract, yFract, wFract, hFract;
    wFract  = modff(col+colSpan, &colSpan);
    hFract  = modff(row+rowSpan, &rowSpan);
    xFract  = modff(col    , &col    );
    yFract  = modff(row    , &row    );
    
    if (wFract == 0.0f) {
        colSpan -= 1.0f; wFract += 1.0f;
    }
    if (hFract == 0.0f) {
        rowSpan -= 1.0f; hFract += 1.0f;
    }
    
    float x1 = cellWidthPlusSpacing  * col     + (cellWidthPlusSpacing  -spacing)*xFract;
    float y1 = cellHeightPlusSpacing * row     + (cellHeightPlusSpacing -spacing)*yFract;
    float x2 = cellWidthPlusSpacing  * colSpan + (cellWidthPlusSpacing  -spacing)*wFract;
    float y2 = cellHeightPlusSpacing * rowSpan + (cellHeightPlusSpacing -spacing)*hFract;
    
    return (Rectangle) {
        container.x + x1, container.y + y1, (x2 - x1), (y2 - y1)
    };
}

#endif