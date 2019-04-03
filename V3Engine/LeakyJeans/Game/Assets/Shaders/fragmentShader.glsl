#version 330 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

out vec4 color;

uniform sampler2D textureMap;
uniform vec3 viewPosition;

void main() {
	//ambient
	//vec3 ambient = material.ambient * texture(material.diffuseMap, TexCoords).rgb *	light.lightColor;

	//diffuse
	//vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.lightPos - FragPosition);
	//float diff = max(dot(norm, lightDir), 0.0);
	//vec3 diffuse = (diff * material.diffuse) * texture(material.diffuseMap,	TexCoords).rgb * light.lightColor;

	//specular
	//vec3 viewDir = normalize(viewPosition - FragPosition);
	//vec3 reflectDir = reflect(-lightDir, norm);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	//vec3 specular = (spec * material.specular) * light.lightColor;

	//vec3 result = ambient + diffuse + specular;
	color = texture(textureMap, TexCoords);
}