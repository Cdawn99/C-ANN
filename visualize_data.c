#define UTILS_IMPLEMENTATION
#include "utils.h"

#include "raylib.h"

#include <string.h>

#define MIN_X (-10.0f)
#define MAX_X 10.0f

#define MIN_Y (-10.0f)
#define MAX_Y 10.0f

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900
#define ZOOM_FACTOR 42.9f

#define POINT_RADIUS 0.2f

typedef struct {
    Vector2 p;
    int classification;
} ClassifiedPoint;

typedef struct {
    ClassifiedPoint *items;
    size_t length;
    size_t capacity;
} ClassifiedPoints;

int main(void) {
    char *data;
    if (read_entire_file("input.csv", &data) == 0) {
        return 1;
    }

    ClassifiedPoints points = {0};

    char *token = strtok(data, ",\n");
    while (token) {
        ClassifiedPoint p = {0};

        p.p.x = atof(token);
        token = strtok(NULL, ",\n");

        p.p.y = -atof(token);
        token = strtok(NULL, ",\n");

        p.classification = atof(token);
        token = strtok(NULL, ",\n");

        UTILS_DA_APPEND(&points, p);
    }

    free(data);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Data visualizer");
    SetTargetFPS(60);

    Camera2D camera = {
        .offset = {
            .x = SCREEN_WIDTH/2.0f,
            .y = SCREEN_HEIGHT/2.0f,
        },
        .zoom = ZOOM_FACTOR,
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                DrawTextEx(GetFontDefault(), "-10", (Vector2){-10.3, 0}, 0.8, 0.08, BLACK);
                DrawTextEx(GetFontDefault(), "10", (Vector2){9.7, 0}, 0.8, 0.08, BLACK);
                DrawLine(-10, 0, 10, 0, BLACK);
                DrawLine(0, -10, 0, 10, BLACK);
                for (size_t i = 0; i < points.length; i++) {
                    DrawCircleV(points.items[i].p, POINT_RADIUS, points.items[i].classification == 1 ? RED : BLUE);
                }
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    UTILS_DA_FREE(&points);

    return 0;
}
