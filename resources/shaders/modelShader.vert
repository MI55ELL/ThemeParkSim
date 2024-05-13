#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    // stores texture coordinates of the thing we are storing
    TexCoords = aTexCoords;    
    // model stores position and rotation of the model we are storing 
    gl_Position = camMatrix * model * vec4(aPos, 1.0);
}