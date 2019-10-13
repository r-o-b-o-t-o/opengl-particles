#version 330 core

layout (points) in;
layout (line_strip, max_vertices = 204) out; // max_vertices = 2 * max_size * 4 + 4

uniform int size;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    mat4 mvp = projection * view * model;

	for (int i = -size; i <= size; ++i) {
        gl_Position = gl_in[0].gl_Position + mvp * vec4(i, 0.0, -size, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + mvp * vec4(i, 0.0, size, 0.0);
        EmitVertex();
        EndPrimitive();

        gl_Position = gl_in[0].gl_Position + mvp * vec4(-size, 0.0, i, 0.0);
        EmitVertex();

        gl_Position = gl_in[0].gl_Position + mvp * vec4(size, 0.0, i, 0.0);
        EmitVertex();
        EndPrimitive();
	}
}
