#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform bool isPlane;

void main()
{
if(isPlane){
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
else{
        FragColor = texture(texture1, TexCoord);
    }
}
