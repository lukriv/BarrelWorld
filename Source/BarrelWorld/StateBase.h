#ifndef __BARREL_WORLD_STATE_BASE__H__
#define __BARREL_WORLD_STATE_BASE__H__




namespace BW
{
	
	class Game;

	class StateBase
	{
	protected:
		StateBase();
		~StateBase();
	public:
	  virtual void Init() = 0;
	  virtual void Cleanup() = 0;

	  virtual void Pause() = 0;
	  virtual void Resume() = 0;

	  virtual void Update(float timeStep) = 0;

	  void ChangeState(Game* game, StateBase* state);

	};

}

#endif // __BARREL_WORLD_STATE_BASE__H__
