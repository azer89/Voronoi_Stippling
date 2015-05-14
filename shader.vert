#version 330 

in vec3 vert;
in vec2 uv;
out vec2 varying_uv;
uniform mat4 mvpMatrix;

void main()
{	
    gl_Position = mvpMatrix * vec4(vert, 1.0);
    varying_uv = uv;
}
