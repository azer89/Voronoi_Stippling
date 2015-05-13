#version 330

uniform vec3 frag_color;

out vec4 finalColor;

void main()
{
    finalColor = vec4(frag_color, 1.0);
}
