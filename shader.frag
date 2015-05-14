#version 330

uniform vec3 frag_color;

in vec2 varying_uv;
uniform sampler2D base_texture;

out vec4 finalColor;

void main()
{
    //finalColor = vec4(frag_color, 1.0);

    finalColor = texture2D(base_texture, varying_uv);
}
