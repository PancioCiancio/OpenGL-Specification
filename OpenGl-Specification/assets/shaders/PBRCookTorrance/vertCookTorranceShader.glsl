#version 430

struct Material
{
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;
};

layout (std140, binding = 13) uniform LitProperties
{
	mat4 CamModelMat;
	mat4 CamViewMat;
	mat4 CamProjMat;
	vec4 CamPos;
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightAmbient;
};

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

// PBR maps
layout (binding = 0) uniform sampler2D uAlbedoMap;
layout (binding = 1) uniform sampler2D uAOMap;
layout (binding = 2) uniform sampler2D uHeightMap;
layout (binding = 3) uniform sampler2D uMetallicMap;
layout (binding = 4) uniform sampler2D uNormalMap;
layout (binding = 5) uniform sampler2D uRoughnessMap;

// Matrices parameteres
uniform mat4 uModel;
uniform float uTilingFactor;
uniform float uDisplacementFactor;
uniform Material uMaterial;

out vec3 Position;
out vec2 TexCoord;
out mat3 TBN;

void main()
{
	vec3 vertexDisplacement = aPosition + (aNormal * texture(uHeightMap, aTexCoord * uTilingFactor).r * uDisplacementFactor);
	Position = (uModel * vec4(vertexDisplacement, 1.0)).xyz;
	TexCoord = aTexCoord * uTilingFactor;

	vec3 T = normalize(vec3(uModel * vec4(aTangent, 0.0)));
	vec3 N = normalize(vec3(uModel * vec4(aNormal,	0.0)));
	T = normalize(T - dot(T, N) * N);
	vec3 B = cross(N, T);
	TBN = mat3(T, B, N);

	gl_Position = CamProjMat * CamViewMat * uModel * vec4(vertexDisplacement, 1.0);
}