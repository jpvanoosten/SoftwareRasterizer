R"(
#version 330 core

in vec2 uv;
uniform sampler2D tex;
out vec4 color;

void main()
{
    color = textureLod(tex, uv, 0).bgra;
    // color = vec4(uv, 0, 0);
}
)"