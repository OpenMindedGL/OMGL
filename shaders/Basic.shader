#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;

out vec2 uv;
out vec3 normal;
out vec3 lightdir;
out vec2 pos;
  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_V;
  
void main(){
  pos = vPos.xz;
  gl_Position =  u_MVP * vec4(vPos,1);
  uv = uv_coords;

  vec3 LightPosition_worldspace = vec3(-10.0f,40.0f,-10.0f);
  vec3 Position_worldspace = (u_M * vec4(vPos,1)).xyz;

  vec3 vertexPosition_cameraspace = ( u_V * u_M * vec4(vPos,1)).xyz;
  vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

  vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
  lightdir = LightPosition_cameraspace + EyeDirection_cameraspace;

  normal = ( u_V * u_M * vec4(normals,0)).xyz; 


}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 uv;
in vec3 normal;
in vec3 lightdir;
in vec2 pos;

void main(){
  vec3 grey = vec3(0.6f,0.6f,0.6f);
  vec3 n = normalize( normal );
  vec3 l = normalize( lightdir );
  float cost = clamp( dot( n,l ), 0,1 );
  float ambient = 0.15f;
  color = vec4(vec3(ambient) + grey * cost,1.0f);
}
