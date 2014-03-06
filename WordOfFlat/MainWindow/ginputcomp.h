#ifndef __GAME_INPUT_COMPONENT_H__
#define __GAME_INPUT_COMPONENT_H__


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
	sf::Vector2i m_lastMousePosition;
	sf::Vector2i m_lastMousePositionPressed;
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

	inline const sf::Vector2i& GetLastMousePosition() {
		return m_lastMousePosition;
	}
	inline void SetLastMousePosition(const sf::Vector2i& mousePos) {
		m_lastMousePosition = mousePos;
	}

	inline const sf::Vector2i& GetLastMousePositionPressed() {
		return m_lastMousePositionPressed;
	}
	inline void SetLastMousePositionPressed(const sf::Vector2i& mousePos) {
		m_lastMousePositionPressed = mousePos;
	}

	inline const wxInt32 GetMouseWheelDelta() {
		return m_mouseWheelDelta;
	}
	inline void SetMouseWheelDelta(wxInt32 mouseWheelDelta) {
		m_mouseWheelDelta = mouseWheelDelta;
	}

};


class GameInputComponent : public wxEventFilter
{
public:
	GameInputComponent()
	{
		wxEvtHandler::AddFilter(this);
	}
	
	~GameInputComponent()
	{
		wxEvtHandler::RemoveFilter(this);
	}
	
	
public:

    virtual int FilterEvent(wxEvent& event)
    {
        // Update the last user activity
        const wxEventType t = event.GetEventType();
        if ( t == wxEVT_KEY_DOWN || t == wxEVT_MOTION ||
                t == wxEVT_LEFT_DOWN ||
                    t == wxEVT_RIGHT_DOWN ||
                        t == wxEVT_MIDDLE_DOWN )
        {
            m_last = wxDateTime::Now();
        }
        // Continue processing the event normally as well.
        return Event_Processed;
    }

};

#endif // __GAME_INPUT_COMPONENT_H__
