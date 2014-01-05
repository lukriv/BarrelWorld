#ifndef __LONE_WOLF_EVENT_PROPERTIES_H__
#define __LONE_WOLF_EVENT_PROPERTIES_H__



#include "lwdefs.h"
#include <wx/vector.h>


struct ItemProperties {
	wxInt32 m_actualCond;
	wxInt32 m_maxCond;
	bool m_oneUse; // item is only for one use
public:
	ItemProperties() : m_actualCond(0), m_maxCond(0) {}
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
						, m_duration(0)
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
	EItem m_neededItem; // event is valid if this item is available
	EDisciplines m_neededSkill; // event is valid if this skill is available
	bool m_fightSkill; // discipline is/is not available in fight
public:
	EventProperties () : m_actualCond(0)
						, m_actualAttack(0)
						, m_cancelItem(ITEM_UNKNOWN)
						, m_cancelSkill(DISCIPLINE_UNKNOWN)
						, m_neededItem(ITEM_UNKNOWN)
						, m_neededSkill(DISCIPLINE_UNKNOWN)
						, m_fightSkill(false) {}
};


class EventBase {
public:
	virtual EEventType GetEventType() = 0;
	virtual EventProperties* GetProperties() = 0;
	virtual	EventBase* GetRandomEvent(wxInt32 randomNumber) = 0;
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
};


/*!
 * \class EventList
 * \author Lukas
 * \date 01/05/14
 * \file eventprop.h
 * \brief Scene events
 * This events contains 10 events which and can get random of this events
 */
struct EventList : public EventBase {
	EEventType m_type;
	wxVector<Event> m_eventList;
public:
	EventList() : m_type(EVENT_UNKNOWN) {}
	virtual EEventType GetEventType() {return m_type; }
	virtual EventProperties* GetProperties() { return NULL; }
	virtual	EventBase* GetRandomEvent(wxInt32 randomNumber) 
	{
		if((randomNumber >= 0)&&(static_cast<wxDword>(randomNumber) < m_eventList.size()))
		{
			return &(m_eventList[randomNumber]);
		}
		return NULL;
	}
	
	void SetLoteryEvents(wxDword fromId, wxDword toId, const Event& event );
};

typedef wxVector<EventBase*> EventVector;

#endif //__LONE_WOLF_EVENT_PROPERTIES_H__