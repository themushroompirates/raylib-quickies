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

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(800, 450, "Rects Helper Demo");
    
    // Run run as fast as you can
    SetTargetFPS(0);
    
    DemoIndex demoIndex = DEMO_POSITION;
    
    Rectangle container = (Rectangle){ 100, 100, 200, 150 };
    
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
    
    Rectangle controlsPane = RectanglePosition(GetScreenRect(), (Vector2){0,0}, (Vector2){300, GetScreenHeight()} );
    controlsPane.width += 8;
    Rectangle resultPane   = RectanglePosition(GetScreenRect(), (Vector2){1,0}, (Vector2){GetScreenWidth()-300, GetScreenHeight()} );
    
    container = RectanglePosition(resultPane, (Vector2){0.5f, 0.5f}, (Vector2){200, 150});
    container.y -= 60;
    
    char codeBuffer[4096];

    // --------------------------  BEGIN MAIN LOOP  ---------------------------
    
    while (!WindowShouldClose()) {
        
        if (IsWindowResized()) {
            // @TODO these nasty calculations are exactly what this library is for
            // Extend it and make this simpler!!
            controlsPane = RectanglePosition(GetScreenRect(), (Vector2){0,0}, (Vector2){300, GetScreenHeight()} );
            controlsPane.width += 8;
            resultPane   = RectanglePosition(GetScreenRect(), (Vector2){1,0}, (Vector2){GetScreenWidth()-300, GetScreenHeight()} );
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
            
            //Rectangle controlBounds = (Rectangle){ 600, 60, 200, 20 };
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
                    GuiLine(controlBounds, "Demo 1 : RectangleOffset()");
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount X", &demo0AmountX, -20.0f, 20.0f, 1);
                    LayoutFloatInput(&controlBounds, "Amount Y", &demo0AmountY, -20.0f, 20.0f, 1);
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleOffset(container, %.1ff, %.1ff);", demo0AmountX, demo0AmountY);
                    
                    result = RectangleOffset(container, demo0AmountX, demo0AmountY);
                }
                break;
                case DEMO_INFLATE:
                {
                    GuiLine(controlBounds, "Demo 2 : RectangleInflate()");
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount", &demo1Amount, -20.0f, 20.0f, 1);
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleInflate(container, %.1ff);", demo1Amount);
                    
                    result = RectangleInflate(container, demo1Amount);
                }
                break;
                case DEMO_INFLATE_EX:
                {
                    GuiLine(controlBounds, "Demo 3 : RectangleInflateEx()");
                    controlBounds.y += controlBounds.height + 5.0f;
                    
                    LayoutFloatInput(&controlBounds, "Amount X", &demo2AmountX, -20.0f, 20.0f, 1);
                    LayoutFloatInput(&controlBounds, "Amount Y", &demo2AmountY, -20.0f, 20.0f, 1);
                    
                    sprintf(codeBuffer, "Rectangle result = RectangleInflateEx(container, %.1ff, %.1ff);", demo2AmountX, demo2AmountY);
                    
                    result = RectangleInflateEx(container, demo2AmountX, demo2AmountY);
                }
                break;
                case DEMO_POSITION:
                {
                    GuiLine(controlBounds, "Demo 3 : RectanglePosition()");
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
                    GuiLine(controlBounds, "Demo 5 : RectangleGridCell()");
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
                    GuiLine(controlBounds, "Demo 6 : RectangleGridCellEx()");
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
            }
            
            // Draw the result rectangle
            DrawRectangleRec(result, GetColor(0xc9effeff));
            
            // Redraw the container outline so it's not obscured
            DrawRectangleLinesEx(container, 1.0f, Fade(GetColor(0x838383ff), .75f));
            
            DrawRectangleLinesEx(result, 1.0f, GetColor(0x5bb2d9ff));
            
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