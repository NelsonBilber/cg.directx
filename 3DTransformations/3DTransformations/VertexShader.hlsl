cbuffer ConstantBuffer
{
	float4x4 matFinal;
};

struct VOut 
{
	float4 position : SV_POSITION;
	float4 color : COLOR; 
};

VOut main( float4 position : POSITION, float4 color: COLOR )
{
	VOut output;
	
	output.position  = mul(matFinal,position); // render the final 2d vector on the screen
	output.color	 = color;

	return output;
}
