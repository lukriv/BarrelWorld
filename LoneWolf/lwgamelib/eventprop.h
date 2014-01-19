#ifndef __LONE_WOLF_EVENT_PROPERTIES_H__
#define __LONE_WOLF_EVENT_PROPERTIES_H__



#include "lwdefs.h"
#include <wx/vector.h>


struct ItemProperties {
	wxInt32 m_actualAttack; //mainly for weapons or other fight accessories
	wxInt32 m_actualCond;
	wxInt32 m_maxCond;
	bool m_oneUse; // item is only for one use - this could be spent by event or manual use
	bool m_freeUse; // item can be used freely on user request
public:
	ItemProperties() : m_actualAttack(0), m_actualCond(0), m_maxCond(0) {}
};

struct EventProperties {
	wxInt32 m_actualCond;
	wxInt32 m_baseCond;
	wxInt32 m_actualAttack;
	wxInt32 m_baseAttack;
	wxInt32 m_duration;
	wxInt32 m_goldCount;
	EItem m_cancelItem; // event is invalid if this item is available (cancel item has more priority than needed item)
	EDisciplines m_cancelSkill; // event is invalid if this skill is available (cancel skill has more prioriry than needed skill)
	EItem m_neededItem; // event is valid if this item is available
	EDisciplines m_neededSkill; // event is valid if this skill is available
public:
	EventProperties () : m_actualCond(0)
						, m_baseCond(0)
						, m_actualAttack(0)
						, m_baseAttack(0)
						, m_duration(DURATION_INFINITE)
						, m_goldCount(0)
						, m_cancelItem(ITEM_UNKNOWN)
						, m_cancelSkill(DISCIPLINE_UNKNOWN)
						, m_neededItem(ITEM_UNKNOWN)
						, m_neededSkill(DISCIPLINE_UNKNOWN) {}
};

struct DisciplineProperties {
	wxInt32 m_actualCond;
	wxInt32 m_actualAttack;
	EItem m_cancelItem; // event is invalid if this item is available (cancel item has more priority than needed item)
	EDisciplines m_cancelSkill; // event is invalid if this skill is available (cancel skill has more prioriry than needed skill)
	wxVector<EWeaponClass> m_weaponClass; // event is valid if this item is available
	EDisciplines m_neededSkill; // event is valid if this skill is available
	bool m_fightSkill; // discipline is/is not available in fight
public:
	DisciplineProperties () : m_actualCond(0)
						, m_actualAttack(0)
						, m_cancelItem(ITEM_UNKNOWN)
						, m_cancelSkill(DISCIPLINE_UNKNOWN)
						, m_neededSkill(DISCIPLINE_UNKNOWN)
						, m_fightSkill(false) {}
	
	DisciplineProperties (const DisciplineProperties& discProp) : m_actualCond(discProp.m_actualCond)
						, m_actualAttack(discProp.m_actualAttack)
						, m_cancelItem(discProp.m_cancelItem)
						, m_cancelSkill(discProp.m_cancelSkill)
						, m_weaponClass(discProp.m_weaponClass)
						, m_neededSkill(discProp.m_neededSkill)
						, m_fightSkill(discProp.m_fightSkill) {}
	
	DisciplineProperties& operator=(const DisciplineProperties& discProp)
	{
		if(this == &discProp) return *this;
		wxVector<EWeaponClass> tempVec(discProp.m_weaponClass);
		m_actualCond = discProp.m_actualCond;
		m_actualAttack = discProp.m_actualAttack;
		m_cancelItem = discProp.m_cancelItem;
		m_cancelSkill = discProp.m_cancelSkill;
		m_neededSkill = discProp.m_neededSkill;
		m_weaponClass.swap(tempVec);
		m_fightSkill = discProp.m_fightSkill;
		
		return *this;
	}
};


class EventBase {
public:
	virtual EEventType GetEventType() = 0;
	virtual EventProperties* GetProperties() = 0;
	virtual	EventBase* GetRandomEvent(wxInt32 randomNumber) = 0;
	virtual EventBase* GetEvent( wxDword index) = 0;
	virtual wxDword GetSize() = 0;
	
	virtual ~EventBase() {}
};

/*!
 * \class Event
 * \author Lukas
 * \date 01/05/14
 * \file eventprop.h
 * \brief Scene events are defined here
 * 
 * EVENT_ADD_ITEM_TO_CHARACTER - adds item to character, added item is defined as 'item_needed' property parameter
 * EVENT_ADD_ITEM_TO_SCENE - adds item to scene, added item is defined as 'item_needed' property parameter
 */
struct Event : public EventBase {
	EEventType m_type;
	EventProperties m_property;
public:
	Event() : m_type(EVENT_UNKNOWN) {}
	virtual EEventType GetEventType() {return m_type; }
	virtual EventProperties* GetProperties() { return &m_property; }
	virtual	EventBase* GetRandomEvent(wxInt32 randomNumber) { return NULL; }
	virtual EventBase* GetEvent( wxDword index) { return NULL; }
	virtual wxDword GetSize() { return 0; }
};


typedef wxVector<EventBase*> EventVector;

/*!
 * \class EventList
 * \author Lukas
 * \date 01/05/14
 * \file eventprop.h
 * \brief Scene events
 * EVENT_LOTERY - This events contains 10 events which and can get random of this events
 * EVENT_LIST - This is a list of events (it has arbitrary size)
 *
 */
struct EventList : public EventBase {
	EEventType m_type;
	wxVector<EventBase*> m_eventList;
	wxVector<EventBase*> m_randomList;
public:
	EventList() : m_type(EVENT_UNKNOWN) {}
	~EventList() 
	{
		for (wxVector<EventBase*>::iterator iter = m_eventList.begin(); iter != m_eventList.end(); iter++)
		{
			if((*iter) != 0)
			{
				delete (*iter);
				*iter = NULL;
			}
		}
	}
	virtual EEventType GetEventType() {return m_type; }
	virtual EventProperties* GetProperties() { return NULL; }
	virtual	EventBase* GetRandomEvent(wxInt32 randomNumber) 
	{
		if((randomNumber >= 0)&&(static_cast<wxDword>(randomNumber) < m_randomList.size()))
		{
			return m_randomList[randomNumber];
		}
		return NULL;
	}
	
	bool SetLoteryEvents(wxDword fromId, wxDword toId, EventBase* event);
	virtual EventBase* GetEvent( wxDword index) 
	{
		if(index < m_eventList.size())
		{
			return m_eventList[index];
		}
		return NULL; 
	}
	virtual wxDword GetSize() { return m_eventList.size(); }
};

#endif //__LONE_WOLF_EVENT_PROPERTIES_H__