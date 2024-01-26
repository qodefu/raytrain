#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>

static float width;
static float height;

float rand_float() {
    return (float) rand() / (float)RAND_MAX;
}

float map(float v, float s1, float e1, float s2, float e2) {
    return v *(e2 - s2)/( e1 - s1 );
}

int random(int start, int end) {
    return start + (rand() % (start - end));
}


Vector2 project_sample_to_screen(float x, float y) {

   return (Vector2) {
     .x = x + (width /2),
     .y = y + (height/2),
   };

}
struct star {

    float x;
    float y;
    float z;
    float pz;
    star() {
        x = random(-width/2, width/2);
        y = random(-height/2, height/2);
        z = random(0,width);
        pz = z;
    }

    void update() {
        z = z - 20;
        if (z < 1) {
            x = random(-width/2, width/2);
            y = random(-height/2, height/2);
            z = random(0,width);
            pz = z;
        }

    }

    void show() {
        float sx = map(x/z, 0, 1, 0, width);
        float sy = map(y/z, 0, 1, 0, height);

        float ox = map(x/pz, 0, 1, 0, width);
        float oy = map(y/pz, 0, 1, 0, height);
        Vector2 opt = project_sample_to_screen(ox, oy);
        Vector2 npt = project_sample_to_screen(sx, sy);
        float r = map(z, 0, width, 4, 0);

        DrawLine(opt.x, opt.y, npt.x, npt.y, WHITE);
        DrawEllipse(npt.x, npt.y, r, r, WHITE);
    }

};



using namespace std;
ostream& operator<< (ostream& os,
                     const star& pt) {
    os << "{"<< pt.x << "," << pt.y << ","<< pt.z  << "}";
    return os;
}

int main(void)  {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800 ,600, "starfield");
    SetTargetFPS(60);

    width = GetScreenWidth();
    height = GetScreenHeight();
    array<star, 500> stars;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818AA));
        if (IsKeyPressed(KEY_P)) {
            for (auto &pt: stars) {
                cout << pt << " ";
            }
            cout << endl;
        }

        for (auto &pt: stars) {
            pt.update();
            pt.show();
        }

        //DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2,10, RED);
        EndDrawing();
    }


    CloseWindow();


    return 0;
}
