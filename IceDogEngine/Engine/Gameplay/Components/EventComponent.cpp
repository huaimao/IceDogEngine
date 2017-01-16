#include "EventComponent.h"
#include "../../../Engine/Engine.h"

namespace IceDogGameplay
{
	EventComponent::EventComponent(class Actor* owner):Component::Component(owner)
	{

	}

	EventComponent::~EventComponent()
	{
	}

	void EventComponent::SetEnable()
	{
		IceDogEngine::Engine::GetEngine()->GetEngineCore().RegistMessageProc(this);
	}

	void EventComponent::SetDisable()
	{
		IceDogEngine::Engine::GetEngine()->GetEngineCore().UnRegistMessageProc(this);
	}

	// 1 means handled
	int EventComponent::Process(const IceDogPlatform::MessageType& msgType, const float& pm0, const float& pm1)
	{
		if (!c_componentEnable) { return 0; }
		using IceDogPlatform::MessageType;
		switch (msgType)
		{
		case MessageType::leftButtonDoubleClick:
			if (onLeftDoubleClick&&onLeftDoubleClick(pm0, pm1)) { return 1; }
			break;
		case MessageType::leftButtonDown:
			if (onLeftDown&&onLeftDown(pm0, pm1)) { return 1; }
			break;
		case MessageType::leftButtonUp:
			if (onLeftUp&&onLeftUp(pm0, pm1)) { return 1; }
			break;
		case MessageType::midButtonDoubleClick:
			if (onMidDoubleClick&&onMidDoubleClick(pm0, pm1)) { return 1; }
			break;
		case MessageType::midButtonDown:
			if (onMidDown&&onMidDown(pm0, pm1)) { return 1; }
			break;
		case MessageType::midButtonUp:
			if (onMidUp&&onMidUp(pm0, pm1)) { return 1; }
			break;
		case MessageType::mouseMove:
			if (onMouseMove&&onMouseMove(pm0, pm1)) { return 1; }
			break;
		case MessageType::mouseWheel:
			if (onMouseWheel&&onMouseWheel(pm0 > 0)) { return 1; }
			break;
		case MessageType::rightButtonDoubleClick:
			if (onRightDoubleClick&&onRightDoubleClick(pm0, pm1)) { return 1; }
			break;
		case MessageType::rightButtonDown:
			if (onRightDown&&onRightDown(pm0, pm1)) { return 1; }
			break;
		case MessageType::rightButtonUp:
			if (onRightUp&&onRightUp(pm0, pm1)) { return 1; }
			break;
		case MessageType::keyDown:
			if (onKeyDown&&onKeyDown(pm1)) { return 1; }
			break;
		case MessageType::keyUp:
			if (onKeyUp&&onKeyUp(pm1-49152)) { return 1; }
			break;
		default:
			break;
		}
		return 0;
	}

}
