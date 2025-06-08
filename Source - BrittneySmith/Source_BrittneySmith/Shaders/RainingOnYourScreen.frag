#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D StaticImage;
uniform sampler2D ImageTexture;
uniform float time;

// Output
out vec4 FinalColor;

void main()
{
	vec2 uv = FragTexCoords.xy; // iResolution.xy;
	vec3 raintex = texture(StaticImage, vec2(uv.x * 2.0, uv.y * 0.1 + time * 0.125)).rgb / 8.0;
	vec2 where = (uv.xy - raintex.xy);
	vec3 texchur1 = texture(ImageTexture, where).rgb;
	
	FinalColor = vec4(texchur1, 1.0);
}