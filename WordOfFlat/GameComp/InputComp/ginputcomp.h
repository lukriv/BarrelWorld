#ifndef __GAME_INPUT_COMPONENT_H__01__
#define __GAME_INPUT_COMPONENT_H__01__

#include <wx/thread.h>
#include <OGRE/OgreVector2.h>
#include <GameSystem/refobject.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/gmap.h>
#include <GameComp/InputComp/ginputsystem.h>
#include "../gcompbase.h"

// forward declaration
class wxXmlNode;
struct InputDef;

class ControlStruct
{
public:
    enum MouseButtonState {
	MOUSE_BUTTON_NONE = 0,
	MOUSE_BUTTON_LEFT = 1,
	MOUSE_BUTTON_RIGHT = 2,
	MOUSE_BUTTON_MIDDLE = 4,
    };

private:
    wxDword m_state;   // control state vector
    wxDword m_clicked; // note thate state was processed yet - reseted by release
    wxDword m_mouseButtonState;
    wxDword m_mouseButtonClicked;
    wxInt32 m_relativeMousePosition[2];
    wxInt32 m_absoluteMousePosition[2];
    wxInt32 m_mouseWheelDelta;

public:
    ControlStruct()
        : m_state(0)
        , m_clicked(0)
        , m_mouseButtonState(0)
        , m_mouseButtonClicked(0)
        , m_mouseWheelDelta(0)
    {
		m_relativeMousePosition[0] = 0;
		m_relativeMousePosition[1] = 0;
		m_absoluteMousePosition[0] = 0;
		m_absoluteMousePosition[1] = 0;
    }

    ControlStruct(const ControlStruct& control)
    {
		m_state = control.m_state;
		m_clicked = control.m_clicked;
		m_mouseButtonState = control.m_mouseButtonState;
		m_mouseButtonClicked = control.m_mouseButtonClicked;
		m_relativeMousePosition[0] = control.m_relativeMousePosition[0];
		m_relativeMousePosition[1] = control.m_relativeMousePosition[1];
		m_absoluteMousePosition[0] = control.m_absoluteMousePosition[0];
		m_absoluteMousePosition[1] = control.m_absoluteMousePosition[1];
		m_mouseWheelDelta = control.m_mouseWheelDelta;
    }

    inline void ResetAll()
    {
		m_state = 0;
		m_clicked = 0;
		m_mouseButtonState = 0;
		m_mouseButtonClicked = 0;
		m_relativeMousePosition[0] = 0;
		m_relativeMousePosition[1] = 0;
		m_absoluteMousePosition[0] = 0;
		m_absoluteMousePosition[1] = 0;
		m_mouseWheelDelta = 0;
    }

    inline void ResetClicked()
    {
		m_clicked = 0;
		m_mouseButtonClicked = 0;
		m_mouseWheelDelta = 0;
		m_relativeMousePosition[0] = 0;
		m_relativeMousePosition[1] = 0;
    }

    inline void Set(wxDword flag)
    {
		m_clicked |= flag;
		m_state |= flag;
    }

    inline void Release(wxDword flag)
    {
		m_state &= ~((wxDword)flag);
    }

    inline void SetMouseButton(wxDword flag)
    {
		m_mouseButtonClicked |= flag;
		m_mouseButtonState |= flag;
    }

    inline void ReleaseMouseButton(wxDword flag)
    {
		m_mouseButtonState &= ~((wxDword)flag);
    }

    inline bool IsPressed(wxDword flag) const
    {
		return ((m_state & flag) != 0);
    }

    inline bool WasClicked(wxDword flag) const
    {
		return ((m_clicked & flag) != 0);
    }

    inline bool IsMousePressed(wxDword flag) const
    {
		return ((m_mouseButtonState & flag) != 0);
    }

    inline bool WasMouseClicked(wxDword flag) const
    {
		return ((m_mouseButtonClicked & flag) != 0);
    }

