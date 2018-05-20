#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

uniform mat4 u_MVP;
out vec2 pos;

void main()
{
        pos = position.xz;
	gl_Position = u_MVP * position;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;
in vec2 pos;

void main()
{
	color = vec4(0.0f,0.0f,1.0f,1.0f);
	//color = vec4(pos.x/4.0f,pos.y/4.0f,0.0f,1.0f);
};
