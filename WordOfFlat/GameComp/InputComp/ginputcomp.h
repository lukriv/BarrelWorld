#ifndef __GAME_INPUT_COMPONENT_H__01__
#define __GAME_INPUT_COMPONENT_H__01__

enum InputActions {
	INPUT_ACTION_NONE 	= 0,
	INPUT_ACTION_UP		= 1,
	INPUT_ACTION_DOWN	= 1<<1,
	INPUT_ACTION_ATTACK	= 1<<2,
	INPUT_ACTION_DEFEND	= 1<<3
};


class InputComponent {
	
public:
	InputComponent() {}
	
	/*!
	 * \brief
	 * \return Move vector - should be identity vector, if vector is zero, object does not want to move
	 */
	Ogre::Vector2& GetMoveDirection2D();
	
	/*!
	 * \brief 
	 * \return Look vector - should be identity vector, zero vector is allowed only in first or third person look
	 */
	Ogre::Vector2& GetLookDirection2D();
	
	wxDword GetActiveActions();
	
	void SetActionMask(wxDword actionMask);
	wxDword GetActionMask();
	
};


#endif //__GAME_INPUT_COMPONENT_H__01__