#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
    // * vec4(aPos * zscale + offset, 1.0));

uniform mat4 models[64];
uniform vec3 colors[6*64];
uniform float opacity[64];
//uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float iTime;
out vec3 fColor;
out float alpha;
uniform  float scale = 20;
uniform  float zscale = 10;

void main()
{
    //float z = sin(offsets[gl_InstanceID][0] /scale*10) * cos (offsets[gl_InstanceID][1]/scale*10)*zscale;
    mat4 model = models[gl_InstanceID];
    //npos.z = (cos(npos.x/scale) * sin(npos.y / scale)) * zscale;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	
    fColor = colors[gl_InstanceID *6];
    //if aNormal = vec3( 0.0,  0.0, 1.0) fColor = colors[0];
    if (aNormal == vec3( 0.0,  0.0,-1.0)) fColor = colors[gl_InstanceID *6 + 1];
    if (aNormal == vec3( 0.0,  1.0, 0.0)) fColor = colors[gl_InstanceID *6 + 2];
    if (aNormal == vec3( 0.0, -1.0, 0.0)) fColor = colors[gl_InstanceID *6 + 3];
    if (aNormal == vec3( 1.0,  0.0, 0.0)) fColor = colors[gl_InstanceID *6 + 4];
    if (aNormal == vec3(-1.0,  0.0, 0.0)) fColor = colors[gl_InstanceID *6 + 5];
    alpha = opacity[gl_InstanceID];

}  
