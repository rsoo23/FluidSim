#version 460 core
out vec4 FragColor;  

in vec2 TexCoord;

uniform sampler2D ourTexture;
  
void main()
{
	float d = texture(ourTexture, TexCoord).r;
	d = clamp(d, 0.0, 1.0);

	// fire palette: black → red → orange → yellow → white
	vec3 col = vec3(
		smoothstep(0.0, 0.4, d),
		smoothstep(0.3, 0.7, d),
		smoothstep(0.7, 1.0, d)
	);
	FragColor = vec4(col, 1.0);
}