#pragma once
#include "../PDRenderingDef.h"
#include "FXHelper.h"

#if defined __DIRECTX__
namespace IceDogRendering
{
	const D3D11_INPUT_ELEMENT_DESC vertexDesc[]
	{
		{ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,28,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TANGENTU",0,DXGI_FORMAT_R32G32B32_FLOAT,0,40,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,52,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "TEXCOORD",1,DXGI_FORMAT_R32G32_FLOAT,0,60,D3D11_INPUT_PER_VERTEX_DATA,0 },
	};
}
#endif