#pragma once

#include "../RenderingManager.h"

namespace IceDogRendering
{
	class DirectXRenderingManager :public RenderingManager
	{
	public:
		DirectXRenderingManager(std::ostream& logOs):RenderingManager(logOs),
			c_driverType(D3D_DRIVER_TYPE_HARDWARE)
		{}
		/* init the render manager */
		bool InitRenderManager(IceDogPlatform::PlatformWindow pfWindow) override;

		/* call close to release the resource */
		virtual void Close() override;

		/* set the driver type */
		void SetDriverType(RenderDriverType type) override;

		/* get the platform dependence resource */
		PlatformDependenceRenderResource GetPDRR() override;

		/* tick */
		void TickRenderingManager() override;

		/* register the ui pipe render data */
		void RegistUIRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) override;

		/* unregister the ui pipe render data */
		void UnRegistUIRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) override;

		/* register the scene pipe render data */
		void RegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) override;

		/* unregister the scene pipe render data */
		void UnRegistSceneRenderData(std::shared_ptr<IceDogRendering::RenderDataBase> rd) override;

		/* register the pipe view */
		void RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv) override;

		/* register the Light data */
		void RegistLightData(std::shared_ptr<IceDogRendering::LightBase> ld, LightType ltp) override;

		/* unregister the light data */
		void UnRegistLightData(std::shared_ptr<IceDogRendering::LightBase> ld, LightType ltp) override;

	private:
		/* update the RenderData index buffer */
		void UpdateRenderDataIndexBuffer(std::shared_ptr<IceDogRendering::RenderDataBase> rd);

		/* update the RenderData vertex buffer */
		void UpdateRenderDataVertexBuffer(std::shared_ptr<IceDogRendering::RenderDataBase> rd);

	private:
		D3D_DRIVER_TYPE c_driverType; //c--> config
		ID3D11Device* r_device; //r --> resource
		ID3D11DeviceContext* r_deviceContext;
	};
}

