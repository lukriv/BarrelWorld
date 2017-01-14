#ifndef __BARREL_WORLD_STATE_CLASS__H__
#define __BARREL_WORLD_STATE_CLASS__H__

#include <cstdint>

namespace BW
{
	
	class StateHandler 
	{
	public:
		virtual void ChangeState(int32_t actualStateId, int32_t nextStateId) = 0;
	};

	class State
	{
	protected:
		StateHandler *m_pStateHandler;
		int32_t m_stateId;
				
	public:
		State(StateHandler *pHandler, int32_t stateID);
		virtual ~State();
		
		int32_t GetStateId();
		
		virtual bool Start() = 0;
		virtual void Stop() = 0;

	};

}

#endif // __BARREL_WORLD_STATE_CLASS__H__
