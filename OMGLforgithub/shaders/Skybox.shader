#shader vertex
#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vPos;

out vec3 pos;
  
// Values that stay constant for the whole mesh.
uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_V;
  
void main(){
  pos = vPos;
  vec4 pos1 = u_MVP * vec4(vPos,1);
  gl_Position =  pos1.xyww;

}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec3 pos;

uniform samplerCube u_SkyboxSampler;

void main(){
  color = vec4(texture(u_SkyboxSampler, pos).rgb,1.0f);
}
