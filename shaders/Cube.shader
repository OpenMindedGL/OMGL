#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uvcoords;

uniform mat4 u_MVP;
uniform mat4 u_V;
uniform mat4 u_M;

out vec2 uv;

void main()
{
	gl_Position = u_MVP * position;
  uv = uvcoords;
}

#shader fragment
#version 330 core

uniform sampler2D u_TextureSampler;

layout(location = 0) out vec4 color;
//layout(location = 0) out vec3 color;

in vec2 uv;

void main()
{
  //color = texture(u_TextureSampler, uv).rgb;
  color = vec4(texture(u_TextureSampler, uv).rgb,1.0f);
  //color = vec4(vec3((float)u_TextureSampler,1.0f,1.0f),1.0f);
}