#include <raylib.h>
#include <math.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

#define MP_CAMERA2D_IMPLEMENTATION
#include "../../src/mp_camera2d.h"


Camera2DConfig cameraConfig;
Camera2D camera;

// Pseudo-random values for generating the pattern
float hashCoefs[12];

// Helper functions for the pattern
float fractf(float x) {
	return x - floorf(x);
}

float SmeggyHash21(float x, float y, float *coefs) {
	return fractf(sinf(x * coefs[0] + y * coefs[1]) * coefs[2]);
}


void UpdateDrawFrame();

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1024, 768, "Camera2D Helper Demo");
    
	// Default camera config
	cameraConfig = LoadDefaultCamera2DConfig();
	
	// Basic camera aiming at world origin
	camera = (Camera2D) {
		.target = (Vector2){ 0.0f, 0.0f },
		.offset = (Vector2){ (float)GetScreenWidth() * .5f, (float)GetScreenHeight() * .5f },
		.zoom = 1.0f
	};
	
	// Pseudo-random values for generating the pattern
	for (int i = 0; i < 12; i++) {
		hashCoefs[i] = GetRandomValue(-99999, 99999) * .1f;
	}
	
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
#else
    // Run run as fast as you can
    SetTargetFPS(0);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateDrawFrame();
    }
#endif
    
    CloseWindow();

    return 0;
	
}

void UpdateDrawFrame() {
        
	// Update the camera based on input
	UpdateCamera2D(&camera, &cameraConfig);
	
	if (IsKeyPressed(KEY_SPACE)) {
		cameraConfig.mouseButtonForPan = (cameraConfig.mouseButtonForPan == -1) ? 1 : -1;
	}
	
	// Draw frame
	BeginDrawing();
	
		ClearBackground(DARKBLUE);
		
		// World space coordinates of screen top-left and bottom-right
		Vector2 tl = GetScreenToWorld2D((Vector2){0,0}, camera);
		Vector2 br = GetScreenToWorld2D((Vector2){(float)GetScreenWidth(), (float)GetScreenHeight()}, camera);
	
		BeginMode2D(camera);
		
			// Just draw a random scene that we can move around in
			DrawLineV( (Vector2){ 0.0f, tl.y }, (Vector2){ 0.0f, br.y }, WHITE);
			DrawLineV( (Vector2){ tl.x, 0.0f }, (Vector2){ br.x, 0.0f }, WHITE);
			
			const float cellSize = 100.0f;
			float xMin = floorf(tl.x / cellSize), xMax = ceilf(br.x / cellSize);
			float yMin = floorf(tl.y / cellSize), yMax = ceilf(br.y / cellSize);
			
			for (float y = yMin; y <= yMax; y += 1.0f) {
				for (float x = xMin; x <= xMax; x += 1.0f) {
					
					// Create a pseudo-random offset for this "cell"
					float xOffset = SmeggyHash21(x, y, hashCoefs+0) - 0.5f;
					float yOffset = SmeggyHash21(x, y, hashCoefs+3) - 0.5f;
					int shapeType = (int)floorf(7.0f * SmeggyHash21(x, y, hashCoefs+6));
					float rotation = SmeggyHash21(x, y, hashCoefs+9) * 360.0f;
					
					Vector2 centre = (Vector2){ (x + xOffset) * cellSize, (y + yOffset)*cellSize };
					float radius = cellSize * .25f;
					
					switch(shapeType) {
						case 0:
							DrawCircleV( centre, radius, Fade(YELLOW, 0.75f) );
							break;
						default:
							DrawPoly(centre, 2+shapeType, radius, rotation, Fade(YELLOW, 0.75f) );
					}
					
				}
			}
		
		EndMode2D();
		
		// Draw UI
		DrawRectangle(0, 0, 450, 330, Fade(BLACK, 0.5f));
		
		float y = 10.0f;
		float fontSize = 20.0f;
		float lineHeight = fontSize + 5.0f;
		
		DrawText("Mouse controls:", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		DrawText("> Hold RMB and drag to pan", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		DrawText("> Mouse wheel to zoom about mouse point", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		
		y += lineHeight;
		
		DrawText("Touch controls:", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		DrawText("> One finger drag to pan", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		DrawText("> Two finger pinch to zoom", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		
		y += lineHeight;
		
		DrawText(TextFormat("Current zoom: %.0f%%", camera.zoom * 100.0f), 10.0f, y += lineHeight, fontSize, RAYWHITE);
		DrawText("Current viewport:", 10.0f, y += lineHeight, fontSize, RAYWHITE);
		DrawText(TextFormat("> (%.1f,%.1f)-(%.1f,%.1f)", tl.x, tl.y, br.x, br.y), 10.0f, y += lineHeight, fontSize, RAYWHITE);
	
	EndDrawing();
}