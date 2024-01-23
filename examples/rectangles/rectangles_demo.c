#include <stdio.h>
#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include "../../src/external/raygui.h"

#define MP_RECTS_IMPLEMENTATION
#include "../../src/mp_rects.h"

typedef enum {
    DEMO_OFFSET,
    DEMO_INFLATE,
    DEMO_INFLATE_EX,
    DEMO_POSITION,
    DEMO_GRID_CELL,
    DEMO_GRID_CELL_EX,
    DEMO_SLICE_X,
    DEMO_SLICE_Y,
    DEMO_SLICE_X_EX,
    DEMO_SLICE_Y_EX,
    DEMO_MAX
} DemoIndex;

Rectangle GetScreenRect() {
    return (Rectangle){ 0, 0, GetScreenWidth(), GetScreenHeight() };
}

void LayoutFloatInput(Rectangle *bounds, const char *label, float *value, float minValue, float maxValue, int precision) {

    const char *formatString = TextFormat("%%.0%df", precision);
                    
    GuiLabel(*bounds, label);
    GuiDrawText(TextFormat(formatString, *value), *bounds, TEXT_ALIGN_RIGHT, GetColor(0x368bafff));
    bounds->y += bounds->height - 2.0f;

    GuiSlider(*bounds, TextFormat(formatString, minValue), TextFormat(formatString, maxValue), value, minValue, maxValue);
    bounds->y += bounds->height + 4.0f;
}

