#version 330 core

layout (location = 0) out vec4 FragColor;
  
in vec3 fColor;
uniform float transpfactor = 1.0;


void main()
{
    float opacity = (fColor.r + fColor.g + fColor.b) / 3.0; 
    FragColor = vec4(fColor, transpfactor * opacity);
}
