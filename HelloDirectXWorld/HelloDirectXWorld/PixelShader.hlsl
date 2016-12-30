float4 main() : SV_TARGET
{
	/* Example of function

	float4 PShader(float4 color: COLOR ) : SV_TARGET
	{

	}
	*/

	return float4(1.0f, 0.0f, 1.0f, 1.0f);
}

/* HLSL

Types of variables in Hight Level Shader Language (HLSL)
========================================================

1. Scalar (bool , float, uint, ...)
GPU are formated to use float variables

2. Vector
float4 ex: float4  = {0.1f,0.1f,0.1f,0.1f }

3. Matrix
float4x4 temp



Symatics
========

COLOR		( float 4 value that stores color )
POSITION	( float 4 value that stores position. Used to denote the position of vertices, usually in 3D space )
SV_POSITION ( float 4 value that stores position. Used to denote the position of vertices, in normalized screen coordinates not world space coordinates )
SV_TARGET   ( float 4 value tells the Output-Merger stage to draw the given color to render target)


*/