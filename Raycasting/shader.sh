#shader vertex
#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
out vec3 v_color;

void main() {
	gl_Position = vec4(position, 0, 1);
	v_color = color;
};


#shader fragment
#version 330 core
layout(location = 0) out vec4 color;
in vec3 v_color;

void main() {
	color = vec4(v_color, 1);
};

