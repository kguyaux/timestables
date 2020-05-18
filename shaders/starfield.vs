#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in mat4 model;

uniform mat4 view;
uniform mat4 projection;
uniform float iTime;

out vec3 fColor;

const vec3 fColors[14] = vec3[](
    vec3(0.792157, 0.792157, 0.792157),
    vec3(1, 1, 1),
    vec3(0.717647, 0.368627, 0.317647),
    vec3(0.85098, 0.392157, 0.34902),
    vec3(0.996078, 0.529412, 0.486275),
    vec3(0.792157, 0.756863, 0.368627),
    vec3(0.866667, 0.807843, 0.521569),
    vec3(0.235294, 0.627451, 0.258824),
    vec3(0.25098, 0.713726, 0.290196),
    vec3(0.45098, 0.807843, 0.486275),
    vec3(0.34902, 0.333333, 0.87451),
    vec3(0.494118, 0.458824, 0.941176),
    vec3(0.392157, 0.854902, 0.933333),
    vec3(0.709804, 0.396078, 0.701961)
);

void main()
{
	//mat4 jaja = translate(model, vec3(0,0,0));
	vec4 pos = projection * view * model * vec4(aPos, 1.0);
	float darkmatter = max(0.0, min(1.0, (5 - pos.z)));

    fColor = fColors[int(mod(gl_InstanceID, 14))] * darkmatter;
    gl_Position = pos;
}  
