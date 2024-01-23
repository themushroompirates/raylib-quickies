#include <string.h>

#include "raylib.h"

#define FPS_TRACKER_IMPLEMENTATION
#include "../src/mp_fps_tracker.h"

void CreateObjectTransform(Transform *t) {
    // Random translation
    t->translation = (Vector3) {
        (float)GetRandomValue(-100, 100) * 0.1f,
        (float)GetRandomValue(-100, 100) * 0.1f,
        (float)GetRandomValue(-100, 100) * 0.1f
    };
    // Random axis of rotation
    Vector3 axis = Vector3Normalize((Vector3) {
        (float)GetRandomValue(-100, 100) * 0.01f,
        (float)GetRandomValue(-100, 100) * 0.01f,
        (float)GetRandomValue(-100, 100) * 0.01f
    });
    // The `Transform` struct is supposed store this as a quat
    // I'm using axis-angle instead
    t->rotation = (Vector4){ axis.x, axis.y, axis.z, 0.0f };
    
    // Don't care about scale for now
    t->scale = Vector3One();
}

int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1024, 768, "FPS Tracker Demo");
    
    // Run run as fast as you can
    SetTargetFPS(0);
    
    // Set up the fps tracker - 4 seconds' worth at 60fps
    InitFpsTracker(2*60*4);

    // Set up a camera
    Camera camera = (Camera){ 0 };
    camera.position = (Vector3){ 12.5f, 12.5f, 13.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type
    
    // Create some basic meshes
    Mesh meshes[] = {
        GenMeshCube(1.0f, 0.5f, 0.5f),
        GenMeshSphere(0.5f, 10, 10),
        GenMeshCone(0.5f, 1.0f, 10),
        GenMeshKnot(0.25f, 1.0f, 10, 50)
    };
    
    // And a material
    Material objectMaterial = LoadMaterialDefault();
    
    int objectCapacity = 20;
    int objectCount = 10;
    Transform *objectTransforms = (Transform*)MemAlloc(sizeof(Transform)*objectCapacity);
    
    for (int i = 0; i < objectCount; i++) {
        CreateObjectTransform(&objectTransforms[i]);
    }


    // --------------------------  BEGIN MAIN LOOP  ---------------------------
    
    while (!WindowShouldClose()) {
        
        if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
            UpdateCamera(&camera, CAMERA_THIRD_PERSON);
        }
        
        if (IsKeyPressed(KEY_LEFT)) {
            int objectsToRemove = 100;
            
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                objectsToRemove *= 10;
            }
            if (IsKeyDown(KEY_LEFT_CONTROL)) {
                objectsToRemove *= 10;
            }
            
            objectCount -= objectsToRemove;
            if (objectCount < 1) objectCount = 1;
        }
        
        if (IsKeyPressed(KEY_RIGHT)) {
            int objectsToAdd = 100;
            
            if (IsKeyDown(KEY_LEFT_SHIFT)) {
                objectsToAdd *= 10;
            }
            if (IsKeyDown(KEY_LEFT_CONTROL)) {
                objectsToAdd *= 10;
            }
            
            if (objectCount + objectsToAdd >= objectCapacity) {
                // Yayyy let's thrash the heap a bit
                int newCapacity = (objectCapacity + objectsToAdd) * 2 + 10;
                Transform *newTransforms = (Transform*)MemAlloc(sizeof(Transform) * newCapacity);
                if (newTransforms == 0) {
                    // Allocation failed; do nothing
                    MarkFpsTracker(MAGENTA);
                }
                else {
                    // Copy the old stuff to the new buffer
                    memcpy(newTransforms, objectTransforms, sizeof(Transform) * objectCount);
                    
                    // Free the old buffer
                    MemFree(objectTransforms);
                    
                    // Update pointers and counts
                    objectTransforms = newTransforms;
                    objectCapacity = newCapacity;
                    
                    // And let's tell the tracker we did an allocation
                    MarkFpsTracker(RAYWHITE);
                }
            }
            
            for (int i = 0; i < objectsToAdd; i++) {
                CreateObjectTransform(&objectTransforms[objectCount++]);
            }
        }
        
        for (int i = 0; i < objectCount; i++) {
            // Spin spin spin
            objectTransforms[i].rotation.w += DEG2RAD * 90 * GetFrameTime();
            
            // Move along the spin axis
            Vector3 axis = (Vector3){ objectTransforms[i].rotation.x, objectTransforms[i].rotation.y, objectTransforms[i].rotation.z };
            objectTransforms[i].translation = Vector3Add(objectTransforms[i].translation, Vector3Scale(axis, 2.0f * GetFrameTime()));
            
            // Bounce off the edges
            if (fabsf(objectTransforms[i].translation.x) > 10.0f) { objectTransforms[i].rotation.x = -objectTransforms[i].rotation.x; }
            if (fabsf(objectTransforms[i].translation.y) > 10.0f) { objectTransforms[i].rotation.y = -objectTransforms[i].rotation.y; }
            if (fabsf(objectTransforms[i].translation.z) > 10.0f) { objectTransforms[i].rotation.z = -objectTransforms[i].rotation.z; }
        }
        
        BeginDrawing();

            ClearBackground(RAYWHITE);
            
            BeginMode3D(camera);
            
            // Draw a cage to contain our objects
            rlPushMatrix();
                rlTranslatef(0, -10.0f, 0);
                DrawGrid(20, 1.0f);
            rlPopMatrix();
            
            rlPushMatrix();
                rlRotatef(90.0f, 1, 0, 0);
                rlTranslatef(0, 10, 0);
                DrawGrid(20, 1.0f);
                rlTranslatef(0, -20, 0);
                DrawGrid(20, 1.0f);
            rlPopMatrix();
            
            rlPushMatrix();
                rlRotatef(90.0f, 0, 0, 1);
                rlTranslatef(0, 10, 0);
                DrawGrid(20, 1.0f);
                rlTranslatef(0, -20, 0);
                DrawGrid(20, 1.0f);
            rlPopMatrix();
            
            // Draw all the objects
            for (int i = 0; i < objectCount; i++) {
                Material material = objectMaterial;
                material.maps[MATERIAL_MAP_DIFFUSE].color = ColorFromHSV( (float)((i*1337)%360), 0.75, 0.75);
                
                Vector3 translation = objectTransforms[i].translation;
                Vector3 axis = (Vector3){ objectTransforms[i].rotation.x, objectTransforms[i].rotation.y, objectTransforms[i].rotation.z };
                Matrix matrix = MatrixRotate(axis, objectTransforms[i].rotation.w);
                matrix = MatrixMultiply(matrix, MatrixTranslate(translation.x, translation.y, translation.z));
                
                DrawMesh(meshes[i%4], material, matrix);
            }
            
            EndMode3D();
            
            // Track this frame and draw
            ProcessFpsTracker((Rectangle){ GetScreenWidth() - 220, 10, 210, 100 });
            
            // Just some info
            DrawRectangleRec((Rectangle){ 5, 5, 360, 110 }, Fade(BLACK, 0.75f));
            DrawFPS(10, 10);
            DrawText(TextFormat("Objects: %d", objectCount), 10, 30, 20, LIME);
            DrawText(TextFormat("Capacity: %d (%.2f MB)", objectCapacity, (float)(objectCapacity * sizeof(Matrix)) / (1024.0f * 1024.0f)), 10, 50, 20, LIME);
            DrawText("Right Arrow for +100 objects", 10, 70, 20, LIME);
            DrawText(" (Shift/Ctrl for +1000 or +10000)", 10, 90, 20, LIME);
            
        EndDrawing();
    }
    
    // ---------------------------  END MAIN LOOP  ----------------------------
    
    MemFree(objectTransforms);
    
    UnloadFpsTracker();
    
    CloseWindow();

    return 0;
}