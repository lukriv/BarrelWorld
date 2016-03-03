#ifndef __GAME_PROPERTY_H__
#define __GAME_PROPERTY_H__


#include <GameSystem/glog.h>
#include <GameSystem/gmap.h>


enum GamePropertyType {
	FWG_PROP_UNKNOWN = 0,
	FWG_PROP_DWORD,
	FWG_PROP_INT32,
	FWG_PROP_FLOAT,
	FWG_PROP_STRING,
	FWG_PROP_VECTOR3
};


class GamePropertyValue {
	GamePropertyType m_type;
	void* m_pContent;
private:
	void ReleaseContent(GamePropertyType type)
	{
		switch (type)
		{
		case FWG_PROP_STRING:
			delete static_cast<wxString*>(m_pContent);
			break;
		case FWG_PROP_VECTOR3:
			delete static_cast<Ogre::Vector3*>(m_pContent);
			break;
		case FWG_PROP_FLOAT:
		case FWG_PROP_INT32:
		case FWG_PROP_DWORD:
		default:
			break;
		}
		m_pContent = nullptr;
		m_type = FWG_PROP_UNKNOWN;
	}
	
	void Copy(const GamePropertyValue& orig)
	{
		switch(orig.m_type)
		{
		case FWG_PROP_FLOAT:
		case FWG_PROP_INT32:
		case FWG_PROP_DWORD:
			m_pContent = orig.m_pContent;
			m_type = orig.m_type;
			break;
		case FWG_PROP_STRING:
			SetValue(* static_cast<wxString*>(orig.m_pContent));
			break;
		case FWG_PROP_VECTOR3:
			SetValue(* static_cast<Ogre::Vector3*>(orig.m_pContent));
			break;
		default:
			break;
		}
	}
	
public:	
	
	GamePropertyValue() : m_type(FWG_PROP_UNKNOWN), m_pContent(nullptr) {}
	GamePropertyValue(const GamePropertyValue& orig) : m_type(FWG_PROP_UNKNOWN), m_pContent(nullptr)
	{
		Copy(orig);
	}
	
	~GamePropertyValue() 
	{
		ReleaseContent(m_type);
	}
	
	inline GamePropertyType GetType() { return m_type; }
	
	GamePropertyValue& operator=(const GamePropertyValue& orig)
	{
		if(&orig != this)
		{
			Copy(orig);
		}
		
		return *this;
	}
	
	void SetValue(const Ogre::Vector3 &vec3)
	{
		if((m_type == FWG_PROP_VECTOR3)&&(m_pContent))
		{
			* static_cast<Ogre::Vector3*>(m_pContent) = vec3;
		} else {
			ReleaseContent(m_type);
			m_pContent = new (std::nothrow) Ogre::Vector3(vec3);
			m_type = FWG_PROP_VECTOR3;
		}
	}
	
	GameErrorCode GetValue (Ogre::Vector3 &vec3) const
	{
		if((m_type == FWG_PROP_VECTOR3)&&(m_pContent))
		{
			vec3 = * static_cast<Ogre::Vector3*>(m_pContent);
			return FWG_NO_ERROR;
		}
		
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	void SetValue(const wxString &str)
	{
		if((m_type == FWG_PROP_STRING)&&(m_pContent))
		{
			* static_cast<wxString*>(m_pContent) = str;
		} else {
			ReleaseContent(m_type);
			m_pContent = new (std::nothrow) wxString(str);
			m_type = FWG_PROP_STRING;
		}
	}
	
	GameErrorCode GetValue (wxString &str) const
	{
		if((m_type == FWG_PROP_STRING)&&(m_pContent))
		{
			str = * static_cast<wxString*>(m_pContent);
			return FWG_NO_ERROR;
		}
		
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	void SetValue(wxDword word)
	{
		if(m_type == FWG_PROP_DWORD)
		{
			m_pContent = reinterpret_cast<void*&>(word);
		} else {
			ReleaseContent(m_type);
			m_pContent = reinterpret_cast<void*&>(word);
			m_type = FWG_PROP_DWORD;
		}
	}
	
	GameErrorCode GetValue (wxDword &word)
	{
		if((m_type == FWG_PROP_DWORD)&&(m_pContent))
		{
			word = reinterpret_cast<wxDword&>(m_pContent);
			return FWG_NO_ERROR;
		}
		
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	void SetValue(wxInt32 word)
	{
		if(m_type == FWG_PROP_INT32)
		{
			m_pContent = reinterpret_cast<void*&>(word);
		} else {
			ReleaseContent(m_type);
			m_pContent = reinterpret_cast<void*&>(word);
			m_type = FWG_PROP_INT32;
		}
	}
	
	GameErrorCode GetValue (wxInt32 &word)
	{
		if(m_type == FWG_PROP_INT32)
		{
			word = reinterpret_cast<wxInt32&>(m_pContent);
			return FWG_NO_ERROR;
		}
		
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	void SetValue(float fl)
	{
		if(m_type == FWG_PROP_FLOAT)
		{
			m_pContent = reinterpret_cast<void*&>(fl);
		} else {
			ReleaseContent(m_type);
			m_pContent = reinterpret_cast<void*&>(fl);
			m_type = FWG_PROP_FLOAT;
		}
	}
	
	GameErrorCode GetValue (float &fl)
	{
		if(m_type == FWG_PROP_FLOAT)
		{
			fl = reinterpret_cast<float&>(m_pContent);
			return FWG_NO_ERROR;
		}
		
		return FWG_E_INVALID_DATA_ERROR;
	}
	
	
};


class GamePropertyContainer {
	typedef GameBasMap<wxString, GamePropertyValue> GamePropertyMap;
private:
	GamePropertyMap m_container;
	GameLoggerPtr m_spLogger;
public:
	GamePropertyContainer(GameLogger *pLogger) : m_spLogger(pLogger) {}
	GamePropertyContainer(const GamePropertyContainer &orig) : m_container(orig.m_container), m_spLogger(orig.m_spLogger) {}
	
	
	
	template<class T>
	void SetProperty(const wxString& name, const T& value )
	{
		GameErrorCode result = FWG_NO_ERROR;
		GamePropertyValue *pValue = m_container.FindValue(name);
		if(pValue)
		{
			pValue->SetValue(value);
		} else {
			GamePropertyValue propValue;
			propValue.SetValue(value);
			if(FWG_FAILED(result = m_container.Insert(name, propValue)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Set property '%s' failed: 0x%08x"), m_spLogger, name.GetData().AsInternal(), result, FWGLOG_ENDVAL);
			}
		}
	}
	
	template<class T>
	GameErrorCode GetProperty(const wxString& name, T& value)
	{
		GameErrorCode result = FWG_NO_ERROR;
		GamePropertyValue *pValue = m_container.FindValue(name);
		if(pValue)
		{
			if(FWG_FAILED(result = pValue->GetValue(value)))
			{
				FWGLOG_ERROR_FORMAT(wxT("Get property '%s' failed: 0x%08x"), m_spLogger, name.GetData().AsInternal(), result, FWGLOG_ENDVAL);
				return result;
			}
		} else {
			return FWG_E_OBJECT_NOT_EXIST_ERROR;
		}
		
		return result;
	}
	
};


#endif // __GAME_PROPERTY_H__