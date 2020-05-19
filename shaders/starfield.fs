#version 330 core

layout (location = 0) out vec4 FragColor;
  
in vec3 fColor;
uniform float transpfactor = 1.0;


void main()
{
	
    FragColor = vec4(fColor, 1.0);
}
