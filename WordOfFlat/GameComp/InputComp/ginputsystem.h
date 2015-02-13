#ifndef __GAME_INPUT_COMPONENT_H__
#define __GAME_INPUT_COMPONENT_H__

#include <OGRE/OgreRenderWindow.h>
#include <OIS/OISInputManager.h>
#include <OIS/OISKeyboard.h>
#include <OIS/OISMouse.h>
#include <wx/thread.h>
#include <GameSystem/gdefs.h>
#include <GameSystem/gerror.h>
#include <GameSystem/refobjectimpl.h>
#include <GameSystem/refobject.h>
#include <GameSystem/glog.h>
#include <GameSystem/gset.h>

class InputDef;
class InputComponent;

class GameInputSystem : public OIS::KeyListener, public OIS::MouseListener {
private:
	class InputCallbackBase {
	public: 
		virtual void Invoke(bool pressed) = 0;
		virtual const void* GetClassPointer() const = 0;
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
		virtual void Invoke(bool pressed) override { (m_pClass->*m_methodPtr)(pressed); }
		
		const void* GetClassPointer() const override { return static_cast<void*>(m_pClass); }
	};
public:
	class InputMouseCallback {
	public:
		virtual void OnMouseMoved( const OIS::MouseState &arg ) = 0;
		virtual void OnMousePressed( const OIS::MouseState &arg, OIS::MouseButtonID id ) = 0;
		virtual void OnMouseReleased( const OIS::MouseState &arg, OIS::MouseButtonID id ) = 0;
	};
	
private:
	OIS::InputManager *m_pInputMgr;
	OIS::Keyboard *m_pKeyboard;
	OIS::Mouse *m_pMouse;
	InputCallbackBase* m_callbackArray[255];
	
	GameBasSet<InputMouseCallback*> m_mouseCallbacks;
	
	wxCriticalSection m_callbackArrayLock;	
	bool m_processMenuEvents;
	GameLoggerPtr m_spLogger;
	
public:

	GameInputSystem(GameLogger *pLogger) : m_pInputMgr(nullptr),
									m_pKeyboard(nullptr),
									m_pMouse(nullptr),
									m_processMenuEvents(false),
									m_spLogger(pLogger)
	{
		for (int i = 0; i < 255; ++i)
		{
			m_callbackArray[i] = nullptr;
		}
	}

	virtual ~GameInputSystem();
	
	GameErrorCode Initialize(Ogre::RenderWindow * pRenderWindow, bool processMenuEvents);
	void Uninitialize();

	/**
	 * \brief Register callback for given class (template)
	 * 
	 * Class T must have non-virtual method with signature 'void T::Method (bool param);'.
	 * 
	 * @param keyCode OIS Keycode
	 * @param pClass Class with method 
	 * @param MethodPtr Pointer to non-virtual method in class 'pClass'
	 * @return 
	 */
	template<class T>
	GameErrorCode RegisterCallback(OIS::KeyCode keyCode, T *pClass, void (T::*MethodPtr)(bool))
	{
		InputCallback<T> *pClbk = nullptr;
		FWG_RETURN_FAIL(GameNewChecked(pClbk,pClass,MethodPtr));
		
		wxCriticalSectionLocker lock(m_callbackArrayLock);
		if(m_callbackArray[keyCode] != nullptr)
		{
			GameDelete(m_callbackArray[keyCode]);
		}
		m_callbackArray[keyCode] = pClbk;
		return FWG_NO_ERROR;
	}
	
	/**
	 * \brief Unregister all callbacks which is registered on given class
	 * 
	 * Class can have registed more than one callback method and this method removes all of this callbacks.
	 * 
	 * @param pCallbackClass
	 */
	template<class T>
	void UnregisterCallbackClass(T* pCallbackClass)
	{
		if(pCallbackClass == nullptr)
		{
			return;
		}
		
		const void* pClass = static_cast<void*>(pCallbackClass);
		
		wxCriticalSectionLocker lock(m_callbackArrayLock);
		for (wxDword i = 0; i < 255; ++i)
		{
			if((m_callbackArray[i] != nullptr)&&(m_callbackArray[i]->GetClassPointer() == pClass))
			{
				GameDelete(m_callbackArray[i]);
				m_callbackArray[i] = nullptr;
			}
		}
	}
	
	/**
	 * \brief Unregister all registered callbacks
	 * 
	 * This method should be called before any of registered callback (or InputComponent) is freed!
	 * 
	 */
	void UnregisterAllCallbacks();
	
	GameErrorCode RegisterCallback(InputMouseCallback *pMouseCallback);
	
	void UnregisterCallback(InputMouseCallback *pMouseCallback);
	
	/// create methods
	GameErrorCode CreateAndRegisterInputComponent( const InputDef &inputDef, InputComponent *&pNewInputComp);
	
	inline GameLogger * GetLogger() { return m_spLogger; }
	
	/// virtual methods
	virtual GameErrorCode ProcessInputs();

	virtual bool keyPressed(const OIS::KeyEvent& arg);
	virtual bool keyReleased(const OIS::KeyEvent& arg);

	virtual bool mouseMoved(const OIS::MouseEvent& arg);
	virtual bool mousePressed(const OIS::MouseEvent& arg, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent& arg, OIS::MouseButtonID id);

};


#endif // __GAME_INPUT_COMPONENT_H__
