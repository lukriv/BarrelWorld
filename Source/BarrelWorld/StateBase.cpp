#include "StateBase.h"

#include "Game.h"

BW::StateBase::StateBase()
{
}

BW::StateBase::~StateBase()
{
}


void BW::StateBase::ChangeState(BW::Game* game, BW::StateBase* state)
{
	game->ChangeState(state);
}
