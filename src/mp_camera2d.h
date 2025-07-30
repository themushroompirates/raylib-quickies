#ifndef MP_CAMERA2D_H
#define MP_CAMERA2D_H

typedef struct Camera2DConfig {
	int mouseButtonForPan;
	
	int modifierKeyForZoom;
	int modifierKeyForRotate;
	
	int keyForZoomIn;
	int keyForZoomOut;
	
	float minZoom;
	float maxZoom;
	
#ifndef MP_CAMERA2D_NO_TOUCH
	// ------ Touch state ----
	bool _zooming;
	Vector2 _zoomTouchPoints[2];
	float _zoomStartZoom;
	
	bool _panning;
	Vector2 _panStartPoint;
#endif
} Camera2DConfig;

Camera2DConfig LoadDefaultCamera2DConfig();
void UpdateCamera2D(Camera2D *camera, Camera2DConfig *config);

#endif

#ifdef MP_CAMERA2D_IMPLEMENTATION

#include <raylib.h>
#include <raymath.h>

Camera2DConfig LoadDefaultCamera2DConfig() {
	Camera2DConfig config = { 0 };
	
	config.mouseButtonForPan = MOUSE_BUTTON_RIGHT;
	
	config.modifierKeyForZoom = KEY_NULL; // Mouse wheel by itself zooms
	
	config.keyForZoomIn  = KEY_EQUAL;
	config.keyForZoomOut = KEY_MINUS;
	
	config.minZoom = 0.5f;
	config.maxZoom = 10.0f;
	
	return config;
}

void UpdateCamera2D(Camera2D *camera, Camera2DConfig *config) {
	// Mouse panning
	if (GetTouchPointCount() == 0) {
		if (config->mouseButtonForPan == -1 || IsMouseButtonDown(config->mouseButtonForPan)) {
			Vector2 p1 = GetMousePosition();
			Vector2 p0 = Vector2Subtract(GetMousePosition(), GetMouseDelta());
			p0 = GetScreenToWorld2D(p0, *camera);
			p1 = GetScreenToWorld2D(p1, *camera);
			camera->target = Vector2Add(camera->target, Vector2Subtract(p0, p1));
		}
	}
	
	// Mouse/Keyboard zooming
	float zoomFactor = 0.0f;
	if (config->modifierKeyForZoom == KEY_NULL || IsKeyDown(config->modifierKeyForZoom)) {
		zoomFactor = GetMouseWheelMove();
	}
	if (IsKeyPressed(config->keyForZoomIn))  zoomFactor =  1.0f;
	if (IsKeyPressed(config->keyForZoomOut)) zoomFactor = -1.0f;
	if (zoomFactor != 0.0f) {
		Vector2 mouseWorldPos = GetScreenToWorld2D(GetMousePosition(), *camera);
		camera->offset = GetMousePosition();
		camera->target = mouseWorldPos;
		
		camera->zoom = Clamp(expf(logf(camera->zoom)+zoomFactor*.2f), config->minZoom, config->maxZoom);
	}
	
#ifndef MP_CAMERA2D_NO_TOUCH
	// Handle touch input
	int touches = GetTouchPointCount();
	
	if (touches == 0) {
		// Touch cancel
		config->_panning = false;
		config->_zooming = false;
	}
	else if (touches == 1) {
		// Touch pan
		config->_zooming = false;
		
		if (config->_panning) {
			Vector2 currentPos = GetTouchPosition(0);
			
			Vector2 delta = Vector2Subtract(
				currentPos, config->_panStartPoint
			);
			
			delta = Vector2Scale(delta, -1.0f/camera->zoom);
			camera->target = Vector2Add(camera->target, delta);
		
			config->_panStartPoint = currentPos;
		}
		else {
			config->_panStartPoint = GetTouchPosition(0);
			config->_panning = true;
		}
	}
	else {
		config->_panning = false;
		
		// Touch zoom
		if (!config->_zooming) {
			config->_zoomTouchPoints[0] = Vector2Multiply(GetTouchPosition(0), (Vector2){GetScreenWidth(), GetScreenHeight()});
			config->_zoomTouchPoints[1] = Vector2Multiply(GetTouchPosition(1), (Vector2){GetScreenWidth(), GetScreenHeight()});
			config->_zooming = true;
			config->_zoomStartZoom = camera->zoom;
		}
		else {
			Vector2 zp0 = Vector2Multiply(GetTouchPosition(0), (Vector2){GetScreenWidth(), GetScreenHeight()});
			Vector2 zp1 = Vector2Multiply(GetTouchPosition(1), (Vector2){GetScreenWidth(), GetScreenHeight()});
			
			float z1 = fabsf(zp0.x - zp1.x) / fabsf(config->_zoomTouchPoints[0].x - config->_zoomTouchPoints[1].x);
			float z2 = fabsf(zp0.y - zp1.y) / fabsf(config->_zoomTouchPoints[0].y - config->_zoomTouchPoints[1].y);
			float scale = fmaxf(z1, z2);
			
			camera->zoom = Clamp(config->_zoomStartZoom * scale, config->minZoom, config->maxZoom);
		}
	}
#endif
	
	
}

#endif