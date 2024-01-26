#include <bits/stdc++.h>
#include <raylib.h>
#include <raymath.h>
#include <rlgl.h>
#include "lighting.cc"

#define RADIUS 6.0f
#define BOX_RADIUS 3.0f

using namespace std;
Color colors[4] ={
WHITE,
RED,
BLUE,
GREEN,
};
static float a = 0;
Shader shader;

struct Box {
    float x, y, z, r;
    Color c;
    Model cube;

    Box(float x, float y, float z, float r): x(x),y(y),z(z),r(r){
        cube = LoadModelFromMesh(GenMeshCube(r,r,r));
        cube.materials[0].shader = shader;
    }

    void show() {
        rlPushMatrix();
            rlRotatef(a, 1, 0, -1);
            DrawModel(cube, Vector3{x,y,z}, 1.0f, WHITE);
            //DrawCube(Vector3{x,y,z}, r,r,r, c);
            //DrawCubeWires(Vector3{x,y,z}, r,r,r, c);
        rlPopMatrix();
    }

    vector<Box> generate() {
        vector<Box> ret;
        for (int i = -1; i <2; i++ ) {
            for (int j = -1; j <2; j++ ) {
                for (int k = -1; k < 2; k++ ) {
                    if (abs(i)+abs(j)+abs(k) <= 1) {
                        continue;
                    }
                    ret.push_back(Box(x+i*r/3, y+j*r/3, z+k*r/3, r/3));
                }
            }
        }
        return ret;

    }
};

int main() {
    TextFormat("blah");
    InitWindow(800, 800, "menger");
    shader = LoadShaderFromMemory(lighting_vs.c_str(), lighting_fs.c_str());

    // Get some required shader locations
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    // NOTE: "matModel" location name is automatically assigned on shader loading,
    // no need to get the location again if using that uniform name
    //shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocation(shader, "matModel");

    // Ambient light level (some basic lighting)
    int ambientLoc = GetShaderLocation(shader, "ambient");
    SetShaderValue(shader, ambientLoc, (float[4]){ 0.1f, 0.1f, 0.1f, 1.0f }, SHADER_UNIFORM_VEC4);

    Camera camera;
    camera.position = {RADIUS, 0, 0};
    camera.target = {0};
    camera.fovy = 90;
    camera.up = {0,1,0};
    camera.projection = CAMERA_PERSPECTIVE;
    Box box(0,0,0, BOX_RADIUS);
    vector<Box> sponge{ Box(0,0,0,BOX_RADIUS)};
    vector<Box> next;
    Light light = CreateLight(LIGHT_POINT, (Vector3){ 3.5, 0, 0 }, Vector3Zero(), RED, shader);



    SetTargetFPS(30);
    while(!WindowShouldClose()) {

        a += 0.2;
        BeginDrawing();
        ClearBackground(GetColor(0x181818AA));
        if (IsKeyPressed(KEY_SPACE)) {
            next.clear();
            for (Box box: sponge) {
               auto children = box.generate();
                next.insert(next.end(), children.begin(), children.end());
            }

            sponge = next;
        }
        BeginMode3D(camera);
        for (Box box: sponge) {
            box.show();
        }
        EndMode3D();
        EndDrawing();

    }
    CloseWindow();

}
