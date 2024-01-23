#ifndef MP_FPS_TRACKER_H
#define MP_FPS_TRACKER_H

typedef struct FpsTrackerStyle {
    Color colBg;
    Color colBorder;
    Color colTracker;
    float margin;
} FpsTrackerStyle;

typedef struct FpsTrackerData {
    int bufferSize;
    float *samples;
    Color *markers;
    
    int sampleIndex;
    
    float minValue;
    float maxValue;
} FpsTrackerData;

void InitFpsTracker(int bufferSize);
void UnloadFpsTracker();

void MarkFpsTracker(Color colour);
void ProcessFpsTracker(Rectangle bounds);

void GetFpsTrackerStyle(FpsTrackerStyle *style);
void SetFpsTrackerStyle(FpsTrackerStyle *style);

#endif

#ifdef MP_FPS_TRACKER_IMPLEMENTATION

#ifndef MP_FPS_TRACKER_ALLOC
#define MP_FPS_TRACKER_ALLOC(bytes) MemAlloc(bytes)
#endif

#ifndef MP_FPS_TRACKER_FREE
#define MP_FPS_TRACKER_FREE(bytes) MemFree(bytes)
#endif

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

static FpsTrackerData tracker = { 0 };

static FpsTrackerStyle style = {
    .colBg      = BLACK,
    .colBorder  = LIME,
    .colTracker = RAYWHITE,
    .margin     = 10
};

void InitFpsTracker(int bufferSize) {
    tracker.bufferSize = bufferSize;
    tracker.samples = (float*)MP_FPS_TRACKER_ALLOC(sizeof(float)*tracker.bufferSize);
    tracker.markers = (Color*)MP_FPS_TRACKER_ALLOC(sizeof(Color)*tracker.bufferSize);
    tracker.sampleIndex = 0;
    
    tracker.minValue = INFINITY;
    tracker.maxValue = -INFINITY;
}

void UnloadFpsTracker() {
    MP_FPS_TRACKER_FREE(tracker.samples);
    tracker.bufferSize = 0;
}

void ProcessFpsTracker(Rectangle bounds) {
    float dt = GetFrameTime();
    
    tracker.samples[tracker.sampleIndex++] = dt;
    if (tracker.sampleIndex == tracker.bufferSize) {
        tracker.sampleIndex = 0;
    }
    tracker.markers[tracker.sampleIndex] = (Color){ 0, 0, 0, 0};
    
    int sampleIndexRead = tracker.sampleIndex + tracker.bufferSize - tracker.bufferSize/2;
    
    // Find the extrema for scaling
    float minValue = INFINITY, maxValue = -INFINITY;
    
    minValue = tracker.minValue;
    maxValue = tracker.maxValue;
    
    for (int i = 0; i < tracker.bufferSize/2; i++) {
        float value = tracker.samples[(sampleIndexRead+i)%tracker.bufferSize];
        minValue = fmin(minValue, value);
        maxValue = fmax(maxValue, value);
    }
    
    if (minValue ==  INFINITY) { minValue = 0.0f; }
    if (maxValue == -INFINITY) { maxValue = 1.0f; }
    
    tracker.minValue = minValue;
    tracker.maxValue = maxValue;
    
    // @TODO option to adjust to new extrema
    minValue = 0.0f;
    maxValue = 1.0f / 15.0f;
    
    rlDrawRenderBatchActive();      // Update and draw internal render batch
    
    // Draw the graph
    DrawRectangleRec(bounds, style.colBg);
    DrawRectangleLinesEx(bounds, 1.0f, style.colBorder);
    
    float margin = style.margin;
    
    float yMin = bounds.y + bounds.height - margin;
    float yMax = bounds.y + margin;
    
    // @TODO custom guides
    {
        // Green line for 60fps
        float y = Remap(Clamp(1.0f/60.0f, minValue, maxValue), minValue, maxValue, yMin, yMax);
        DrawLine(bounds.x, y, bounds.x + bounds.width, y, Fade(GREEN, 0.5));
        
        // Orange line for 30fps
        y = Remap(Clamp(1.0f/30.0f, minValue, maxValue), minValue, maxValue, yMin, yMax);
        DrawLine(bounds.x, y, bounds.x + bounds.width, y, Fade(ORANGE, 0.5));
        
        // Red line for 15fps
        y = Remap(Clamp(1.0f/15.0f, minValue, maxValue), minValue, maxValue, yMin, yMax);
        DrawLine(bounds.x, y, bounds.x + bounds.width, y, Fade(RED, 0.5));
    }
    
    float prevX = 0, prevY = 0;
    for (int i = 0; i < tracker.bufferSize/2; i++) {
        int index = (sampleIndexRead+i)%tracker.bufferSize;
        float value = tracker.samples[index];
        float y = Remap(Clamp(value, minValue, maxValue), minValue, maxValue, yMin, yMax);
        float x = bounds.x + margin + (bounds.width-margin*2) * 2.0f * (float)i / (float)tracker.bufferSize;

        if (i > 0) {
            DrawLine(prevX, prevY, x, y, style.colTracker);
        }
        
        if (tracker.markers[index].a > 0) {
            DrawLine(x, bounds.height, x, bounds.height-margin, tracker.markers[index]);
        }
        
        prevX = x;
        prevY = y;
    }
    
    DrawText(TextFormat("%.1f", dt * 1000.0f), bounds.x + margin, bounds.y + margin, 10, style.colBorder);
    DrawText("ms", bounds.x + margin + 20, bounds.y + margin, 10, style.colBorder);
}

void MarkFpsTracker(Color colour) {
    tracker.markers[tracker.sampleIndex] = colour;
}

void GetFpsTrackerStyle(FpsTrackerStyle *outStyle) {
    *outStyle = style;
}

void SetFpsTrackerStyle(FpsTrackerStyle *inStyle) {
    style = *inStyle;
}

#endif