#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D bgtexture;
//uniform sampler2D fgtexture;


void main()
{
    /*
    vec4 fgColor = vec4(texture(fgtexture, texCoord));
    if (fgColor.a > 0.0) {
        FragColor = fgColor;
    } else {
        FragColor = vec4(texture(bgtexture, texCoord));
    }*/
    FragColor = vec4(texture(bgtexture, texCoord));


}
