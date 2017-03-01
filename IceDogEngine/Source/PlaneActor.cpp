#include "PlaneActor.h"
#include "../Engine/Engine.h"
#include "../Engine/Gameplay/Components/StaticMeshComponent.h"
#include "../Resources/Geometry/GeometryGenerator.h"

using namespace IceDogGameplay;


PlaneActor::PlaneActor()
{
	// test code
	StaticMeshComponent* st = new StaticMeshComponent(this);
	std::shared_ptr<IceDogRendering::RenderData> rd = std::make_shared<IceDogRendering::RenderData>();
	IceDogResources::IceDogGeometry::GeometryGenerator::CreateGrid(100, 100, 100, 100, rd);

	rd->SetMaterial(IceDogEngine::Engine::GetEngine()->LoadMaterialFromUrl("Source/Material/Mat_wood.IDAssets"));

	SetActorRotation(IceDogUtils::float3(0, 90, 0));

	st->SetStaticMesh(rd);
	SetEnable();
}


PlaneActor::~PlaneActor()
{
}