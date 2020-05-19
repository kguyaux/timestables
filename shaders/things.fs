#version 330 core

layout (location = 0) out vec4 FragColor;
  
in vec3 fColor;
in float alpha;


void main()
{
    FragColor = vec4(fColor, alpha);
}
