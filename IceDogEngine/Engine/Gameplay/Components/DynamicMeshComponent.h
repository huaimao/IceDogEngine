#pragma once
#include "Component.h"
#include "../../../Rendering/DynamicRenderData.h"

namespace IceDogGameplay 
{
	class DynamicMeshComponent:public Component
	{
	public:
		DynamicMeshComponent();
		~DynamicMeshComponent();
	private:
		std::shared_ptr<IceDogRendering::DynamicRenderData> r_renderData;
	};
}