    inline wxInt32 GetAbsX() const
    {
		return m_absoluteMousePosition[0];
    }
    inline wxInt32 GetAbsY() const
    {
		return m_absoluteMousePosition[1];
    }

    inline wxInt32 GetRelX() const
    {
		return m_relativeMousePosition[0];
    }
	
    inline wxInt32 GetRelY() const
    {
		return m_relativeMousePosition[1];
    }

    inline void SetAbsoluteMousePosition(wxInt32 x, wxInt32 y)
    {
		m_absoluteMousePosition[0] = x;
		m_absoluteMousePosition[1] = y;
    }

    inline void SetRelativeMousePosition(wxInt32 x, wxInt32 y)
    {
		m_relativeMousePosition[0] = x;
		m_relativeMousePosition[1] = y;
    }

    inline wxInt32 GetMouseWheelDelta() const
    {
		return m_mouseWheelDelta;
    }
    inline void SetMouseWheelDelta(wxInt32 mouseWheelDelta)
    {
		m_mouseWheelDelta = mouseWheelDelta;
    }
};

struct InputDef : public RefObjectImpl<IRefObject> {
    typedef GameBasMap<wxString, wxInt32> TInputMap;

public:
    TInputMap m_inputMap;

public:
    InputDef()
    {
    }
};

class InputComponent : public ComponentBase, public GameInputSystem::InputMouseCallback
{
private:
    ControlStruct m_ctrlStruct;
    wxCriticalSection m_inputLock;
	

protected:
    GameInputSystem* m_pInputSystem;
	RefObjSmPtr<InputDef> m_spDefinition;
	
public:
    InputComponent()
        : ComponentBase(GAME_COMP_INPUT)
        , m_inputLock(wxCRITSEC_DEFAULT)
    {
    }

    GameErrorCode Initialize(GameInputSystem* pInputSystem);
	
    /*!
     * \brief Export actual control struct state and reset it
     * \param actualControls Exported actual controls state
     */
    void ExportControlStruct(ControlStruct& actualControls)
    {
		wxCriticalSectionLocker lock(m_inputLock);
		actualControls = m_ctrlStruct;
		m_ctrlStruct.ResetClicked();
    }

    ///*!
    // * \brief
    // * \return Move vector - should be identity vector, if vector is zero, object does not want to move
    // */
    //Ogre::Vector2& GetMoveDirection2D();
    //
    ///*!
    // * \brief
    // * \return Look vector - should be identity vector, zero vector is allowed only in first or third person look
    // */
    //Ogre::Vector2& GetLookDirection2D();

    virtual GameErrorCode ReceiveMessage(TaskMessage& msg) override;
	
	virtual GameErrorCode Load(wxXmlNode* pNode);
    virtual GameErrorCode Store(wxXmlNode* pParentNode);

    
	virtual void OnMouseMoved(const OIS::MouseState& arg) override;
    virtual void OnMousePressed(const OIS::MouseState& arg, OIS::MouseButtonID id) override;
    virtual void OnMouseReleased(const OIS::MouseState& arg, OIS::MouseButtonID id) override;
	
	virtual GameErrorCode Create( InputDef *pInputDef) = 0;
	
protected:
	void SetDefinition(InputDef *pDef);
	InputDef* GetDefinition() const;
	
    GameErrorCode CreateInputDef(wxXmlNode* pNode, InputDef& output);

    GameErrorCode GetKeyValue(wxXmlNode* pNode, wxString& action, wxInt32& keyCode);
    GameErrorCode SetKeyValue(wxXmlNode* pParentNode, wxString& action, wxInt32& keyCode);

    inline void SetState(bool state, wxDword ctrlFlag)
    {
		wxCriticalSectionLocker lock(m_inputLock);
		if(state) {
			m_ctrlStruct.Set(ctrlFlag);
		} else {
			m_ctrlStruct.Release(ctrlFlag);
		}
    }
};



#endif //__GAME_INPUT_COMPONENT_H__01__