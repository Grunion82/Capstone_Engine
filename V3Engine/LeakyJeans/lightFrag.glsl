#version 330
in vec2 texCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light{
	vec3 position;
	vec3 color;
	
	float linear;
	float quadratic;
	float radius;
};

const int numLights = 4;
uniform vec3 viewPos;
uniform Light lights[numLights];

out vec4 fragColor;

void main() { 
		vec3 FragPos = texture(gPosition,texCoords).rgb;
		vec3 Normal = texture(gNormal,texCoords).rgb;
		vec3 Diffuse = texture(gAlbedoSpec,texCoords).rgb;
		float Specular = texture(gAlbedoSpec,texCoords).a;
		
		vec3 lighting = Diffuse * 0.1;
		vec3 viewDir = normalize(viewPos - FragPos);
		for(int i = 0; i < numLights; i++){
			float dist = length(lights[i].position - FragPos);
			if(dist < lights[i].radius){
				//Diffuse 
				vec3 lightDir = normalize(lights[i].position - FragPos);
				vec3 diffuse = max(dot(Normal,lightDir),0.0) * Diffuse * lights[i].color;
				
				//Specular
				vec3 halfway = normalize(lightDir + viewDir);
				float spec = pow(max(dot(Normal,halfway),0.0),16.0);
				vec3 specular = lights[i].color * spec * Specular;
				
				//Attenuation
				float attenuation = 1.0 / (1.0 + lights[i].linear * dist + lights[i].quadratic * dist * dist);
				diffuse *= attenuation;
				specular *= attenuation;
				lighting += diffuse + specular;
			}
		}
		
		fragColor = vec4(lighting,1.0);
} 