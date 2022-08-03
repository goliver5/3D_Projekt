Texture2D testTexture : register(t0);
SamplerState testSampler : register(s0);


struct PixelShaderInput
{
	float4 pos : SV_POSITION;
};

float4 main(PixelShaderInput input) : SV_TARGET
{
	return float4(1.0f,0.2f,1.0f, 1.0f);
	//float4 texColor = testTexture.Sample(testSampler, input.uv);
	//float4 lightColor = float4(0.1f, 0.1f, 0.1f, 0.0f);
	//float3 specularColor = (0.5f, 0.5f, 0.5f);
	//float shiny = 50.0f;

	//float3 lightMaterial = lightColor + texColor;
	////ambient
	//float3 amColor = texColor * lightColor;
	//float3 ambientLight = amColor * ambienceStrength;

	////diffuse light 
	////float3 norm = normalize(input.normal);
	////float3 vecBetweenWPosLight = normalize(lightPos - input.worldPos);
	////float diffuselight = clamp(dot(vecBetweenWPosLight, norm),0.0f,1.0f);
	////diffuselight = lightColor*texColor * diffuselight,;
	
	////diffuse
	//float3 norm = normalize(input.normal);
	//float3 vecBetweenWPosLight = normalize(lightPos - input.worldPos);
	//float diffuselight = clamp(dot(norm, vecBetweenWPosLight),0.0f,1.0f);// * lightColor;
	//float3 diffuselightClr = texColor * diffuselight;


	////specular light
	//float3 Veccamera = input.worldPos - lightPos;
	//Veccamera = normalize(Veccamera);
	//float3 reflectionray = reflect(-Veccamera, input.normal);
	//reflectionray = normalize(reflectionray);
	//float3 specular = pow(max(dot(normalize(reflectionray), Veccamera), 0), shiny);

	//float3 specClr = float3(0.8f,0.8f,0.8f) * specular;

	//float3 pixelColor = ambientLight + diffuselightClr + specClr;

	//return float4(pixelColor, 1.0f);
	//return float4(testTexture.Sample(testSampler, input.uv).xyz, 1.0f);
}