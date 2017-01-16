#include "Engine.h"

using namespace IceDogEngine;

Engine* Engine::r_egPtr = nullptr;

Engine::Engine(std::ostream& errorLog, IceDogPlatform::PlatformWindow plfWindow)
	:s_errorlogOutStream(errorLog),
	r_enginePlatform(plfWindow,errorLog),
	r_renderAdapter(errorLog),
	r_logicAdapter(errorLog),
	r_aspectRatio(static_cast<float>(plfWindow.width)/static_cast<float>(plfWindow.height))
{
	r_egPtr = this;
	// the platform window will be further construct, we will not hold it.
}

void Engine::Init()
{
	// init the engine core
	r_engineCore.Init();
	// init the platform
	r_enginePlatform.InitPlatform();
	// regist the platform event
	r_engineCore.RegistPlatformTick(std::bind(&IceDogPlatform::Platform::TickPlatform, &r_enginePlatform));
	r_enginePlatform.RegistMessageProcessChain(std::bind(&IceDogCore::EngineCore::ProcessMessageChain, &r_engineCore, std::placeholders::_1));
	// init the render adapter
	r_renderAdapter.Init(r_enginePlatform.GetConstructedPlatformWindow());
	r_engineCore.RegistRenderingTick(std::bind(&IceDogRendering::RenderingAdapter::TickRendering, &r_renderAdapter));
	// init the logic adapter
	r_logicAdapter.Init();
	r_engineCore.RegistLogicTick(std::bind(&IceDogLogic::LogicAdapter::TickLogic, &r_logicAdapter, std::placeholders::_1));
}

void Engine::RegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd,IceDogRendering::RenderPipeType rpt)
{
	// add data to level
	r_defaultLevel.RegistRenderData(rd);
	r_renderAdapter.RegistRenderData(rd, rpt);
}

void IceDogEngine::Engine::UnRegistRenderData(std::shared_ptr<IceDogRendering::RenderData> rd, IceDogRendering::RenderPipeType rpt)
{
	// remove from level
	r_defaultLevel.UnRegistRenderData(rd);
	r_renderAdapter.UnRegistRenderData(rd, rpt);
}


void Engine::RegistLogicData(IceDogLogic::LogicData* ld)
{
	// add data to level
	r_defaultLevel.RegistLogicData(ld);
	r_logicAdapter.RegistLogicData(ld);
}

void Engine::UnRegistLogicData(IceDogLogic::LogicData* ld)
{
	// remove 
	r_defaultLevel.UnRegistLogicData(ld);
	r_logicAdapter.UnRegistLogicData(ld);
}


void Engine::RegistActor(IceDogGameplay::Actor* ac)
{
	r_defaultLevel.RegistActor(ac);
}

void IceDogEngine::Engine::UnRegistActor(IceDogGameplay::Actor* ac)
{
	r_defaultLevel.UnRegistActor(ac);
}

void IceDogEngine::Engine::RegistMainPipeView(std::shared_ptr<IceDogRendering::PipeView> pv)
{
	r_renderAdapter.RegistMainPipeView(pv);
}

float IceDogEngine::Engine::GetAspectRatio()
{
	return r_aspectRatio;
}

IceDogCore::EngineCore& IceDogEngine::Engine::GetEngineCore()
{
	return r_engineCore;
}

IceDogEngine::Engine* IceDogEngine::Engine::GetEngine()
{
	return r_egPtr;
}

void IceDogEngine::Engine::Run()
{
	// call the engine core to run. update the tick
	r_engineCore.Run();
}