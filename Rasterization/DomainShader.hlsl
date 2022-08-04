struct DomainShaderOutput
{
	float4 position : SV_POSITION;
	float4 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
	float4 posLight : LIGHTPOS;
};

struct HS_CONTROL_POINT_OUTPUT
{
	float3 vPosition : WORLDPOS; 
};

struct HS_CONSTANT_DATA_OUTPUT
{
	float EdgeTessFactor[3]			: SV_TessFactor;
	float InsideTessFactor			: SV_InsideTessFactor;
};

struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float4 worldPos : WORLD_POS;
	float3 normal : NORMAL;
	float2 uv : UV;
	float4 posLight : LIGHTPOS;
};

[domain("tri")]
DomainShaderOutput main(HS_CONSTANT_DATA_OUTPUT input, float3 uvw : SV_DomainLocation,
		const OutputPatch<VertexShaderOutput, 3> patch)
{
	DomainShaderOutput output;
	
	output.position = patch[0].position * uvw.x + patch[1].position * uvw.y + patch[2].position * uvw.z;
	output.uv = patch[0].uv * uvw.x + patch[1].uv * uvw.y + patch[2].uv * uvw.z;
	output.normal = patch[0].normal * uvw.x + patch[1].normal * uvw.y + patch[2].normal * uvw.z;
	output.posLight = patch[0].posLight * uvw.x + patch[1].posLight * uvw.y + patch[2].posLight * uvw.z;
	return output;
}