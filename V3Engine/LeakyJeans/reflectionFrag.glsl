#version 330
in  vec3 eyeDir;
in  vec3 vertNormal;
in  vec3 lightDir;

out vec4 fragColor;

uniform samplerCube cubeTexture; 
uniform mat4 modelViewMatrix; 

void main() { 
	mat3 scale = mat3(vec3(-1.0,0.0,0.0),
					  vec3(0.0,1.0,0.0),
					  vec3(0.0,0.0,1.0));

	vec3 reflection = reflect(eyeDir, vertNormal);
	vec3 refraction = refract (eyeDir, vertNormal, 1.0f/2.65f);
	reflection = vec3 (inverse (modelViewMatrix) * vec4 (reflection, 0.0f));
	refraction = vec3 (inverse (modelViewMatrix) * vec4 (refraction, 0.0f));

	vec3 mixture = mix(refraction, reflection, max(0.0f, dot(vertNormal, eyeDir)));

	vec3 halfWayVec = normalize(lightDir + eyeDir);
	float diff = max(0.0f, dot(vertNormal, lightDir));
	float spec = 0.0f;
	if(diff < 0.0f) {	
		spec = max(0.0, dot(vertNormal, halfWayVec));
		spec = pow(spec, 32.0f);
	}
	vec4 kd = texture(cubeTexture, scale * mixture);
	vec4 ks = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	vec4 ka = 0.1f * kd;

	vec4 phongMix =  ka + (diff * kd) + (spec * ks);

	fragColor = phongMix;//texture(cubeTexture, scale * mixture);
}