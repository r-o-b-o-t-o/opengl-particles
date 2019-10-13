#version 330 core
// Attributes with same values for all particles
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

// Instance attributes
layout (location = 2) in vec3 aInstancePos;
layout (location = 3) in vec4 aInstanceColor;
layout (location = 4) in vec2 aInstanceScale;

out vec4 color;
out vec2 texCoord;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vec2 aScale = vec2(1.0, 1.0);

    mat4 m = view;
    // First colunm
    m[0][0] = aInstanceScale.x;
    m[0][1] = 0.0;
    m[0][2] = 0.0;
    // Second column
    m[1][0] = 0.0;
    m[1][1] = aInstanceScale.y;
    m[1][2] = 0.0;
    // Third column
    m[2][0] = 0.0;
    m[2][1] = 0.0;
    m[2][2] = 1.0;

    vec4 p = m * vec4(aPos.xyz, 1.0);
    gl_Position = projection * (p + (view * vec4(aInstancePos.xyz, 1.0)));

    color = aInstanceColor;
    texCoord = aTexCoord;
}
