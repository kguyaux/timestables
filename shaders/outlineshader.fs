#version 330 core

layout (location = 0) out vec4 FragColor;
  
in vec3 fColor;
in float alpha;
uniform vec3 outlinecolor;

void main()
{
    FragColor = vec4(outlinecolor,1);
}
