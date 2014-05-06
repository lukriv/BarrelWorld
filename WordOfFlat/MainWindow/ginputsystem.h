#ifndef __GAME_INPUT_COMPONENT_H__
#define __GAME_INPUT_COMPONENT_H__

#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>


class GameInputSystem : public OIS::KeyListener, public OIS::MouseListener
{
private:
	class InputCallbackBase {
	public: 
		virtual void Invoke(bool pressed) = 0;
		virtual ~InputCallbackBase() {}
	};
	
	template<class T>
	class InputCallback : public InputCallbackBase {
	public:
		typedef void (T::*MethodPtr)(bool);
	private:
		T *m_pClass;
		MethodPtr m_methodPtr;
	public:
		InputCallback(T* pClass, MethodPtr methodPtr): m_pClass(pClass), m_methodPtr(methodPtr) {}
		virtual void Invoke(bool pressed) { (m_pClass->*m_methodPtr)(pressed); }
	};

private:
	OIS::InputManager *m_pParent;
	OIS::Keyboard *m_pKeyboard;
	OIS::Mouse *m_pMouse;
	InputCallbackBase* m_callbackArray[255];
public:

	GameInputSystem(OIS::InputManager* pInputMgr) : m_pParent(pInputMgr),
									m_pKeyboard(NULL),
									m_pMouse(NULL)
	{
		for (int i = 0; i < 256; i++)
		{
			m_callbackArray[i] = NULL;
		}
	}

	virtual ~GameInputSystem();
	
	GameErrorCode Initialize(wxInt32 width, wxInt32 height);
	
	template<class T>
	GameErrorCode RegisterCallback(OIS::KeyCode keyCode, T *pClass, void (T::*MethodPtr)(bool))
	{
		InputCallback<T> *pClbk = NULL;
		FWG_RETURN_FAIL(GameNewChecked(pClbk,pClass,MethodPtr));
		if(m_callbackArray[keyCode] != NULL)
		{
			delete m_callbackArray[keyCode];
		}
		m_callbackArray[keyCode] = pClbk;
		return FWG_NO_ERROR;
	}
	
	virtual GameErrorCode ProcessInputs();

	virtual bool keyPressed(const OIS::KeyEvent& arg);
	virtual bool keyReleased(const OIS::KeyEvent& arg);

	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

};


#endif // __GAME_INPUT_COMPONENT_H__
