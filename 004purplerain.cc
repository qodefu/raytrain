#include "common.cc"
#include "raylib.h"

Vector2 dim;
Color bg, purple;
#define RAIN_RADIUS 3
#define RAIN_SPEED 0.8

struct rain {
    float x,y,z;
    float dy;
    rain() {
       x = random(0, dim.x);
       y = random(0, dim.y);
       z = random(0, dim.x);
    }
    void update() {
        dy = map_range(z, 0, dim.x,RAIN_SPEED, RAIN_SPEED*2);
        y = y + dy;
        if (y >= dim.y) {
           y = 0;
       }
    }
    void draw() {
        update();
        float radius  = map_range(z, 0, dim.x, RAIN_RADIUS, RAIN_RADIUS*3);
        DrawRectangle(x, y, radius*0.6, radius*6, purple);
    }
};

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800, 600, "004 purple rain");
    purple=Color{138,43,226,255};
    bg=Color{230, 230, 250, 255};
    dim=dimension();
    array<rain, 500> rains;
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(bg);
        for (auto &r: rains) {
            r.draw();
        }

        EndDrawing();
    }
    CloseWindow();
}
