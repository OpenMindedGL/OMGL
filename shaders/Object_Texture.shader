#shader vertex
#version 330 core

layout(location = 0) in vec4 position; 
layout(location = 1) in vec2 textCoord; 

out vec2 v_TextCoord;

void main()
{
	gl_Positions = position; 
	v_TextCoord;	
};

#shader fragment
#version 330 core

layout location = 0) out vec4 color; 

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texturte(u_Texture, textCoords); 
	color = texColor;
}