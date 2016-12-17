#pragma once

#include "../Utils/Common/UtilBasic.h"
#include "LogicData.h"

// logic adapter run on it`s own thread, handle the logic request

namespace IceDogLogic
{
	class LogicAdapter
	{
	public:
		/* the construct function */
		LogicAdapter(std::ostream& errOs);
		/* init the logic adapter */
		void Init();
		/* tick the logic adapter, than tick all registed logic data */
		void TickLogic(float deltaTime);
		/* regist/unRegist the logic data */
		void RegistLogicData(LogicData* ld);
		void UnRegistLogicData(LogicData* ld);

	private:
		// the log output port
		std::ostream& s_errorlogOutStream;
		// the logic data that this adapter holds
		std::vector<LogicData*> r_logicDatas;
	};
}

