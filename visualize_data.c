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
#define ZOOM_FACTOR 40.0f

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
        .target = {
            .x = MIN_X,
            .y = MIN_Y,
        },
        .offset = {
            .x = (MAX_X - MIN_X) / 2,
            .y = (MAX_Y - MIN_Y) / 2,
        },
        .zoom = ZOOM_FACTOR,
    };

    while (!WindowShouldClose()) {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
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
