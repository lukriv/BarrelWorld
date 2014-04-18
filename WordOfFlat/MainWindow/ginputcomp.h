#ifndef __GAME_INPUT_COMPONENT_H__
#define __GAME_INPUT_COMPONENT_H__

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include "../GameSystem/gdefs.h"
#include "../GameSystem/gerror.h"

class ControlStruct
{
public:
	enum StateFlags {
	    SF_LEFT = 				(1<<0),
	    SF_RIGHT = 				(1<<1),
	    SF_UP =					(1<<2),
	    SF_DOWN = 				(1<<3),
	    SF_MOUSE_LEFT = 		(1<<4),
	    SF_MOUSE_RIGHT =		(1<<5),
	    SF_MOUSE_WHEEL_MOVE =	(1<<6)
	};
private:
	wxDword m_state; //control state vector
	wxDword m_processed; //note thate state was processed yet - reseted by release
//	wxInt32 m_lastMousePosition[2];
//	wxInt32 m_lastMousePositionPressed[2];
	wxInt32 m_mouseWheelDelta;
public:
	inline void SetLeft() {
		m_state |= SF_LEFT;
	}
	inline void SetRight() {
		m_state |= SF_RIGHT;
	}
	inline void SetUp() {
		m_state |= SF_UP;
	}
	inline void SetDown() {
		m_state |= SF_DOWN;
	}
	inline void SetMouseLeft() {
		m_state |= SF_MOUSE_LEFT;
	}
	inline void SetMouseRight() {
		m_state |= SF_MOUSE_RIGHT;
	}
	inline void SetMouseWheel() {
		m_state |= SF_MOUSE_WHEEL_MOVE;
	}

	inline void RelLeft() {
		m_state &= ~((wxDword) SF_LEFT);
		m_processed &= ~((wxDword) SF_LEFT);
	}
	inline void RelRight() {
		m_state &= ~((wxDword) SF_RIGHT);
		m_processed &= ~((wxDword) SF_RIGHT);
	}
	inline void RelUp() {
		m_state &= ~((wxDword) SF_UP);
		m_processed &= ~((wxDword) SF_UP);
	}
	inline void RelDown() {
		m_state &= ~((wxDword) SF_DOWN);
		m_processed &= ~((wxDword) SF_DOWN);
	}
	inline void RelMouseLeft() {
		m_state &= ~((wxDword) SF_MOUSE_LEFT);
		m_processed &= ~((wxDword) SF_MOUSE_LEFT);
	}
	inline void RelMouseRight() {
		m_state &= ~((wxDword) SF_MOUSE_RIGHT);
		m_processed &= ~((wxDword) SF_MOUSE_RIGHT);
	}
	inline void RelMouseWheel() {
		m_state &= ~((wxDword) SF_MOUSE_WHEEL_MOVE);
		m_processed &= ~((wxDword) SF_MOUSE_WHEEL_MOVE);
	}

	inline bool IsLeftPressed() {
		return ((m_state & SF_LEFT) != 0);
	}
	inline bool IsRightPressed() {
		return ((m_state & SF_RIGHT) != 0);
	}
	inline bool IsUpPressed() {
		return ((m_state & SF_UP) != 0);
	}
	inline bool IsDownPressed() {
		return ((m_state & SF_DOWN) != 0);
	}
	inline bool IsMouseLeftPressed() {
		return ((m_state & SF_MOUSE_LEFT) != 0);
	}
	inline bool IsMouseRightPressed() {
		return ((m_state & SF_MOUSE_RIGHT) != 0);
	}
	inline bool IsMouseWheelMove() {
		return ((m_state & SF_MOUSE_WHEEL_MOVE) != 0);
	}

	inline void SetLeftProcessed() {
		m_processed |= SF_LEFT;
	}
	inline void SetRightProcessed() {
		m_processed |= SF_RIGHT;
	}
	inline void SetUpProcessed() {
		m_processed |= SF_UP;
	}
	inline void SetDownProcessed() {
		m_processed |= SF_DOWN;
	}
	inline void SetMouseLeftProcessed() {
		m_processed |= SF_MOUSE_LEFT;
	}
	inline void SetMouseRightProcessed() {
		m_processed |= SF_MOUSE_RIGHT;
	}
	inline void SetMouseWheelProcessed() {
		m_processed |= SF_MOUSE_WHEEL_MOVE;
	}

	inline bool WasLeftProcessed() {
		return ((m_processed & SF_LEFT) != 0);
	}
	inline bool WasRightProcessed() {
		return ((m_processed & SF_RIGHT) != 0);
	}
	inline bool WasUpProcessed() {
		return ((m_processed & SF_UP) != 0);
	}
	inline bool WasDownProcessed() {
		return ((m_processed & SF_DOWN) != 0);
	}
	inline bool WasMouseLeftProcessed() {
		return ((m_processed & SF_MOUSE_LEFT) != 0);
	}
	inline bool WasMouseRightProcessed() {
		return ((m_processed & SF_MOUSE_RIGHT) != 0);
	}
	inline bool WasMouseWheelMove() {
		return ((m_processed & SF_MOUSE_WHEEL_MOVE) != 0);
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


class GameInputComponent : public OIS::KeyListener, public OIS::MouseListener
{
	OIS::InputManager *m_pParent;
	OIS::Keyboard *m_pKeyboard;
	OIS::Mouse *m_pMouse;
	bool m_exit;
public:

	GameInputComponent(OIS::InputManager* pInputMgr) : m_pParent(pInputMgr),
									m_pKeyboard(NULL),
									m_pMouse(NULL),
									m_exit(false) {}

	virtual ~GameInputComponent();
	
	GameErrorCode Initialize(wxInt32 width, wxInt32 height);
	
	virtual GameErrorCode ProcessInputs();

	virtual bool keyPressed(const OIS::KeyEvent& arg);
	virtual bool keyReleased(const OIS::KeyEvent& arg);

	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

public:
	void SetExit()
	{
		m_exit = true;
	}


	bool Exit() {
		return m_exit;
	}

};


#endif // __GAME_INPUT_COMPONENT_H__
