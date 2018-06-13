#shader vertex
#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;
out vec2 uv;
out vec3 normal;
out vec3 lightdir;
out vec2 pos;
out vec3 eyedir;
uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_V;
void main(){
	pos = vPos.xz;	gl_Position =  u_MVP * vec4(vPos,1);
	uv = uv_coords;

	vec3 LightPosition_worldspace = vec3(-10.0f,40.0f,-10.0f);
	vec3 Position_worldspace = (u_M * vec4(vPos,1)).xyz;

	vec3 vertexPosition_cameraspace = ( u_V * u_M * vec4(vPos,1)).xyz;
	vec3 eyedir = vec3(0,0,0) - vertexPosition_cameraspace;
	vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
	lightdir = LightPosition_cameraspace + eyedir;

	normal = ( u_V * u_M * vec4(normals,0)).xyz;

}
#shader fragment
#version 330 core
layout (location = 0) out vec4 color;
in vec2 v_TexCoord;
in vec2 uv;
in vec3 normal;
in vec3 lightdir;
in vec2 pos;
in vec3 eyedir;
uniform float u_Ni;
uniform sampler2D u_KdMap;
uniform vec3 u_Ks;
uniform vec3 u_Ka;
uniform float u_D;
void main(){
  	vec3 n = normalize( normal );  	
  	vec3 l = normalize( lightdir );
  	vec3 e = normalize(eyedir);
  	vec3 r = reflect(-l,n);
	float cost = clamp( dot( n,l ), 0,1 );
  	float cosAlpha = clamp( dot( e,r ), 0,1 );

	vec3 lightColor = vec3(0.8f);
	vec3 u_Kd = texture( u_KdMap, uv).rgb;
	color = vec4(u_Ka * u_Kd + vec3(u_D) * u_Kd * lightColor * vec3(u_Ni) * cost + vec3(u_D) * u_Ks * lightColor * vec3(u_Ni) * pow(cosAlpha, 5), 1.0f);
}
