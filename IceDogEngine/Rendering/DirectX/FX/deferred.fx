
struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 att;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 direction;
	float spot;
	float3 att;
	float pad;
};

cbuffer cbPerFrame
{
	DirectionalLight directionLight;
	SpotLight spotLight;
	PointLight pointLight;
	float3 lightOn;
	float3 eyePos;
};

cbuffer cbPerObject
{
	float4x4 m_world;
	float4x4 m_view;
	float4x4 m_viewInv;
	float4x4 m_proj;
	float4x4 m_worldInverseTranspose;
	float4 DifNorParEmi;
	float4 parallaxCfg;
	float isolevel;
};

struct VSIn
{
	float3 position : POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
	float2 diffuseUV : TEXCOORD0;
	float2 lightUV : TEXCOORD1;
};

struct VSOut
{
	float4 positionH : SV_POSITION;
	float4 color : COLOR;
	float4 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 modelUV : TEXCOORD0;
	float2 depth : TEXCOORD1;
};

struct PSOut
{
	float4 normal		: COLOR0;
	float4 baseColor	: COLOR1;
	float4 specular		: COLOR2;
	float4 depth		: COLOR3;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Voxel Terrian Marching Cube Pass Def Begin										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//  [ 7 6 ]
//[ 3 2 ]
//	[ 4 5 ]
//[ 0 1 ]
//
Texture1D<int1> mcEdgeTable;
Texture2D<int1> mcTriTable;

const float3 vertDecal[8] = {
	float3(0,0,0),
	float3(1,0,0),
	float3(1,1,0),
	float3(0,1,0),
	float3(0,0,1),
	float3(1,0,1),
	float3(1,1,1),
	float3(0,1,1),
};

float3 vertexInterp(float isolevel, float3 p1, float valp1, float3 p2, float valp2)
{
	return lerp(p1, p2, (isolevel - valp1) / (valp2 - valp1));
}

//Get edge table value
int edgeTableValue(int i) {
	return mcEdgeTable[i].r;
}

//Get triangle table value
int triTableValue(int i, int j) {
	return mcTriTable[int2(j,i)].r;
}

struct MCVsIn
{
	float3 pos : POSITION;
	float4 val_f : WEIGHT0;
	float4 val_b : WEIGHT1;
};

MCVsIn MCVS(MCVsIn vin)
{
	return vin;
}

[maxvertexcount(16)]
void MCGS(point MCVsIn gIn[1], inout TriangleStream<VSOut> triStream)
{
	//  [ 7 6 ] (back)
	//[ 3 2 ] (front)
	//	[ 4 5 ]
	//[ 0 1 ]
	//
	MCVsIn vet = gIn[0];
	int cubeindex = 0;
	//Determine the index into the edge table which
	//tells us which vertices are inside of the surface
	if (vet.val_f.x < isolevel) cubeindex = cubeindex | 1;
	if (vet.val_f.y < isolevel) cubeindex = cubeindex | 2;
	if (vet.val_f.z < isolevel) cubeindex = cubeindex | 4;
	if (vet.val_f.w < isolevel) cubeindex = cubeindex | 8;
	if (vet.val_b.x < isolevel) cubeindex = cubeindex | 16;
	if (vet.val_b.y < isolevel) cubeindex = cubeindex | 32;
	if (vet.val_b.z < isolevel) cubeindex = cubeindex | 64;
	if (vet.val_b.w < isolevel) cubeindex = cubeindex | 128;

	//Cube is entirely in/out of the surface
	if (edgeTableValue(cubeindex) == 0)
		return;
	
	float3 vertlist[12];
	//Find the vertices where the surface intersects the cube
	if ((edgeTableValue(cubeindex) & 1) != 0)
		vertlist[0] = vertexInterp(isolevel, vet.pos + vertDecal[0], vet.val_f.x, vet.pos + vertDecal[1], vet.val_f.y);
	if ((edgeTableValue(cubeindex) & 2) != 0)
		vertlist[1] = vertexInterp(isolevel, vet.pos + vertDecal[1], vet.val_f.y, vet.pos + vertDecal[2], vet.val_f.z);
	if ((edgeTableValue(cubeindex) & 4) != 0)
		vertlist[2] = vertexInterp(isolevel, vet.pos + vertDecal[2], vet.val_f.z, vet.pos + vertDecal[3], vet.val_f.w);
	if ((edgeTableValue(cubeindex) & 8) != 0)
		vertlist[3] = vertexInterp(isolevel, vet.pos + vertDecal[3], vet.val_f.w, vet.pos + vertDecal[0], vet.val_f.x);
	if ((edgeTableValue(cubeindex) & 16) != 0)
		vertlist[4] = vertexInterp(isolevel, vet.pos + vertDecal[4], vet.val_b.x, vet.pos + vertDecal[5], vet.val_b.y);
	if ((edgeTableValue(cubeindex) & 32) != 0)
		vertlist[5] = vertexInterp(isolevel, vet.pos + vertDecal[5], vet.val_b.y, vet.pos + vertDecal[6], vet.val_b.z);
	if ((edgeTableValue(cubeindex) & 64) != 0)
		vertlist[6] = vertexInterp(isolevel, vet.pos + vertDecal[6], vet.val_b.z, vet.pos + vertDecal[7], vet.val_b.w);
	if ((edgeTableValue(cubeindex) & 128) != 0)
		vertlist[7] = vertexInterp(isolevel, vet.pos + vertDecal[7], vet.val_b.w, vet.pos + vertDecal[4], vet.val_b.x);
	if ((edgeTableValue(cubeindex) & 256) != 0)
		vertlist[8] = vertexInterp(isolevel, vet.pos + vertDecal[0], vet.val_f.x, vet.pos + vertDecal[4], vet.val_b.x);
	if ((edgeTableValue(cubeindex) & 512) != 0)
		vertlist[9] = vertexInterp(isolevel, vet.pos + vertDecal[1], vet.val_f.y, vet.pos + vertDecal[5], vet.val_b.y);
	if ((edgeTableValue(cubeindex) & 1024) != 0)
		vertlist[10] = vertexInterp(isolevel, vet.pos + vertDecal[2], vet.val_f.z, vet.pos + vertDecal[6], vet.val_b.z);
	if ((edgeTableValue(cubeindex) & 2048) != 0)
		vertlist[11] = vertexInterp(isolevel, vet.pos + vertDecal[3], vet.val_f.w, vet.pos + vertDecal[7], vet.val_b.w);

	for (int i = 0; i<16; i += 3) {
		if (triTableValue(cubeindex,i) == -1) { break; }
		// vertex 0 
		VSOut v0;
		v0.positionH = mul(mul(mul(float4(vertlist[triTableValue(cubeindex,i)], 1), m_world), m_view), m_proj);
		v0.positionW = mul(float4(vertlist[triTableValue(cubeindex,i)], 1), m_world);
		v0.color = float4(0.6, 0.6, 0.6, 1);
		v0.depth = v0.positionH.zw;
		v0.modelUV = float2(0, 0);

		// vertex 1
		VSOut v1;
		v1.positionH = mul(mul(mul(float4(vertlist[triTableValue(cubeindex,i + 1)], 1), m_world), m_view), m_proj);
		v1.positionW = mul(float4(vertlist[triTableValue(cubeindex,i + 1)], 1), m_world);
		v1.color = float4(0.6, 0.6, 0.6, 1);
		v1.depth = v1.positionH.zw;
		v1.modelUV = float2(0, 1);

		// vertex 2
		VSOut v2;
		v2.positionH = mul(mul(mul(float4(vertlist[triTableValue(cubeindex,i + 2)], 1), m_world), m_view), m_proj);
		v2.positionW = mul(float4(vertlist[triTableValue(cubeindex,i + 2)], 1), m_world);
		v2.color = float4(0.6, 0.6, 0.6, 1);
		v2.depth = v2.positionH.zw;
		v2.modelUV = float2(1, 1);

		// cal world normal
		float3 normal = normalize(cross(v2.positionW - v0.positionW, v1.positionW - v0.positionW));
		v0.normalW = normal;
		v1.normalW = normal;
		v2.normalW = normal;

		triStream.Append(v1);
		triStream.Append(v0);
		triStream.Append(v2);
		triStream.RestartStrip();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Voxel Terrian Marching Cube Pass Def End										//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										G-Buffer Pass Start																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float3x3 compute_tangent_frame(float3 N, float3 p, float2 uv)
{
	// get edge vectors of pixel triangle
	float3 dp1 = ddx(p);
	float3 dp2 = ddy(p);
	float2 duv1 = ddx(uv);
	float2 duv2 = ddy(uv);

	// solve the linear system
	float3 dp2perp = cross(dp2, N);
	float3 dp1perp = cross(N, dp1);
	float3 T = (dp2perp * duv1.x + dp1perp * duv2.x);
	float3 B = (dp2perp * duv1.y + dp1perp * duv2.y);

	// construct a scale-invariant frame 
	float invmax = rsqrt(max(dot(T, T), dot(B, B)));
	return float3x3(T*invmax, B*invmax, N);
}

VSOut GBufferVS(VSIn vin)
{
	VSOut vout;
	vout.positionH = mul(mul(mul(float4(vin.position, 1.0), m_world), m_view), m_proj);
	vout.positionW = mul(float4(vin.position, 1.0), m_world);
	vout.modelUV = vin.diffuseUV;
	vout.normalW = normalize(mul(float4(vin.normal, 0.0), m_worldInverseTranspose).xyz);
	vout.color = vin.color;
	vout.depth = vout.positionH.zw;
	return vout;
}

float3 float_to_color(float f)
{
	float3 color;
	f *= 256;
	color.x = floor(f);
	f = (f - color.x) * 256;
	color.y = floor(f);
	color.z = f - color.y;
	color.xy *= 0.00390625; // *= 1.0/256
	return color;
}

float color_to_float(float3 color)
{
	const float3 byte_to_float = float3(1.0, 1.0 / 256, 1.0 / (256 * 256));
	return dot(color, byte_to_float);
}

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

Texture2D diffuseMap;
Texture2D normalMap;
Texture2D parallaxMap;

float2 parallaxMapping_low(float3 tant_eyeVec, float2 uv)
{
	float3 eyeVec = tant_eyeVec;
	float h = parallaxMap.Sample(samAnisotropic, uv).x;
	return eyeVec.xy / eyeVec.z *h * parallaxCfg.x;
}

float2 parallaxMapping_high(float3 tant_eyeVec, float2 uv)
{
	float3 eyeVec = tant_eyeVec;
	float _ParallaxScale = parallaxCfg.x;

	// subdiv layers
	float numLayers = parallaxCfg.y;

	// step high in one layer
	float layerHeight = 1.0 / numLayers;
	// max high
	float currentLayerHeight = 1.0;
	// max delta
	float2 P = eyeVec.xy * _ParallaxScale;
	// delta step length
	float2 deltaTexCoords = P / numLayers;

	// step by step until find the cloestest
	float2 currentTexCoords = uv;
	float currentDepthMapValue = parallaxMap.Sample(samAnisotropic, currentTexCoords).x;
	int times = 0;
	while (currentLayerHeight > currentDepthMapValue&&times<41)
	{
		currentTexCoords -= deltaTexCoords;
		currentDepthMapValue = parallaxMap.Sample(samAnisotropic, currentTexCoords).x;
		currentLayerHeight -= layerHeight;
		times = times + 1;
	}

	// cal h1, h2
	float2 prevTexCoords = currentTexCoords + deltaTexCoords;
	float afterHeight = currentDepthMapValue - currentLayerHeight;
	float beforeHeight = currentLayerHeight + layerHeight - parallaxMap.Sample(samAnisotropic, prevTexCoords).x;
	// get weight by h1 h2, interplate bet two point
	float weight = afterHeight / (afterHeight + beforeHeight);
	float2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return finalTexCoords - uv;
}

PSOut GBufferPS(VSOut pin) : SV_Target{
	PSOut result;
	
	float3x3 tangentMat;
	if (DifNorParEmi.z == 1 || DifNorParEmi.y == 1)
	{
		tangentMat = compute_tangent_frame(normalize(pin.normalW), pin.positionW, pin.modelUV);
	}

	// if use parallax mapping just modify the uv
	if (DifNorParEmi.z == 1)
	{
		float3 eyeVec = normalize(mul(normalize(eyePos - pin.positionW), transpose(tangentMat)));
		if (abs(1 - parallaxCfg.z) < 0.01)
			pin.modelUV = pin.modelUV + parallaxMapping_high(eyeVec, pin.modelUV);
		else
			pin.modelUV = pin.modelUV + parallaxMapping_low(eyeVec, pin.modelUV);
	}
	
	if (DifNorParEmi.y==1)
	{
		float3 normal_map = normalMap.Sample(samAnisotropic, pin.modelUV);
		normal_map = normalize(2 * (normal_map - 0.5));
		result.normal = float4(normalize(mul(normal_map, tangentMat))*0.5f + 0.5f, 0);
	}
	else
	{
		result.normal = float4(normalize(pin.normalW)*0.5f + 0.5f, 0);
	}

	if (DifNorParEmi.x==1)
	{
		result.baseColor = diffuseMap.Sample(samAnisotropic, pin.modelUV);
	}
	else
	{
		result.baseColor = float4(0.75, 0.75, 0.75, 1);
	}
	result.specular = float4(0.1, 0.1, 0.1, 1);
	float depth = pin.depth.x / pin.depth.y;
	// if use parallax mapping depth also need to be adjust
	if (DifNorParEmi.z == 1)
	{
		depth += parallaxCfg.x*parallaxMap.Sample(samAnisotropic, pin.modelUV).x;
	}
	result.depth.rgb = float_to_color(depth);
	result.depth.a = 1;
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										G-Buffer Pass End																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Lighting Pass Start																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LightVSIn
{
	float3 position : POSITION;
};

struct LightVSOut
{
	float4 position : POSITION;
};

struct LightPSOut
{
	float4 finalColor : COLOR;
};

struct LightGSOut
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
};

float NrmDevZToViewZ(float nz)
{
	float a = m_proj[2].z;
	float b = m_proj[3].z;
	float vz = b / (nz - a);
	return vz;
}

float NrmDevXToViewX(float nx, float vz)
{
	float a = m_proj[0].x;
	float vx = (nx*vz) / a;;
	return vx;
}

float NrmDevYToViewY(float ny, float vz)
{
	float a = m_proj[1].y;
	float vy = (ny*vz) / a;
	return vy;
}

LightVSOut LightVS(LightVSIn vin)
{
	LightVSOut a;
	a.position = float4(0.5, 0.5, 0, 1);
	return a;
}

[maxvertexcount(4)]
void LightGS(point LightVSOut gIn[1],
inout TriangleStream<LightGSOut> triStream)
{
	LightGSOut O0;
	LightGSOut O1;
	LightGSOut O2;
	LightGSOut O3;
	O0.uv = float2(0, 1);   //------->u
	O1.uv = float2(0, 0);	//|
	O2.uv = float2(1, 1);	//|
	O3.uv = float2(1, 0);	//v
	O0.position = float4(-1, -1, 0, 1);		//y
	O1.position = float4(-1, 1, 0, 1);		//|
	O2.position = float4(1, -1, 0, 1);		//|
	O3.position = float4(1, 1, 0, 1);		//----->x
	triStream.Append(O0);
	triStream.Append(O1);
	triStream.Append(O2);
	triStream.Append(O3);
}

Texture2D gBuffer_normal;
Texture2D gBuffer_baseColor;
Texture2D gBuffer_specular;
Texture2D gBuffer_depth;

const static float Pi = 3.14159265374;

float3 CalRf(float3 Rf0, float thetai)
{
	return Rf0 + (1.0 - Rf0)*pow((1.0 - max(cos(thetai), 0.0)), 5);
}

LightPSOut LightPS(LightGSOut vout) : SV_Target{
	LightPSOut result;
	if (vout.uv.y > 0.75)
	{
		if (vout.uv.x <= 0.25)
		{
			result.finalColor= gBuffer_normal.Sample(samAnisotropic, float2(vout.uv.x * 4, (vout.uv.y - 0.75) * 4));
		}
		else if (vout.uv.x <= 0.5)
		{
			result.finalColor = gBuffer_baseColor.Sample(samAnisotropic, float2(vout.uv.x * 4-1, (vout.uv.y - 0.75) * 4));
		}
		else if (vout.uv.x <= 0.75)
		{
			result.finalColor = gBuffer_specular.Sample(samAnisotropic, float2(vout.uv.x * 4 - 2, (vout.uv.y - 0.75) * 4));
		}
		else
		{
			result.finalColor = gBuffer_depth.Sample(samAnisotropic, float2(vout.uv.x * 4 - 3, (vout.uv.y - 0.75) * 4));
		}
	}
	else
	{
		// cal world position using depth
		float4 depthSample = gBuffer_depth.Sample(samAnisotropic, vout.uv);
		float ndcDepth = clamp(color_to_float(depthSample.xyz), 0.0f, 1.0f);
		float4 wPos;
		wPos.z = NrmDevZToViewZ(ndcDepth);
		wPos.x = NrmDevXToViewX(vout.uv.x*2.0f - 1.0f, wPos.z);
		wPos.y = NrmDevYToViewY(-(vout.uv.y*2.0f - 1.0f), wPos.z);
		wPos.w = 1;
		wPos = mul(wPos, m_viewInv);

		float m = 10;
		float3 wNormal = gBuffer_normal.Sample(samAnisotropic, vout.uv)*2.0f - 1.0f;
		float3 El = float3(3.5,3.5,3.5);
		float3 l = normalize(-directionLight.direction);
		float Cosi = saturate(dot(wNormal, l));
		float4 Cdiff = gBuffer_baseColor.Sample(samAnisotropic, vout.uv);
		float4 Cspec = gBuffer_specular.Sample(samAnisotropic, vout.uv);
		float3 v = normalize(eyePos - wPos.xyz);
		float3 h = normalize(l + v);
		float Cosh = saturate(dot(wNormal, h));

		// the brdf: diff term      the hight light term
		float4 brdf = Cdiff / Pi + ((m + 8) / (8 * Pi))*pow(Cosh, m)*Cspec;

		result.finalColor = brdf * float4(El*Cosi, 1);
	}
	return result;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//										Lighting Pass End																//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

technique11 Deferred
{
	pass VoxelStage
	{
		SetVertexShader(CompileShader(vs_5_0, MCVS()));
		SetGeometryShader(CompileShader(gs_5_0, MCGS()));
		SetPixelShader(CompileShader(ps_5_0, GBufferPS()));
	}
	pass GBufferStage
	{
		SetVertexShader(CompileShader(vs_5_0, GBufferVS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_5_0, GBufferPS()));
	}
	pass LightingStage
	{
		SetVertexShader(CompileShader(vs_5_0, LightVS()));
		SetGeometryShader(CompileShader(gs_5_0, LightGS()));
		SetPixelShader(CompileShader(ps_5_0, LightPS()));
	}
}
