#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

uniform mat4 PVMMatrix;

in VS_GS_VERTEX
{
	in vec4 Position;
} gs_in[];	// variable name array (local to this shader)


out vec2 FragTexCoords;


// Pentagon vertices
vec4 topLeftPoint = vec4(-0.228f, 0.3f, 0.0f, 1.0f);
vec4 topRightPoint = vec4(0.2275f, 0.31f, 0.0f, 1.0f);
vec4 middleLeftPoint = vec4(-0.365f, -0.12f, 0.0f, 1.0f);
vec4 middleRightPoint = vec4(0.3625f, -0.12f, 0.0f, 1.0f);
vec4 bottomCentrePoint = vec4(0.0f, -0.38f, 0.0f, 1.0f);


void MakePentagon()
{
	gl_Position = gs_in[0].Position + PVMMatrix * topLeftPoint;			// T1: Top left point
	FragTexCoords = vec2(0.375f, 0.625f);
	EmitVertex();																			 
	gl_Position = gs_in[0].Position + PVMMatrix * middleLeftPoint;		// T1: Middle left point
	FragTexCoords = vec2(0.3f, 0.375f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * bottomCentrePoint;	// T1: Bottom centre point
	FragTexCoords = vec2(0.5f, 0.25f);
	EmitVertex();

	EndPrimitive();

	gl_Position = gs_in[0].Position + PVMMatrix * topLeftPoint;			// T2: Top left point
	FragTexCoords = vec2(0.375f, 0.625f);
	EmitVertex();																			 
	gl_Position = gs_in[0].Position + PVMMatrix * bottomCentrePoint;	// T2: Bottom centre point
	FragTexCoords = vec2(0.5f, 0.25f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * middleRightPoint;		// T2: Middle right point
	FragTexCoords = vec2(0.7f, 0.375f);
	EmitVertex();

	EndPrimitive();

	gl_Position = gs_in[0].Position + PVMMatrix * topLeftPoint;			// T3: Top left point
	FragTexCoords = vec2(0.375f, 0.625f);
	EmitVertex();																			 
	gl_Position = gs_in[0].Position + PVMMatrix * middleRightPoint;		// T3: Middle right point
	FragTexCoords = vec2(0.7f, 0.375f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * topRightPoint;		// T3: Top right point
	FragTexCoords = vec2(0.625f, 0.625f);
	EmitVertex();

	EndPrimitive();
}

void MakeOuterTriangles()
{
	// Top triangle //												
	gl_Position = gs_in[0].Position + PVMMatrix * vec4(0.0f, 1.0f, 0.0f, 1.0f);
	FragTexCoords = vec2(0.5f, 1.0f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * topLeftPoint;	
	FragTexCoords = vec2(0.375f, 0.625f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * topRightPoint;
	FragTexCoords = vec2(0.625f, 0.625f);
	EmitVertex();

	EndPrimitive();

	// Right triangle //
	gl_Position = gs_in[0].Position + PVMMatrix * vec4(0.945f, 0.3f, 0.0f, 1.0f);
	FragTexCoords = vec2(0.875f, 0.625f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * topRightPoint;	
	FragTexCoords = vec2(0.625f, 0.625f);
	EmitVertex();												
	gl_Position = gs_in[0].Position + PVMMatrix * middleRightPoint;
	FragTexCoords = vec2(0.7f, 0.375f);
	EmitVertex();

	EndPrimitive();

	// Bottom-Right triangle //
	gl_Position = gs_in[0].Position + PVMMatrix * vec4(0.58f, -0.8f, 0.0f, 1.0f);
	FragTexCoords = vec2(0.75f, 0.125f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * middleRightPoint;	
	FragTexCoords = vec2(0.7f, 0.375f);
	EmitVertex();												
	gl_Position = gs_in[0].Position + PVMMatrix * bottomCentrePoint;
	FragTexCoords = vec2(0.5f, 0.25f);
	EmitVertex();

	EndPrimitive();

	// Bottom-Left triangle //
	gl_Position = gs_in[0].Position + PVMMatrix * vec4(-0.58f, -0.8f, 0.0f, 1.0f);
	FragTexCoords = vec2(0.25f, 0.125f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * bottomCentrePoint;
	FragTexCoords = vec2(0.5f, 0.25f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * middleLeftPoint;	
	FragTexCoords = vec2(0.3f, 0.375f);
	EmitVertex();												

	EndPrimitive();

	// Left triangle //
	gl_Position = gs_in[0].Position + PVMMatrix * vec4(-0.945f, 0.3f, 0.0f, 1.0f);
	FragTexCoords = vec2(0.125f, 0.625f);
	EmitVertex();
	gl_Position = gs_in[0].Position + PVMMatrix * middleLeftPoint;	
	FragTexCoords = vec2(0.3f, 0.375f);
	EmitVertex();												
	gl_Position = gs_in[0].Position + PVMMatrix * topLeftPoint;
	FragTexCoords = vec2(0.375f, 0.625f);
	EmitVertex();

	EndPrimitive();
}

void main()
{
	// Pentagon
	MakePentagon();

	// Star 'limbs' (outer triangles)
	MakeOuterTriangles();

}