#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec2 uv_coords;
layout(location = 2) in vec3 normals;

out vec2 uv;
//out vec3 normal;
out vec3 lightdir;
out mat4 MV;

out float d1;

  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_VP;
uniform mat4 u_M;
uniform mat4 u_V;
uniform sampler2D u_HeightMap;
uniform sampler2D u_NormalMap;
  
void main(){
  vec4 pos = u_M * vec4(vPos,1.0);
  uv = (pos.xz+4048)/8096;
  pos.y = texture(u_HeightMap, uv).r*1000;
  gl_Position =  u_VP * pos;

  MV = u_V * u_M;

  vec3 LightPosition_worldspace = vec3(10.0f,100.0f,10.0f);
  vec3 Position_worldspace = (u_M * vec4(vPos,1)).xyz;

  vec3 vertexPosition_cameraspace = ( MV * vec4(vPos,1)).xyz;
  vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;

  vec3 LightPosition_cameraspace = ( u_V * vec4(LightPosition_worldspace,1)).xyz;
  lightdir = LightPosition_cameraspace + EyeDirection_cameraspace;

  //uv = vPos.xz;

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in mat4 MV;
in vec2 uv;
//in vec3 normal;
in vec3 lightdir;
in vec2 pos;
in float d1;

uniform sampler2D u_NormalMap;
//out vec3 color;

void main(){
/*  if(0.01f  > mod(pos.y,31.0f) || 0.01f  > mod(pos.x,31.0f)){
    color = vec4(0.0f,1.0f,0.0f,1.0f);
  }
  else{
    color = vec4(0.0f,0.0f,1.0f,1.0f);
  */
  //color = vec4(0.0f,mod(pos.y,2.0f),mod(pos.x,2.0f),1.0f);
  vec3 blue = vec3(0.2f,0.6f,0.2f);

  vec3 norm = texture(u_NormalMap, uv).rgb;

  vec3 normal = ( MV * vec4(norm,0)).xyz; 

  vec3 n = normalize( normal );
  vec3 l = normalize( lightdir );
  float cost = clamp( dot( n,l ), 0,1 );
  float ambient = 0.15f;
  color = vec4(vec3(ambient) + blue * cost,1.0f);

  //color = vec4(uv.x/2,0.0,uv.y,1.0f);

 // color = texture(u_NormalMap, uv/16);
 // }
}
