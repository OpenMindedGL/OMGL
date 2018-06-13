#shader vertex
#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;

out vec2 uv;
/*out vec3 normal;
out vec3 lightdir;
out vec2 pos;
out vec3 eyedir;*/
  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_V;


void main()
{
	//pos = vPos.xz;
	gl_Position =  u_MVP * vec4(vPos,1);
	uv = uv_coords;

	//vec3 LightPosition_worldspace = vec3(-10.0f,40.0f,-10.0f);
	//vec3 Position_worldspace = (u_M * vec4(vPos,1)).xyz;

	//vec3 vertexPosition_cameraspace = ( u_V * u_M * vec4(vPos,1)).xyz;
	//vec3 eyedir = vec3(0,0,0) - vertexPosition_cameraspace;
	//vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
	//lightdir = LightPosition_cameraspace + eyedir;

	//normal = ( u_V * u_M * vec4(normals,0)).xyz;
}

#shader fragment
#version 330 core

layout (location = 0) out vec4 color; 

in vec2 v_TexCoord;
in vec2 uv;
//in vec3 normal;
//in vec3 lightdir;
//in vec2 pos;
//in vec3 eyedir;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor =  texture(u_Texture, uv);
	color = texColor;
}