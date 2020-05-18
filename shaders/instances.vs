#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

//uniform vec3 offsets[768];
uniform float heightscale;
uniform float density;
uniform vec3 inColor[768];
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
out vec3 fColor;

void main()
{
	vec3 coll = inColor[gl_InstanceID];
    float x = float(gl_InstanceID % 32)/32.0;
    float y = float(gl_InstanceID / 32)/24.0;
	float bw = (coll.x + coll.y, coll.z) / 3.0;
    vec3 aP = aPos + vec3(0.0, 0.0, 0.5);
	vec3 offset = (vec3(x, -y, 0.0) - vec3(0.5, -0.5, 0.0)) * density;
    vec3 zscale = vec3(1.0, 1.0, pow((bw * (1.0 + heightscale)/2.0 * 100.0), 2.0));
    gl_Position = projection * view * model * vec4(aP * zscale + offset, 1.0);
    // * vec4(aPos * zscale + offset, 1.0));
	fColor = coll; 
}  
