#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 uvCoord;

out vec2 v_texCoord;

void main()
{
	gl_Position = vec4(position, 0, 1);
	v_texCoord = uvCoord;
};


#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
	color = texture(u_texture, v_texCoord);
	color[3] = 1;
};
