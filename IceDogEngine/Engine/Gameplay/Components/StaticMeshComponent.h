#pragma once
#include "Component.h"
#include "../../../Rendering/RenderData/MeshData.h"

namespace IceDogGameplay
{
	class StaticMeshComponent:public Component
	{
	public:
		StaticMeshComponent(class Actor* owner);
		~StaticMeshComponent();

		/* set the render data */
		void SetStaticMesh(std::shared_ptr<IceDogRendering::MeshData> rd);

		/* set enable */
		virtual void SetEnable() override;
		/* set disable */
		virtual void SetDisable() override;

		/* update the location from owner */
		virtual void Update() override;

	private:
		std::shared_ptr<IceDogRendering::MeshData> r_renderData;
	};
}

