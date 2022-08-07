RWTexture2D <unorm float4> backBuffer : register(u0);
Texture2D textureColor : register(t0);
Texture2D normal : register(t1);
Texture2D position : register(t2);
Texture2D DiffuseMap : register(t3);
Texture2D AmbientMap : register(t4);
Texture2D SpecularMap : register(t5);


cbuffer Spotlight1 : register(b0)
{
	float3 positionSpotlight;
	float range;
	float3 direction;
	float cone;
	float3 color;
	float pad;
	float3 attenuation;
	float pad2;
};

cbuffer Spotlight2 : register(b1)
{
	float3 positionSpotlight2;
	float range2;
	float3 direction2;
	float cone2;
	float3 color2;
	float pad222;
	float3 attenuation2;
	float pad22;
};

cbuffer Spotlight3 : register(b2)
{
	float3 positionSpotlight3;
	float range3;
	float3 direction3;
	float cone3;
	float3 color3;
	float pad3;
	float3 attenuation3;
	float pad23;
};

cbuffer cameraBuffer : register(b3)
{
	float3 cameraPostion;
	float cameraPadding;
}

cbuffer DirectionalLight : register(b4)
{
	float3 DirLightDirection;
	float paddingDir;
	float3 dirColor;
	float paddingDir2;
}


[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	int3 location = int3(0, 0, 0);
	
	location.x = DTid.x;
	location.y = DTid.y;
	
	float4 texcolor = textureColor[DTid.xy];
	
	float3 currentNormal = normalize(normal[DTid.xy].xyz);
	
	//float4 value = textureColor.Load(location);
	//backBuffer[DTid.xy] =  color;
	
		/*int3 location = int3(0, 0, 0);
	
	float shadow = textureColor.Load(location).w;
	
	location.x = DTid.x;
	location.y = DTid.y;
	
	float3 temp = textureColor[DTid.xy] * shadow;
	float4 color = float4(temp, 1.0f);*/
	
	
	
	float4 diffuse = DiffuseMap.Load(location).xyzw;
	
	float3 pixelToCam = position[DTid.xy].xyz - cameraPostion;
	pixelToCam = normalize(pixelToCam);
	float3 lightToPixel = positionSpotlight - position[DTid.xy].xyz;
	lightToPixel = normalize(lightToPixel);
	float lightLength = length(lightToPixel);
	float diffuseLevel = 0.0f;
	float specComp = 0.0f;
	
	if (lightLength <= range)
	{
		lightToPixel /= lightLength;
		float amountLight = dot(lightToPixel, currentNormal);
		
		if (amountLight > 0.0f)
		{
			diffuse /= (attenuation[0] + (attenuation[1] * lightLength)) + (attenuation[2] * (lightLength * lightLength));
			diffuseLevel = pow(max(dot(-lightToPixel, direction), 0.0f), cone);
			
			float3 reflection = normalize(reflect(-direction, currentNormal));
			//specComp = pow(max(dot(reflection, pixelToCam), 0.0f), shinines;

		}
		

	}
	
	float4 diffuse2 = DiffuseMap.Load(location).xyzw;
	
	float3 pixelToCam2 = position[DTid.xy].xyz - cameraPostion;
	pixelToCam2 = normalize(pixelToCam2);
	float3 lightToPixel2 = positionSpotlight2 - position[DTid.xy].xyz;
	lightToPixel2 = normalize(lightToPixel2);
	float lightLength2 = length(lightToPixel2);
	float diffuseLevel2 = 0.0f;
	float specComp2 = 0.0f;
	
	if (lightLength2 <= range)
	{
		lightToPixel2 /= lightLength2;
		float amountLight2 = dot(lightToPixel2, currentNormal);
		
		if (amountLight2 > 0.0f)
		{
			diffuse2 /= (attenuation2[0] + (attenuation2[1] * lightLength2)) + (attenuation2[2] * (lightLength2 * lightLength2));
			diffuseLevel2 = pow(max(dot(-lightToPixel2, direction), 0.0f), cone2);
		}
		

	}
	
	float4 diffuse3 = DiffuseMap.Load(location).xyzw;
	
	float3 pixelToCam3 = position[DTid.xy].xyz - cameraPostion;
	pixelToCam3 = normalize(pixelToCam3);
	float3 lightToPixel3 = positionSpotlight3 - position[DTid.xy].xyz;
	lightToPixel3 = normalize(lightToPixel3);
	float lightLength3 = length(lightToPixel3);
	float diffuseLevel3 = 0.0f;
	float specComp3 = 0.0f;
	
	if (lightLength3 <= range)
	{
		lightToPixel3 /= lightLength3;
		float amountLight3 = dot(lightToPixel3, currentNormal);
		
		if (amountLight3 > 0.0f)
		{
			diffuse3 /= (attenuation3[0] + (attenuation3[1] * lightLength3)) + (attenuation3[2] * (lightLength3 * lightLength3));
			diffuseLevel3 = pow(max(dot(-lightToPixel3, direction), 0.0f), cone3);
		}
		

	}
	
	//if (range == 500.0f)
	//{
	//	backBuffer[DTid.xy] = DiffuseMap[DTid.xy].xyzw;
	//}
	//else
	//{
	//	backBuffer[DTid.xy] = float4(0.0f, 1.0f, 1.0f,1.0f);
	//}
	
	float3 ambi = float3(0.2f, 0.2f, 0.2f);
	
	
	
	float3 lightDirectionV = -normalize(DirLightDirection);
	float3 diffusmapXD = DiffuseMap[DTid.xy].xyz;
	float3 diffuseStrength = (diffusmapXD * ((max(dot(currentNormal, lightDirectionV), 0.0f)))) * dirColor; //color last argument
	float3 refXD = normalize(reflect(DirLightDirection, currentNormal));
	
	float3 specColor = SpecularMap[DTid.xy].xyz * pow(max(dot(refXD, -pixelToCam), 0.0f), 25);
	
	float3 currentTexture = textureColor[DTid.xy].xyz;
	
	//diffuseStrength = diffuseStrength * float3(0.3, 0.3f, 0.0f);
	
	diffuseStrength = diffuseStrength * DiffuseMap[DTid.xy].w;
	specColor *= DiffuseMap[DTid.xy].w;
	float3 ambient = AmbientMap[DTid.xy].xyz * 0.2f;
	
	//ggr med shadow
	float3 diffuseSpot = (diffuse * diffuseLevel) * position[DTid.xy].w;//d0
	float3 diffuseSpot2 = (diffuse2 * diffuseLevel2) * normal[DTid.xy].w;//d1
	float3 diffuseSpot3 = (diffuse3 * diffuseLevel3) * AmbientMap[DTid.xy].w;//d2
	float3 newshit = (diffuseSpot + diffuseSpot2 + diffuseSpot3 + ambient + diffuseStrength + specColor);
	
	float3 newshit2 = (diffuseStrength + specColor);
	
	backBuffer[DTid.xy] = float4(newshit, 1.0f);
	
	//backBuffer[DTid.xy] = textureColor[DTid.xy].xyzw;
	

}