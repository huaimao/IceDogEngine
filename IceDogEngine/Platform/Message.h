#pragma once

namespace IceDogPlatform
{
	enum class MessageAuthority
	{
		SYSTEM,
		USER
	};

	enum class MessageType
	{
		fps,

		systemResize,

		leftButtonDoubleClick,
		leftButtonDown,
		leftButtonUp,
		rightButtonDoubleClick,
		rightButtonDown,
		rightButtonUp,
		midButtonDoubleClick,
		midButtonDown,
		midButtonUp,
		mouseMove,
		mouseWheel,
		keyDown,
		keyUp,
		aspectRatioChange,
		closeEngine
	};

	struct Message
	{
		MessageAuthority c_messageAuthority;
		MessageType c_messageType;
		float c_param0;
		float c_param1;
	};
}