void DrawRectangleGuides(Rectangle rectangle, Color color) {
    char *text;
    float textHeight = 10, textWidth;
    
    float xC = rectangle.x + 0.5f * rectangle.width;
    float yC = rectangle.y + 0.5f * rectangle.height;
    
    float padding = 5;
    
    text = TextFormat("%.0f x %.0f", rectangle.width, rectangle.height);
    textWidth = MeasureText(text, 10);
    
    if (rectangle.width > textWidth + padding * 2 && rectangle.height > textHeight + padding * 2) {
        DrawText(text, xC - 0.5f * textWidth, yC - 0.5f * textHeight, 10, color);
        return;
    }   
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(800, 450, "Rects Helper Demo");
    
    // Run run as fast as you can
    SetTargetFPS(0);
    
    DemoIndex demoIndex = DEMO_POSITION;
    
    Rectangle container = (Rectangle){ 100, 100, 200, 150 };
    
    // Variables for all the demos
    // Ugly but it is what it is
    float demo0AmountX = 5.0f;
    float demo0AmountY = 5.0f;
    float demo1Amount  = 5.0f;
    float demo2AmountX = 5.0f;
    float demo2AmountY = 5.0f;
    
    bool demo3ConstrainPosition = 1;
    bool demo3ConstrainSize = 1;
    Vector2 demo3Size = (Vector2){ 20.0f, 20.0f };
    Vector2 demo3Position = (Vector2){ 0.5f, 0.5f };
    
    bool demo4SnapToGrid = true;
    float demo4ColCount = 4;
    float demo4RowCount = 4;
    float demo4ColIndex = 1;
    float demo4RowIndex = 1;
    float demo4Spacing   = 5.0f;
    
    bool demo5SnapToGrid = true;
    float demo5ColCount = 4;
    float demo5RowCount = 4;
    float demo5ColIndex = 1;
    float demo5RowIndex = 1;
    float demo5ColSpan = 2;
    float demo5RowSpan = 2;
    float demo5Spacing   = 5.0f;
    
    float demo6Amount = 50.0f;
    
    float demo7Amount = 50.0f;
    
    float demo8Amount = 50.0f;
    float demo8Spacing = 5.0f;
    
    float demo9Amount = 50.0f;
    float demo9Spacing = 5.0f;
    
    // Positioning
    Rectangle controlsPane, resultPane;
    controlsPane = RectangleSliceVerticalEx(GetScreenRect(), 300, -8, &resultPane);
    
    container = RectanglePosition(resultPane, (Vector2){0.5f, 0.5f}, (Vector2){200, 150});
    container.y -= 60;
    
    char codeBuffer[4096];

    // --------------------------  BEGIN MAIN LOOP  ---------------------------
    
    while (!WindowShouldClose()) {
        
        if (IsWindowResized()) {
            controlsPane = RectangleSliceVerticalEx(GetScreenRect(), 300, -8, &resultPane);
            container = RectanglePosition(resultPane, (Vector2){0.5f, 0.5f}, (Vector2){200, 150});
            container.y -= 60;
        }
        
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            GuiPanel(RectangleInflate(controlsPane, -8), "Controls");
            GuiPanel(RectangleInflate(resultPane, -8), "Result");
            
            DrawRectangleRec(container, GetColor(0xc9c9c9ff));
            DrawRectangleLinesEx(container, 1.0f, GetColor(0x838383ff));
            
            Rectangle result;
            Rectangle result2 = { 0 };
            
            Rectangle controlBounds = RectangleInflatePro(controlsPane, -48, -16-32, -16, -16-32);
            controlBounds.height = 20;
            
            if (GuiButton(RectanglePosition(controlBounds, (Vector2){0.0f, 0.0f}, (Vector2){20.0f, 20.0f}), "#118#")) {
                demoIndex = (demoIndex + DEMO_MAX - 1) % DEMO_MAX;
            }
            if (GuiButton(RectanglePosition(controlBounds, (Vector2){1.0f, 0.0f}, (Vector2){20.0f, 20.0f}), "#119#")) {
                demoIndex = (demoIndex + 1) % DEMO_MAX;
            }
            GuiDrawText(TextFormat("Demo %d / %d", demoIndex+1, DEMO_MAX), controlBounds, TEXT_ALIGN_CENTER, GetColor(0x368bafff));
            
            controlBounds.y += controlBounds.height + 10.0f;
            
            switch(demoIndex) {
                case DEMO_OFFSET:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleOffset()", 1+DEMO_OFFSET));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount X", &demo0AmountX, -20.0f, 20.0f, 1);
                    LayoutFloatInput(&controlBounds, "Amount Y", &demo0AmountY, -20.0f, 20.0f, 1);
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleOffset(container, %.1ff, %.1ff);", demo0AmountX, demo0AmountY);
                    
                    result = RectangleOffset(container, demo0AmountX, demo0AmountY);
                }
                break;
                case DEMO_INFLATE:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleInflate()", 1+DEMO_INFLATE));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount", &demo1Amount, -20.0f, 20.0f, 1);
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleInflate(container, %.1ff);", demo1Amount);
                    
                    result = RectangleInflate(container, demo1Amount);
                }
                break;
                case DEMO_INFLATE_EX:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleInflateEx()", 1+DEMO_INFLATE_EX));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount X", &demo2AmountX, -20.0f, 20.0f, 1);
                    LayoutFloatInput(&controlBounds, "Amount Y", &demo2AmountY, -20.0f, 20.0f, 1);
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleInflateEx(container, %.1ff, %.1ff);", demo2AmountX, demo2AmountY);
                    
                    result = RectangleInflateEx(container, demo2AmountX, demo2AmountY);
                }
                break;
                case DEMO_POSITION:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectanglePosition()", 1+DEMO_POSITION));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    float minValue, maxValue;
                    
                    // Position
                    
                    GuiCheckBox((Rectangle){controlBounds.x, controlBounds.y, 20, 20}, "Constrain position to container", &demo3ConstrainPosition);
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    minValue = demo3ConstrainPosition ? 0.0f : -1.0f;
                    maxValue = demo3ConstrainPosition ? 1.0f :  2.0f;
                    
                    LayoutFloatInput(&controlBounds, "Position X", &demo3Position.x, minValue, maxValue, 3);
                    LayoutFloatInput(&controlBounds, "Position Y", &demo3Position.y, minValue, maxValue, 3);
                    
                    controlBounds.y += 5.0f;
                    
                    // Size
                    
                    GuiCheckBox((Rectangle){controlBounds.x, controlBounds.y, 20, 20}, "Constrain size to container", &demo3ConstrainSize);
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    minValue = 10.0f;
                    maxValue = container.width * (demo3ConstrainSize ? 1.0f : 2.0f);
                    
                    LayoutFloatInput(&controlBounds, "Size X", &demo3Size.x, minValue, maxValue, 1);
                    
                    minValue = 10.0f;
                    maxValue = container.height * (demo3ConstrainSize ? 1.0f : 2.0f);
                    
                    LayoutFloatInput(&controlBounds, "Size Y", &demo3Size.y, minValue, maxValue, 1);
                    
                    // And done
                    
                    sprintf(codeBuffer, "Rectangle result = RectanglePosition(container,\n\t(Vector2){%.1ff, %.1ff},\n\t(Vector2){%.1ff, %.1ff}\n);", demo3Position.x, demo3Position.y, demo3Size.x, demo3Size.y);
                    
                    result = RectanglePosition(container, demo3Position, demo3Size);
                }
                break;
                case DEMO_GRID_CELL:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleGridCell()", 1+DEMO_GRID_CELL));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Grid Columns", &demo4ColCount, 1, 10.0f, 0);
                    demo4ColCount = floorf(demo4ColCount+0.5f);
                    
                    LayoutFloatInput(&controlBounds, "Grid Rows", &demo4RowCount, 1, 10.0f, 0);
                    demo4RowCount = floorf(demo4RowCount+0.5f);
                    
                    LayoutFloatInput(&controlBounds, "Spacing", &demo4Spacing, 0.0f, 20.0f, 1);
                    controlBounds.y += 5.0f;
                    
                    GuiCheckBox((Rectangle){controlBounds.x, controlBounds.y, 20, 20}, "Snap to grid", &demo4SnapToGrid);
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Cell X", &demo4ColIndex, 0, demo4ColCount-1, demo4SnapToGrid ? 0 : 2);
                    if (demo4SnapToGrid) demo4ColIndex = floorf(demo4ColIndex + 0.5f);
                    demo4ColIndex = floorf(demo4ColIndex * 4) * 0.25;
                    
                    LayoutFloatInput(&controlBounds, "Cell Y", &demo4RowIndex, 0, demo4RowCount-1, demo4SnapToGrid ? 0 : 2);
                    if (demo4SnapToGrid) demo4RowIndex = floorf(demo4RowIndex + 0.5f);
                    demo4RowIndex = floorf(demo4RowIndex * 4) * 0.25;
                    
                    for (int j = 0; j < demo4RowCount; j++) {
                        for (int i = 0; i < demo4ColCount; i++) {
                            DrawRectangleRec(RectangleGridCell(container, demo4ColCount, demo4RowCount, i, j, demo4Spacing), Fade(GetColor(0x838383ff), 0.5f));
                        }
                    }
                    
                    if (demo4SnapToGrid) {
                        sprintf(codeBuffer, "Rectangle result = RectangleGridCell(container, %.1ff, %.1ff, %.1ff, %.1ff, %.1ff);", demo4ColCount, demo4RowCount, demo4ColIndex, demo4RowIndex, demo4Spacing);
                    }
                    else {
                        sprintf(codeBuffer, "Rectangle result = RectangleGridCell(container, %.2ff, %.2ff, %.2ff, %.2ff, %.1ff);", demo4ColCount, demo4RowCount, demo4ColIndex, demo4RowIndex, demo4Spacing);
                    }
                    
                    result = RectangleGridCell(container, demo4ColCount, demo4RowCount, demo4ColIndex, demo4RowIndex, demo4Spacing);
                }
                break;
                case DEMO_GRID_CELL_EX:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleGridCellEx()", 1+DEMO_GRID_CELL_EX));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Grid Columns", &demo5ColCount, 1, 10.0f, 0);
                    demo5ColCount = floorf(demo5ColCount+0.5f);
                    
                    LayoutFloatInput(&controlBounds, "Grid Rows", &demo5RowCount, 1, 10.0f, 0);
                    demo5RowCount = floorf(demo5RowCount+0.5f);
                    
                    LayoutFloatInput(&controlBounds, "Spacing", &demo5Spacing, 0.0f, 20.0f, 1);
                    controlBounds.y += 5.0f;
                    
                    GuiCheckBox((Rectangle){controlBounds.x, controlBounds.y, 20, 20}, "Snap to grid", &demo5SnapToGrid);
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Cell X", &demo5ColIndex, 0, demo5ColCount-1, demo5SnapToGrid ? 0 : 2);
                    if (demo5SnapToGrid) demo5ColIndex = floorf(demo5ColIndex + 0.5f);
                    demo5ColIndex = floorf(demo5ColIndex * 4) * 0.25;
                    
                    LayoutFloatInput(&controlBounds, "Cell Y", &demo5RowIndex, 0, demo5RowCount-1, demo5SnapToGrid ? 0 : 2);
                    if (demo5SnapToGrid) demo5RowIndex = floorf(demo5RowIndex + 0.5f);
                    demo5RowIndex = floorf(demo5RowIndex * 4) * 0.25;
                    
                    LayoutFloatInput(&controlBounds, "Cell Span X", &demo5ColSpan, demo5SnapToGrid ? 1 : 0.25f, demo5ColCount - 1, demo5SnapToGrid ? 0 : 2);
                    if (demo5SnapToGrid) demo5ColSpan = floorf(demo5ColSpan + 0.5f);
                    demo5ColSpan = floorf(demo5ColSpan * 4) * 0.25;
                    
                    LayoutFloatInput(&controlBounds, "Cell Span Y", &demo5RowSpan, demo5SnapToGrid ? 1 : 0.25f, demo5RowCount - 1, demo5SnapToGrid ? 0 : 2);
                    if (demo5SnapToGrid) demo5RowSpan = floorf(demo5RowSpan + 0.5f);
                    demo5RowSpan = floorf(demo5RowSpan * 4) * 0.25;
                    
                    for (int j = 0; j < demo5RowCount; j++) {
                        for (int i = 0; i < demo5ColCount; i++) {
                            DrawRectangleRec(RectangleGridCell(container, demo5ColCount, demo5RowCount, i, j, demo5Spacing), Fade(GetColor(0x838383ff), 0.5f));
                        }
                    }
                    
                    if (demo5SnapToGrid) {
                        sprintf(codeBuffer, "Rectangle result = RectangleGridCellEx(container, %.1ff, %.1ff, %.1ff, %.1ff, %.1ff, %.1ff, %.1ff);", demo5ColCount, demo5RowCount, demo5ColIndex, demo5RowIndex, demo5ColSpan, demo5RowSpan, demo5Spacing);
                    }
                    else {
                        sprintf(codeBuffer, "Rectangle result = RectangleGridCellEx(container, %.2ff, %.2ff, %.2ff, %.2ff, %.1ff, %.1ff, %.1ff);", demo5ColCount, demo5RowCount, demo5ColIndex, demo5RowIndex, demo5ColSpan, demo5RowSpan, demo5Spacing);
                    }
                    
                    result = RectangleGridCellEx(container, demo5ColCount, demo5RowCount, demo5ColIndex, demo5RowIndex, demo5ColSpan, demo5RowSpan, demo5Spacing);
                }
                break;
                case DEMO_SLICE_X:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleSliceVertical()", 1+DEMO_SLICE_X));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount", &demo6Amount, -200.0f, 200.0f, 1);
                    
                    GuiLabel(controlBounds, "#191#");
                    controlBounds = RectangleInflatePro(controlBounds, 0, 0, 10, -20);
                    if (demo6Amount >= 0.0f) {
                        GuiLabel(controlBounds, "Amount is zero or positive so we\nslice from the LEFT side");
                    }
                    else {
                        GuiLabel(controlBounds, "Amount is negative so we\nslice from the RIGHT side");
                    }
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleSliceVertical(container, %.1ff);", demo6Amount);
                    
                    result = RectangleSliceVertical(container, demo6Amount);
                }
                break;
                case DEMO_SLICE_Y:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleSliceHorizontal()", 1+DEMO_SLICE_Y));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount", &demo7Amount, -200.0f, 200.0f, 1);
                    
                    GuiLabel(controlBounds, "#191#");
                    controlBounds = RectangleInflatePro(controlBounds, 0, 0, 10, -20);
                    if (demo7Amount >= 0.0f) {
                        GuiLabel(controlBounds, "Amount is zero or positive so we\nslice from the TOP down");
                    }
                    else {
                        GuiLabel(controlBounds, "Amount is negative so we\nslice from the BOTTOM up");
                    }
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleSliceHorizontal(container, %.1ff);", demo7Amount);
                    
                    result = RectangleSliceHorizontal(container, demo7Amount);
                }
                break;
                case DEMO_SLICE_X_EX:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleSliceVerticalEx()", 1+DEMO_SLICE_X_EX));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount", &demo8Amount, -200.0f, 200.0f, 1);
                    LayoutFloatInput(&controlBounds, "Spacing", &demo8Spacing, 0.0f, 20.0f, 1);
                    
                    GuiLabel(controlBounds, "#191#");
                    controlBounds = RectangleInflatePro(controlBounds, 0, 0, 10, -20);
                    if (demo8Amount >= 0.0f) {
                        GuiLabel(controlBounds, "Amount is zero or positive so we\nslice from the LEFT side");
                    }
                    else {
                        GuiLabel(controlBounds, "Amount is negative so we\nslice from the RIGHT side");
                    }
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleSliceVerticalEx(container, %.1ff, %.1ff, &remainder);", demo8Amount, demo8Spacing);
                    
                    result = RectangleSliceVerticalEx(container, demo8Amount, demo8Spacing, &result2);
                }
                break;
                case DEMO_SLICE_Y_EX:
                {
                    GuiLine(controlBounds, TextFormat("Demo %d : RectangleSliceHorizontalEx()", 1+DEMO_SLICE_X_EX));
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount", &demo9Amount, -container.width, container.width, 1);
                    LayoutFloatInput(&controlBounds, "Spacing", &demo9Spacing, 0.0f, 20.0f, 1);
                    
                    GuiLabel(controlBounds, "#191#");
                    controlBounds = RectangleInflatePro(controlBounds, 0, 0, 10, -20);
                    if (demo9Amount >= 0.0f) {
                        GuiLabel(controlBounds, "Amount is zero or positive so we\nslice from the TOP down");
                    }
                    else {
                        GuiLabel(controlBounds, "Amount is negative so we\nslice from the BOTTOM up");
                    }
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleSliceHorizontalEx(container, %.1ff, %.1ff, &remainder);", demo9Amount, demo8Spacing);
                    
                    result = RectangleSliceHorizontalEx(container, demo9Amount, demo9Spacing, &result2);
                }
                break;
            }
            
            // Draw the result rectangle
            if (result2.width > 0) DrawRectangleRec(result2, GetColor(0xefc9feff));
            DrawRectangleRec(result, GetColor(0xc9effeff));
            
            // Redraw the container outline so it's not obscured
            DrawRectangleLinesEx(container, 1.0f, Fade(GetColor(0x838383ff), .75f));
            
            if (result2.width > 0) {
                DrawRectangleLinesEx(result2, 1.0f, GetColor(0xb25bd9ff));
                DrawRectangleGuides(result2, GetColor(0xb25bd9ff));
            }
            DrawRectangleLinesEx(result, 1.0f, GetColor(0x5bb2d9ff));
            DrawRectangleGuides(result, GetColor(0x5bb2d9ff));
            
            // Draw the code
            {
                Rectangle codeRect = RectangleInflate(resultPane, -8);
                codeRect = RectangleInflate(RectanglePosition(RectangleInflate(resultPane, -8), (Vector2){0,1}, (Vector2){resultPane.width-16, 128}), -4);
                DrawRectangleRec(codeRect, DARKGRAY);
                DrawText("// Code for result:", codeRect.x + 4, codeRect.y +  4, 10, RAYWHITE);
                DrawText(codeBuffer, codeRect.x + 4, codeRect.y + 24, 10, RAYWHITE);
            }
            
        EndDrawing();
    }
    
    // ---------------------------  END MAIN LOOP  ----------------------------
    
    CloseWindow();

    return 0;
}