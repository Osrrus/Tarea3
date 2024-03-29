#version 330 core

in vec3 offsetPos;

uniform sampler3D texVolume;
uniform sampler2D texRender;
uniform vec2 wSize;

out vec4 FragColor;

void main(){

    /*vec4 endColor = vec4(0.0f,0.0f,0.0f,1.0f);
    vec4  col=vec4(0.0, 0.0, 0.0, 0.0);

    vec2 coord = (gl_FragCoord.xy)/wSize;
    vec3 rayDir = vec3(texture(texRender,coord).xyz - offsetPos); 
    vec3 rayStar = offsetPos;

    float maxStep = length(rayDir);
    rayDir = normalize(rayDir);
    float step = 1.0f/256.0f;

    vec3 stepRay;
    float alpha = 1.0f;

    for(float i = 0.0f; i < 256 ; i++){

        col = texture(texVolume,rayStar).r * vec4(texture(texVolume,rayStar).r);
        col.rgb *= col.a;

        endColor.rgb += col.rgb * alpha; 
        alpha *= 1.0 - col.a;

        if(1.0f-endColor.a >= 0.99f)break;
        rayStar += rayDir*step; 

        if(rayStar.x > 1.0 || rayStar.y > 1.0 || rayStar.z > 1.0) break;
		if(rayStar.x < 0.0 || rayStar.y < 0.0 || rayStar.z < 0.0) break;
    }

    endColor.a = 1.0f;
    FragColor = endColor;*/

    vec4 endColor = vec4(0.0f,0.0f,0.0f,1.0f);
    vec3 rayStep;

	vec2 coord = (gl_FragCoord.xy) / wSize;
	vec3 rayDir = vec3(texture(texRender,coord).xyz - offsetPos);

	vec3 rayStar = offsetPos;
	float max = length(rayDir);
    
	rayDir = normalize(rayDir);
    float step=1.0f/255;

	for(float i=0.0f;i<max;i+=step){

		rayStep= texture(texVolume,rayStar).r * vec3(texture(texVolume,rayStar).r);
		rayStep *=endColor.a;

        endColor.rgb +=rayStep;
		endColor.a *= 1 - texture(texVolume,rayStar).r;

		if(1-endColor.a >= 0.99f) break;

		rayStar += rayDir*step;
	}

	endColor.a = 1.0f;
	FragColor = endColor;
    
}