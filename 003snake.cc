#include "common.cc"
#define SNAKE_RADIUS 20



float round_radius(float v) {
    float ret = v;
    int mod = int(v) % SNAKE_RADIUS;
    if (mod != 0 ) {
        ret = int(v) + SNAKE_RADIUS - mod;
    }

    return ret;
}



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


struct snake {

    float x;
    float y;
    float dx = 1;
    float dy = 0;
    std::deque<Vector2> body;
    Vector2 dim;
    snake() {
        dim = dimension();
        x = round_radius(dim.x/2);
        y = round_radius(dim.y/2);
        grow();
    }

    void grow() {
        body.push_front(Vector2{x,y});
    }

    void checkDeath() {
        bool dead = false;
        for (auto v: body) {
            if (x == v.x && y == v.y) {
                cout << "hit body, end game!!!";
                dead =true;
            }
        }
        if (x <0 || x >= dim.x || y >= dim.y || y < 0) {
            cout << "hit wall, end game!!";
            dead=true;
        }
        if (dead) {
            x = round_radius(dim.x/2);
            y = round_radius(dim.y/2);

            body.clear();
            grow();

        }
    }
    void update() {
        x += dx*SNAKE_RADIUS;
        y += dy*SNAKE_RADIUS;
        checkDeath();
        x = Clamp(x, 0, dim.x - SNAKE_RADIUS);
        y = Clamp(y, 0, dim.y - SNAKE_RADIUS);
        body.push_front(Vector2{x,y});
        body.pop_back();
    }
    void eat(food &foodie) {
        if (x == foodie.x && y == foodie.y) {
            grow();
            foodie.next();
        }

    }

    void show() {
        for (auto vec2: body) {
            DrawRectangle(vec2.x, vec2.y, SNAKE_RADIUS, SNAKE_RADIUS, WHITE);
//            cout << vec2 << " ";
        }
//        cout << endl;
    }

};



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
        snake.eat(food);
        snake.update();
        snake.show();
        food.show();

        //DrawCircle(GetScreenWidth()/2, GetScreenHeight()/2,10, RED);
        EndDrawing();
    }


    CloseWindow();


    return 0;
}
