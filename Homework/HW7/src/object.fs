#version 330 core

in VS_OUT {
  vec3 FragPos;
  vec2 TexCoord;
  vec3 Color;
  vec3 Normal;	
  vec4 FragPosLightSpace;
} fs_in;

out vec4 FragColor;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, float bias) {
  vec3 projCoord = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoord = projCoord * 0.5 + 0.5;
  float currentDepth = projCoord.z;

  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
  for (int i = -1; i <= 1; ++i) {
  	  for (int j = -1; j <= 1; ++j) {
	  	  float pcfDepth = texture(shadowMap, projCoord.xy + vec2(i, j) * texelSize).r;
		  shadow += (currentDepth > 1.0 || currentDepth - pcfDepth < bias) ? 0.0f : 1.0f;
	  }
  }

  return shadow / 9.0;
}

void main() {
  vec3 color = texture(diffuseTexture, fs_in.TexCoord).rgb;
  vec3 normal = normalize(fs_in.Normal);
  vec3 lightColor = vec3(1.0);

  vec3 ambient = 0.2 * lightColor;

  vec3 lightDir = normalize(lightPos - fs_in.FragPos);
  float diff = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = 0.8 * diff * lightColor;

  vec3 viewDir = normalize(viewPos - fs_in.FragPos);
  vec3 reflectDir = reflect(-lightDir, normal);
  vec3 halfwayDir = normalize(lightDir + viewDir);
  float spec = pow(max(dot(normal, halfwayDir), 0.0f), 64.0);
  vec3 specular = 0.8 * spec * lightColor;
  
  float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
  float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);
  vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular));
  FragColor = vec4(lighting, 1.0) * vec4(fs_in.Color * color, 1.0);
}