#ifndef __CIRCULAR_QUEUE_H__
#define __CIRCULAR_QUEUE_H__

#include "gerror.h"

template <class T>
class GameCircularQueue {
	wxVector<T> m_storage;
	wxDword m_front;
	wxDword m_back;
	wxDword m_maxSize;
	wxDword m_sizeRep;
	wxDword m_actualSize;
public:
	GameCircularQueue(wxDword maxSize = 128): m_front(0), m_back(0), m_actualSize(0) 
	{
		wxDword aproxSize = 1;
		while(maxSize > aproxSize) {
			aproxSize = aproxSize << 1;
		}
		m_maxSize = aproxSize;
		m_sizeRep = aproxSize - 1;
		
		m_storage.resize(m_maxSize);
	}
	~GameCircularQueue() {}
	
	T& front() {
		return m_storage[m_front];
	}
	
	const T& front() const {
		return m_storage[m_front];
	}
	
	T& back() {
		return m_storage[m_back];
	}
	
	const T& back() const {
		return m_storage[m_back];
	}
	
	GameErrorCode push(const T& value) {
		if (m_maxSize == m_actualSize)
		{
			return FWG_E_FULL_BUFFER_ERROR;
		}
		
		m_storage[m_back] = value;
		m_actualSize++;
		m_back = (m_back + 1) & m_sizeRep;
		return FWG_NO_ERROR;
	}
	
	void pop() {
		if (m_actualSize > 0)
		{
			m_front = (m_front + 1) & m_sizeRep;
			m_actualSize--;			
		}
	}
	
	bool isEmpty()
	{
		return (m_actualSize == 0);
	}
	
};



#endif //__CIRCULAR_QUEUE_H__