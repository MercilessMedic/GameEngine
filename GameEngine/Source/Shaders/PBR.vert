#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in ivec4 aBoneIDs;     
layout (location = 6) in vec4 aWeights;  

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix;

out vec2 TexCoord;
out vec3 Normal;
out vec3 WorldPos;
out mat3 TBN;


void main()
{
	WorldPos = vec3( model * vec4( aPos, 1.0 ));
	gl_Position = projection * view * vec4(WorldPos, 1.0);
	TexCoord = aTexCoord;
	
	vec3 N = normalize(normalMatrix * aNormal);
	vec3 T = normalize(normalMatrix * aTangent);

	//This is Gram–Schmidt method that makes sure T is
	//completely orthogonal to normal vector
	T = normalize(T - dot(T, N) * N);

	vec3 B = cross(N, T);

	TBN = mat3(T, B, N);
	Normal = N;
}