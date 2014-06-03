#ifndef __GAME_INPUT_COMPONENT_H__01__
#define __GAME_INPUT_COMPONENT_H__01__

#include <wx/thread.h>
#include <OGRE/OgreVector2.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>


class ControlStruct
{
public:
	enum StateFlags {
		INPUT_ACTION_NONE =						0 ,
	    INPUT_ACTION_LEFT = 				(1<<0),
	    INPUT_ACTION_RIGHT = 				(1<<1),
	    INPUT_ACTION_UP =					(1<<2),
	    INPUT_ACTION_DOWN = 				(1<<3),
		INPUT_ACTION_FORWARD =				(1<<4),
		INPUT_ACTION_BACKWARD =				(1<<5),
		INPUT_ACTION_JUMP =					(1<<6),
		INPUT_ACTION_CROUCH =				(1<<7),
	    INPUT_ACTION_ATTACK =	 			(1<<8),
	    INPUT_ACTION_DEFEND =				(1<<9),
		INPUT_ACTION_USE =					(1<<10),
	    
	};
private:
	wxDword m_state; //control state vector
	wxDword m_clicked; //note thate state was processed yet - reseted by release
//	wxInt32 m_lastMousePosition[2];
//	wxInt32 m_lastMousePositionPressed[2];
	wxInt32 m_mouseWheelDelta;
public:
	ControlStruct() : m_state(0), m_clicked(0), m_mouseWheelDelta(0) {}
	ControlStruct(const ControlStruct& control)
	{
		m_state = control.m_state;
		m_clicked = control.m_clicked;
		m_mouseWheelDelta = control.m_mouseWheelDelta;
	}
	
	inline void ResetAll()
	{
		m_state = 0;
		m_clicked = 0;
		m_mouseWheelDelta = 0;
	}
	
	inline void ResetClicked()
	{
		m_clicked = 0;
		m_mouseWheelDelta = 0;
	}

	inline void Set(StateFlags flag)
	{
		m_clicked |= flag;
		m_state |= flag;
	}
	
	inline void Release(StateFlags flag)
	{
		m_state &= ~((wxDword) flag);
	}

	inline bool IsPressed(StateFlags flag) {
		return ((m_state & flag) != 0);
	}

	inline bool WasClicked(StateFlags flag) {
		return ((m_clicked & flag) != 0);
	}

	

//	inline const sf::Vector2i& GetLastMousePosition() {
//		return m_lastMousePosition;
//	}
//	inline void SetLastMousePosition(const sf::Vector2i& mousePos) {
//		m_lastMousePosition = mousePos;
//	}
//
//	inline const sf::Vector2i& GetLastMousePositionPressed() {
//		return m_lastMousePositionPressed;
//	}
//	inline void SetLastMousePositionPressed(const sf::Vector2i& mousePos) {
//		m_lastMousePositionPressed = mousePos;
//	}

	inline const wxInt32 GetMouseWheelDelta() {
		return m_mouseWheelDelta;
	}
	inline void SetMouseWheelDelta(wxInt32 mouseWheelDelta) {
		m_mouseWheelDelta = mouseWheelDelta;
	}

};


class InputComponent : public RefObjectImpl<IRefObject> 
{
	ControlStruct m_ctrlStruct;
	wxCriticalSection m_inputLock;
public:
	InputComponent() : m_inputLock(wxCRITSEC_DEFAULT) {}
	
	/*!
	 * \brief Export actual control struct state and reset it
	 * \param actualControls Exported actual controls state
	 */
	void ExportControlStruct(ControlStruct &actualControls)
	{
		wxCriticalSectionLocker lock(m_inputLock);
		actualControls = m_ctrlStruct;
		m_ctrlStruct.ResetClicked();
	}
	
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
	
	void SetMoveUp(bool move)
	{
		SetState(move, ControlStruct::INPUT_ACTION_UP);
	}
	
	void SetMoveDown(bool move)
	{
		SetState(move, ControlStruct::INPUT_ACTION_DOWN);
	}
	
	void SetMoveLeft(bool move)
	{
		SetState(move, ControlStruct::INPUT_ACTION_LEFT);
	}
	
	void SetMoveRight(bool move)
	{
		SetState(move, ControlStruct::INPUT_ACTION_RIGHT);
	}
	
	void SetMoveForward(bool move)
	{
		SetState(move, ControlStruct::INPUT_ACTION_FORWARD);
	}
	
	void SetMoveBackward(bool move)
	{
		SetState(move, ControlStruct::INPUT_ACTION_BACKWARD);
	}

protected:
	inline void SetState(bool state, ControlStruct::StateFlags ctrlFlag)
	{
		wxCriticalSectionLocker lock(m_inputLock);
		if(state)
		{
			m_ctrlStruct.Set(ctrlFlag);
		} else {
			m_ctrlStruct.Release(ctrlFlag);
		}
	}
};


#endif //__GAME_INPUT_COMPONENT_H__01__