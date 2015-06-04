#ifndef __GAME_INPUT_CHARACTER_MANAGER_H__
#define __GAME_INPUT_CHARACTER_MANAGER_H__

class InputCharacterManager : public GameManagerBase<CharacterInput>
{
private:
	GameInputSystem* m_pInputSystem;
public:
	InputCharacterManager(GameInputSystem* pInputSystem);
	~InputCharacterManager();
	
	GameErrorCode CreateComponent( wxDword compId,  CharacterInput *&pNewInputComp);

};

#endif // __GAME_INPUT_CHARACTER_MANAGER_H__
