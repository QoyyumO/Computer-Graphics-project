#version 330 core

// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Texture Coordinates
layout (location = 1) in vec2 aTex;

// Outputs the texture coordinates to the fragment shader
out vec2 texCoord;

// Imports the camera matrix from the main function
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Outputs the positions/coordinates of all vertices
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // Assigns the texture coordinates from the Vertex Data to "texCoord"
    texCoord = aTex;
}
