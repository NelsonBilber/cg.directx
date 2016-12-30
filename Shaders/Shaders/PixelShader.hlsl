float4 main(float4 position: SV_POSITION, float4 color: COLOR) : SV_TARGET
{
	color.r = 1.0f;
	

	return color;
}
