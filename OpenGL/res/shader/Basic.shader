#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float a_texIndex;

out vec2 v_TexCoord;
out vec4 v_Color;
out float v_TexIndex;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Color = a_Color;
	v_TexIndex = a_texIndex;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec4 v_Color;
in float v_TexIndex;

uniform vec4 u_Color;
uniform sampler2D u_Textures[3];

void main()
{
	int index = int(v_TexIndex);
	vec4 texColor = texture(u_Textures[index], v_TexCoord);
	color = texColor;
};