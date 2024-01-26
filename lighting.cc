#include <string>
#include "raylib.h"
using namespace std;
static string lighting_fs = "#version 330\n"
"\n"
"// Input vertex attributes (from vertex shader)\n"
"in vec3 fragPosition;\n"
"in vec2 fragTexCoord;\n"
"//in vec4 fragColor;\n"
"in vec3 fragNormal;\n"
"\n"
"// Input uniform values\n"
"uniform sampler2D texture0;\n"
"uniform vec4 colDiffuse;\n"
"\n"
"// Output fragment color\n"
"out vec4 finalColor;\n"
"\n"
"// NOTE: Add here your custom variables\n"
"\n"
"#define     MAX_LIGHTS              4\n"
"#define     LIGHT_DIRECTIONAL       0\n"
"#define     LIGHT_POINT             1\n"
"\n"
"struct Light {\n"
"    int enabled;\n"
"    int type;\n"
"    vec3 position;\n"
"    vec3 target;\n"
"    vec4 color;\n"
"};\n"
"\n"
"// Input lighting values\n"
"uniform Light lights[MAX_LIGHTS];\n"
"uniform vec4 ambient;\n"
"uniform vec3 viewPos;\n"
"\n"
"void main()\n"
"{\n"
"    // Texel color fetching from texture sampler\n"
"    vec4 texelColor = texture(texture0, fragTexCoord);\n"
"    vec3 lightDot = vec3(0.0);\n"
"    vec3 normal = normalize(fragNormal);\n"
"    vec3 viewD = normalize(viewPos - fragPosition);\n"
"    vec3 specular = vec3(0.0);\n"
"\n"
"    // NOTE: Implement here your fragment shader code\n"
"\n"
"    for (int i = 0; i < MAX_LIGHTS; i++)\n"
"    {\n"
"        if (lights[i].enabled == 1)\n"
"        {\n"
"            vec3 light = vec3(0.0);\n"
"\n"
"            if (lights[i].type == LIGHT_DIRECTIONAL)\n"
"            {\n"
"                light = -normalize(lights[i].target - lights[i].position);\n"
"            }\n"
"\n"
"            if (lights[i].type == LIGHT_POINT)\n"
"            {\n"
"                light = normalize(lights[i].position - fragPosition);\n"
"            }\n"
"\n"
"            float NdotL = max(dot(normal, light), 0.0);\n"
"            lightDot += lights[i].color.rgb*NdotL;\n"
"\n"
"            float specCo = 0.0;\n"
"            if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), 16.0); // 16 refers to shine\n"
"            specular += specCo;\n"
"        }\n"
"    }\n"
"\n"
"    finalColor = (texelColor*((colDiffuse + vec4(specular, 1.0))*vec4(lightDot, 1.0)));\n"
"    finalColor += texelColor*(ambient/10.0)*colDiffuse;\n"
"\n"
"    // Gamma correction\n"
"    finalColor = pow(finalColor, vec4(1.0/2.2));\n"
"}\n";

static string lighting_vs =
"#version 330\n"
"\n"
"// Input vertex attributes\n"
"in vec3 vertexPosition;\n"
"in vec2 vertexTexCoord;\n"
"in vec3 vertexNormal;\n"
"in vec4 vertexColor;\n"
"\n"
"// Input uniform values\n"
"uniform mat4 mvp;\n"
"uniform mat4 matModel;\n"
"uniform mat4 matNormal;\n"
"\n"
"// Output vertex attributes (to fragment shader)\n"
"out vec3 fragPosition;\n"
"out vec2 fragTexCoord;\n"
"out vec4 fragColor;\n"
"out vec3 fragNormal;\n"
"\n"
"// NOTE: Add here your custom variables\n"
"\n"
"void main()\n"
"{\n"
"    // Send vertex attributes to fragment shader\n"
"    fragPosition = vec3(matModel*vec4(vertexPosition, 1.0));\n"
"    fragTexCoord = vertexTexCoord;\n"
"    fragColor = vertexColor;\n"
"    fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));\n"
"\n"
"    // Calculate final vertex position\n"
"    gl_Position = mvp*vec4(vertexPosition, 1.0);\n"
"}";


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#define MAX_LIGHTS  4         // Max dynamic lights supported by shader

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

// Light data
typedef struct {
    int type;
    bool enabled;
    Vector3 position;
    Vector3 target;
    Color color;
    float attenuation;

    // Shader locations
    int enabledLoc;
    int typeLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int attenuationLoc;
} Light;

// Light type
typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT
} LightType;


//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);   // Create a light and get shader locations
void UpdateLightValues(Shader shader, Light light);         // Send light properties to shader




/***********************************************************************************
*
*   RLIGHTS IMPLEMENTATION
*
************************************************************************************/

//#if defined(RLIGHTS_IMPLEMENTATION)


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static int lightsCount = 0;    // Current amount of created lights

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Create a light and get shader locations
Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    Light light = { 0 };

    if (lightsCount < MAX_LIGHTS)
    {
        light.enabled = true;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color = color;

        // NOTE: Lighting shader naming must be the provided ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightsCount));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightsCount));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightsCount));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightsCount));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightsCount));

        UpdateLightValues(shader, light);

        lightsCount++;
    }

    return light;
}

// Send light properties to shader
// NOTE: Light shader locations should be available
void UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255,
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}
