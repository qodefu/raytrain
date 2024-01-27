#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>

using namespace std;
static float rand_float() {
    return (float) rand() / (float)RAND_MAX;
}

static float map_range(float v, float s1, float e1, float s2, float e2) {
    return v *(e2 - s2)/( e1 - s1 );
}

static int random(int start, int end) {
    return start + (rand() % (start - end));
}

static ostream& operator<< (ostream& os,
                     const Vector2& pt) {
    os << "{"<< pt.x << "," << pt.y << ","<< "}";
    return os;
}

static Vector2 dimension() {
    return Vector2{float(GetScreenWidth()), float(GetScreenHeight())};
}

Color color(int r, int g, int b) {


}
