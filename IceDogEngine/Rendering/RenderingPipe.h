#pragma once

#include "PlatformDependenceRenderResource.h"
#include "RenderData/MeshData.h"
#include "RenderData/VoxelData.h"
#include "PipeView.h"
#include "LightingGroup.h"
#include "CubeMapSource.h"
#include "../Core/MessageProc.h"
#include "../Platform/PlatformWindow.h"

namespace IceDogRendering
{
	class ShaderManager;

	class RenderingPipe
	{
	public:
		RenderingPipe(std::ostream& errOS) :s_errorlogOutStream(errOS)
		{
			r_messageProc = IceDogCore::MessageProc::Create(IceDogCore::MessagePriority::SYSTEM_3);
			//bind the event processor
			r_messageProc->BindProcessor(std::bind(&RenderingPipe::EventProcessor, this, std::placeholders::_1, std::placeholders::_2,std::placeholders::_3));
		}
		/* call close to release the resource */
		virtual void Close();
		/* set msaa config not enable immediately*/
		virtual void SetMsaaConfig(bool msaaEnable) { c_enableMsaa = msaaEnable;};
		/* init the pipe line */
		virtual void InitPipe(IceDogPlatform::PlatformWindow pfWindow, ShaderManager* shaderManager) = 0;
		/* render */
		virtual void Render(std::vector<std::shared_ptr<RenderDataBase>>& renderDatas) = 0;
		/* init the platform dependence data */
		virtual void InitPlatformDepedence(PlatformDependenceRenderResource pdrr) { c_PDRR = pdrr; };
		/* register the main pipe view */
		virtual void RegistMainPipeView(std::shared_ptr<PipeView> pv) { r_mainPipeView = pv; }
		/* register the light data */
		virtual void RegistLightData(std::shared_ptr<LightBase> ld, LightType ltp);
		/* unregister the light data */
		virtual void UnRegistLightData(std::shared_ptr<LightBase> ld, LightType ltp);
	protected:
		/* reset the render target size */
		virtual void Resize(int newWidth, int newHeight) = 0;

	private:
		int EventProcessor(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)
		{
			if (msgType==IceDogPlatform::MessageType::systemResize)
			{
				Resize(static_cast<int>(pm0), static_cast<int>(pm1));
			}
			return 0;
		}

		/* add directional light to light group */
		void AddDirectionalLightToLightGroup(std::shared_ptr<DirectionLightData> dl);

	protected:
		// default light group
		std::vector<LightingGroup<1>> r_lightGroups;
		// the pipe view
		std::shared_ptr<PipeView> r_mainPipeView;
		std::vector < std::shared_ptr<PipeView> > r_pipeViewList;
		// the platform window that output the render result
		IceDogPlatform::PlatformWindow c_platformWindow;
		// the platform dependence rendering resource
		PlatformDependenceRenderResource c_PDRR;
		// the cube map resource
		CubeMapSource r_cubeMapSource;
		// whether enable the multi sample anti alian
		bool c_enableMsaa;
		// msaa quality
		unsigned int c_msaaQuility;
		// the refresh rate of swap chain
		unsigned int c_swapChainBufferRefreshRate;
		// back buffer width
		unsigned int c_backBufferWidth;
		// back buffer height
		unsigned int c_backBufferHeight;
		// the error out put log port
		std::ostream& s_errorlogOutStream;
		// a shader manager, only a raw ptr, not willing this guy to have the authority to controls the memory destroy
		ShaderManager* r_shaderManager;
	private:
		// default event processor
		std::shared_ptr<IceDogCore::MessageProc> r_messageProc;
	};
}