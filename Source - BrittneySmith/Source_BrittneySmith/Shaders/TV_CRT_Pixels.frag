// TV CRT pattern 
// Created by fragcula in 2023-01-08
// https://www.shadertoy.com/view/dlfGWs

#version 460 core

// Vertex Shader Inputs
in vec3 FragColor;
in vec2 FragTexCoords;

// Uniform Inputs
uniform sampler2D ImageTexture;

// Output
out vec4 FinalColor;

void main()
{
	vec2 uv = FragTexCoords;//iResolution.xy;

    vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
    vec4 green = vec4(0.0, 1.0, 0.0, 1.0);
    vec4 blue = vec4(0.0, 0.0, 1.0, 1.0);
    vec4 black = vec4(0.0, 0.0, 0.0, 1.0);
    
    // Reduce resolution for visibility, PAL was720x576.
    float h_res = 720.0/5.0;
    float v_res = 576.0/5.0;
    
    float colN = floor(uv.x * h_res);
    bool even = mod(colN, 2.0) >= 0.999;
    float s = mod(uv.x * h_res * 10.0, 10.0);
    vec4 pxl;
    if (s <= 1.01)
        pxl = black;
    else if (s <= 3.01)
        pxl = red;
    else if (s <= 4.01)
        pxl = black;
    else if (s <= 6.01)
        pxl = green;
    else if (s <= 7.01)
        pxl = black;
    else if (s <= 9.01)
        pxl = blue;


    FinalColor = (mod(uv.y*(even? v_res : v_res + 0.5)*10.0, 10.0) <= 2.5) ? black : pxl * texture(ImageTexture, uv);
}