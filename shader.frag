#version 330

in vec2 varying_uv;
uniform float use_color;
uniform sampler2D base_texture;
uniform vec3 frag_color;
out vec4 finalColor;

void main()
{
    finalColor = texture2D(base_texture, varying_uv);
    if(use_color > 0.5)
    {
     	finalColor = vec4(frag_color, 1.0);
    }
}
