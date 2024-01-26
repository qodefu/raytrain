#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>
#define SNAKE_RADIUS 20

using namespace std;

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

ostream& operator<< (ostream& os,
                     const Vector2& pt) {
    os << "{"<< pt.x << "," << pt.y << ","<< "}";
    return os;
}

float round_radius(float v) {
    float ret = v;
    int mod = int(v) % SNAKE_RADIUS;
    if (mod != 0 ) {
        ret = int(v) + SNAKE_RADIUS - mod;
    }

    return ret;
}

Vector2 dimension() {
    width = GetScreenWidth();
    height = GetScreenHeight();
    return Vector2{width, height};
}

Vector2 project_sample_to_screen(float x, float y) {

   return (Vector2) {
     .x = x + (width /2),
     .y = y + (height/2),
   };

}


struct snake {

    float x;
    float y;
    float dx = 1;
    float dy = 0;
    std::deque<Vector2> body;
    snake() {
        auto dim = dimension();
        x = round_radius(dim.x/2);
        y = round_radius(dim.y/2);
        grow();
    }

    void grow() {
        body.push_front(Vector2{x,y});
    }
    void update() {
        x += dx*SNAKE_RADIUS;
        y += dy*SNAKE_RADIUS;
        body.push_front(Vector2{x,y});
        body.pop_back();
    }

    void show() {
        for (auto vec2: body) {
            DrawRectangle(vec2.x, vec2.y, SNAKE_RADIUS, SNAKE_RADIUS, WHITE);
            cout << vec2 << " ";
        }
        cout << endl;
    }

};


struct food {
    float x;
    float y;

    food() {
        next();
    }

    void show() {
        DrawRectangle(x, y, SNAKE_RADIUS, SNAKE_RADIUS, RED);
    }

    void next() {
        auto dim = dimension();
        x = round_radius(random(0, dim.x-SNAKE_RADIUS));
        y = round_radius(random(0, dim.y-SNAKE_RADIUS));
        cout << "food: " << x << ", " << y << endl;
    }
};

void check_eating(snake &snk, food &foodie) {
    if (snk.x == foodie.x && snk.y == foodie.y) {
        snk.grow();
        foodie.next();
    }

}

int main(void)  {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(800 ,600, "003snake");
    int fps = SNAKE_RADIUS/2;
    SetTargetFPS(fps);

    snake snake;
    food food;

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(GetColor(0x181818AA));
        if (IsKeyPressed(KEY_UP)) {
            if (snake.dy == 0) {
                snake.dy = -1;
                snake.dx = 0;
            }
        } else if (IsKeyPressed(KEY_DOWN)) {
            if (snake.dy == 0) {
                snake.dy = 1;
                snake.dx = 0;
            }
        } else if (IsKeyPressed(KEY_LEFT)) {
            if (snake.dx == 0) {
                snake.dy = 0;
                snake.dx = -1;
            }
        } else if (IsKeyPressed(KEY_RIGHT)) {
            if (snake.dx == 0) {
                snake.dy = 0;
                snake.dx = 1;
            }
        }
        if (IsKeyPressed(KEY_G)) {
            snake.grow();
        }
        if (IsKeyPressed(KEY_A)) {
            fps +=1;
            SetTargetFPS(fps);

        }
        check_eating(snake, food);
        snake.update();
        snake.show();
        food.show();

        //DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2,10, RED);
        EndDrawing();
    }


    CloseWindow();


    return 0;
}
