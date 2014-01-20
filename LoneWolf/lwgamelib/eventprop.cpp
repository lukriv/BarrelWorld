#include "eventprop.h"


bool EventList::SetLoteryEvents(wxDword fromId, wxDword toId, EventBase* pEvent)
{
	if(toId < fromId) return false;
	if(toId > 9) return false;
	
	if(pEvent == NULL)
	{
		return false;
	}
	
	if( m_randomList.empty() )
	{
		m_randomList.resize(10,0);
	}
	
	// set event for destruction
	m_eventList.push_back(pEvent);
	
	// set event for random calling (no deleting is provided on m_randomList vector
	for (wxDword i = fromId; i <= toId; i++)
	{
		m_randomList[i] = pEvent;
	}
	
	return true;
}
