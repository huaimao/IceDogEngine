#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "../Utils/Common/DDSTextureLoader.h"
#include "PlatformDependenceRenderResource.h"
#include "PDRenderingDef.h"

namespace IceDogRendering
{
	class MaterialData
	{
	public:
		MaterialData();
		~MaterialData();

		/* call close to release the resource */
		void Close();

		/* set the diffuse map location */
		void SetDiffuseMap(std::wstring url);

		/* set the normal map location */
		void SetNormalMap(std::wstring url);

		/* set the parallax map location */
		void SetParallaxMap(std::wstring url, float scale, float layers, float quility);

		/* set the SRME Map Location */
		void SetSRMEMap(std::wstring url);

		/* load the material to the memory and create the shader resource view */
		bool LoadMaterial(IceDogRendering::PlatformDependenceRenderResource prr);

		/* get the diffuse shader resource view for rendering */
		ID3D11ShaderResourceView* GetDiffuseSRV();

		/* get the normal shader resource view for rendering */
		ID3D11ShaderResourceView* GetNormalSRV();

		/* get the parallax shader resource view for rendering */
		ID3D11ShaderResourceView* GetParallaxSRV();

		/* get the SRME shader resource view for rendering */
		ID3D11ShaderResourceView* GetSrmeSRV();

		/* get the parallax mapping config */
		IceDogUtils::float4 GetParallaxCfg();

		/* get the texture enabled description for shader usage */
		IceDogRendering::float4 GetTextureEnableDesc();

	private:
		// the location of diffuse map
		std::wstring c_diffuseMap_url;

		// the location of normal map
		std::wstring c_normalMap_url;

		// the location of parallax map
		std::wstring c_parallaxMap_url;

		// the location of the Specular Roughness Metallic Emiss
		std::wstring c_srme_url;

		// the shader resource view of diffuse map
		ID3D11ShaderResourceView* r_diffuseSRV;

		// the shader resource view of normal map
		ID3D11ShaderResourceView* r_normalSRV;

		// the shader resource view of parallax map
		ID3D11ShaderResourceView* r_parallaxSRV;

		// the shader resource view of srme
		ID3D11ShaderResourceView* r_srmeSRV;

		// the config of parallax mapping
		IceDogUtils::float4 c_parallaxCfg;

		// the flag inidicated the material has loaded;
		bool c_loaded;

		// the description of the enabled textures
		IceDogRendering::float4 c_DifNorParEmi;
	};
}

