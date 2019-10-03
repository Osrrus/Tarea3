#version 330 core

in vec3 offsetPos;

uniform sampler3D texVolume;
uniform sampler2D texRender;
uniform vec2 wSize;

out vec4 FragColor;

void main(){

    vec4 endColor = vec4(0.0f,0.0f,0.0f,1.0f);
    vec2 coord = (gl_FragCoord.xy)/wSize;
    vec3 rayDir = vec3(texture(texRender,coord).xyz - offsetPos); 
    vec3 rayStar = offsetPos;
    float maxStep = length(rayDir);
    rayDir = normalize(rayDir);
    float step = 1.0f/256;
    vec3 stepRay;

    for(float i = 0.0f; i < maxStep ; i+=step){

        stepRay = texture(texVolume,rayStar).r * vec3(texture(texVolume,rayStar).r);
        stepRay *= endColor.a;

        endColor.rgb += stepRay; 
        endColor.a *= 1 - texture(texVolume,rayStar).r;

        if(1-endColor.a >= 0.99f)break;
        rayStar += rayDir*step; 
    }

    endColor.a = 1.0f;
    FragColor = endColor;

}