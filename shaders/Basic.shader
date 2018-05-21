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
        if(0.01f  > mod(pos.y,31.0f) || 0.01f  > mod(pos.x,31.0f)){
          color = vec4(0.0f,1.0f,0.0f,1.0f);
        }
        else{
          color = vec4(0.0f,0.0f,1.0f,1.0f);
        }
	//color = vec4(0.0f,mod(pos.y,2.0f),mod(pos.x,2.0f),1.0f);
};
