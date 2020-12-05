#shader vertex
#version 330 core

layout(location = 0) in vec2 position;

void main()
{
	gl_Position = vec4(position, 0, 1);
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

void main()
{
	color = vec4(0.7, 0.8, 1, 1);
};
