#include "eventprop.h"


void EventList::SetLoteryEvents(wxDword fromId, wxDword toId, const Event& event )
{
	if(toId < fromId) return;
	if(toId > 9) return;
	
	if ( m_eventList.empty() )
	{
		m_eventList.resize(10);
	}
	
	for (wxDword i = fromId; i <= toId; i++)
	{
		m_eventList[i] = event;
	}
}