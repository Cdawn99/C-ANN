#define UTILS_IMPLEMENTATION
#include "utils.h"

#include "raylib.h"

#include <math.h>
#include <string.h>

#define MIN_X (-10.0f)
#define MAX_X 10.0f

#define MIN_Y (-10.0f)
#define MAX_Y 10.0f

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

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

void get_classified_points_from_csv(ClassifiedPoints *points, const char *filepath) {
    if (!points || !filepath) {
        return;
    }

    char *data;
    if (read_entire_file(filepath, &data) == 0) {
        return;
    }

    char *token = strtok(data, ",\n");
    while (token) {
        ClassifiedPoint p = {0};

        p.p.x = atof(token);
        token = strtok(NULL, ",\n");

        p.p.y = -atof(token);
        token = strtok(NULL, ",\n");

        p.classification = atof(token);
        token = strtok(NULL, ",\n");

        UTILS_DA_APPEND(points, p);
    }

    free(data);
}

int main(int argc, char **argv) {
    if (argc == 1) {
        printf("Usage: %s <training.csv> [<testing.csv>]\n", argv[0]);
        putchar('\n');
        puts("Both CSV files should have the following format witout a header:");
        puts("x_coord,y_coord,class");
        putchar('\n');
        puts("The 'class' may have one of the following values: '+1' or '-1'.");
        putchar('\n');
        puts("As long as at least one argument is provided, the GUI will open");
        puts("but nothing will be displayed from files that the program failed to read.");
        return 0;
    }

    ClassifiedPoints training = {0};
    if (argc >= 2) {
        get_classified_points_from_csv(&training, argv[1]);
    }

    ClassifiedPoints testing = {0};
    if (argc >= 3) {
        get_classified_points_from_csv(&testing, argv[2]);
    }

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Data visualizer");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    Camera2D camera = {0};

    while (!WindowShouldClose()) {
        float screen_width = GetScreenWidth();
        float screen_height = GetScreenHeight();
        float min_screen_side = (screen_width + screen_height)/2.0f - fabsf(screen_width - screen_height)/2.0f;
        camera.offset = (Vector2){
            .x = screen_width/2.0f,
            .y = screen_height/2.0f,
        };
        camera.zoom = min_screen_side/21.0f;

        BeginDrawing();
            ClearBackground(RAYWHITE);
            BeginMode2D(camera);
                // Draw axes with min and max x-values.
                DrawTextEx(GetFontDefault(), "-10", (Vector2){-10.3, 0}, 0.8, 0.08, BLACK);
                DrawTextEx(GetFontDefault(), "10", (Vector2){9.7, 0}, 0.8, 0.08, BLACK);
                DrawLine(-10, 0, 10, 0, BLACK);
                DrawLine(0, -10, 0, 10, BLACK);

                for (size_t i = 0; i < training.length; i++) {
                    if (training.items[i].classification == 1) {
                        DrawCircleV(training.items[i].p, POINT_RADIUS, PINK);
                    } else if (training.items[i].classification == -1) {
                        DrawCircleV(training.items[i].p, POINT_RADIUS, SKYBLUE);
                    }
                }

                for (size_t i = 0; i < testing.length; i++) {
                    if (testing.items[i].classification == 1) {
                        DrawCircleV(testing.items[i].p, POINT_RADIUS, MAROON);
                    } else if (testing.items[i].classification == -1) {
                        DrawCircleV(testing.items[i].p, POINT_RADIUS, DARKBLUE);
                    }
                }
            EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    UTILS_DA_FREE(&training);
    UTILS_DA_FREE(&testing);

    return 0;
}
