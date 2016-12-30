//NOTE !!
//POSITION and COLOR are defined in *.cpp files


cbuffer ConstantBuffer
{
	float3 OffsetAnotherValue;
};

struct VOut 
{
	float4 position : SV_POSITION;
	float4 color : COLOR; 
};

VOut main( float4 position : POSITION, float4 color: COLOR )
{
	VOut output;
	
	output.position    = position;
	output.position.x += OffsetAnotherValue.x;
	output.position.y += OffsetAnotherValue.y;
	output.position.xy *= OffsetAnotherValue.z;

	output.color = color;

	//EXAMPLES OF SHADERS EFFECTS !!!

	//ex1 - invert colors
	//output.color = 1.0f - color;

	//ex2: hot colors
	//output.color = color; 
	//output.color.r = 1.0f;

	//ex3. shrink effect
	//output.position.xy *= 0.7f; //shrink the vertices on the X and the Y axes
	//output.color = color;

	return output;
}


/* HLSL

========================================================
Types of variables in Hight Level Shader Language (HLSL)
========================================================

1. Scalar (bool , float, uint, ...)
GPU are formated to use float variables

2. Vector
float4 ex: float4  = {0.1f,0.1f,0.1f,0.1f }

3. Matrix
float4x4 temp


========
Symatics
========

COLOR		( float 4 value that stores color )
POSITION	( float 4 value that stores position. Used to denote the position of vertices, usually in 3D space )
SV_POSITION ( float 4 value that stores position. Used to denote the position of vertices, in normalized screen coordinates not world space coordinates )
SV_TARGET   ( float 4 value tells the Output-Merger stage to draw the given color to render target)



==========
FUNCTIONS
==========
mul (a, b)			//	multiplies a and b and returns the result. Wish to multiply matrix variables with other values
pow(base, exponent) //	returns the base reaides to the given exponent
tan(A)				//	tangent in radianst
sin(A)				//	sine in radians
cos(A)				//	consine in rads
sqrt(A)
round(A)	
saturate(A)			//forces 'a' to be between 0 and 1. returns the new value and keeps the original unchanged
degrees(a)			// converts 'a' from radians to degrees
radians(a)
min(a, b)
max(a,b)

*/