#version 330
out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube skyBox;

void main()
{
	fragColor = texture(skyBox, texCoords);
}