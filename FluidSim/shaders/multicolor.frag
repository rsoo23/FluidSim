#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D tex;

void main()
{
	vec3 col = texture(tex, TexCoord).rgb;

	// reinhard tone mapping
	col = col / (col + 1.0);

	// gamma correction
	col = pow(col, vec3(1.0 / 2.2));

	FragColor = vec4(col, 1.0);
}
