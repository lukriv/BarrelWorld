#ifndef __MEMORY_ALLOCATOR_CONTAINER_H__
#define __MEMORY_ALLOCATOR_CONTAINER_H__


/*! \brief Simple memory allocator
 * 
 * Memory allocator for allocation simple struct objects.
 */
template <class T>
class MemoryAllocContainer {
public:
	static const wxDword INVALID_INDEX = wxUINT32_MAX;
private:
	static const wxByte VALID = 1;
	static const wxByte INVALID = 0;
private:
	wxVector<T> m_itemVector;
	wxVector<wxDword> m_freeGaps;
	wxVector<wxByte> m_validItem;
	wxDword m_iterIndex;
	wxDword m_itemCount;
	wxCriticalSection m_criticalSection;
public:
	MemoryAllocContainer(wxDword expectedSize) : m_iterIndex(0), m_itemCount(0) {
		if (expectedSize > 0)
		{
			m_itemVector.reserve(expectedSize);
			m_validItem.reserve(expectedSize);
		}
	}
	
	~MemoryAllocContainer() {}
	
	/*!
	 * \brief Prepare new place for item
	 * \param index Returned valid index
	 * \return ErrorCode with error. (FWG_NO_ERROR means success)
	 */
	GameErrorCode AllocNewItem(wxDword &index) 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		if(m_freeGaps.empty())
		{
			index = m_itemVector.size();
			m_itemVector.push_back(T());
			m_validItem.push_back(1);
		} else {
			index = m_freeGaps.back();
			m_validItem[index] = 1;
			m_freeGaps.pop_back();
		}
		m_itemCount++;
		return FWG_NO_ERROR;
	}
	/*!
	 * \brief Return pointer to item with given index
	 * \param index Item index
	 * \return Return NULL if method fails, else it returns valid pointer.
	 */
	inline T* GetItem(wxDword index) 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		if((index < m_itemVector.size()) && (m_validItem[index] == 1))
		{
			return &m_itemVector[index];
		} else {
			return NULL;
		}
	}
	/*!
	 * \brief Mark item as invalid
	 * \param index Selected item
	 */
	void FreeItem(wxDword index) 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		if((index < m_itemVector.size())
		{
			if (m_validItem[index] == VALID)
			{
				m_itemCount--;
				m_validItem[index] = 0;
			}
			
		}
	}
	/*!
	 * \brief free whole memory
	 */
	void FreeAll() 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		wxVector<wxByte>::iterator iter;
		for(iter = m_validItem.begin(); iter != m_validItem.end(); iter++)
		{
			*iter = 0;
		}
	}
	
	/*!
	 * \brief Reset iterator to first item
	 * \retval FWG_NO_ERROR on success
	 * \retval FWG_E_END_OF_SEQUENCE_INFORMATION if sequence is empty
	 */
	GameErrorCode Reset() 
	{
		m_iterIndex = 0;
		wxCriticalSectionLocker lock(m_criticalSection);
		while ((m_iterIndex < m_validItem.size())&&(m_validItem[m_iterIndex] != 1))
		{
			m_iterIndex++;
		}
		
		if (m_validItem.empty || (m_validItem.size() >= m_iterIndex))
		{
			return FWG_E_END_OF_SEQUENCE_INFORMATION;
		}
		
		return FWG_NO_ERROR;
	}
	/*!
	 * \brief Get actual iterator value
	 * \return Value
	 */
	inline T& GetActualValue() 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		return m_itemVector[m_iterIndex];
	}
	/*!
	 * \brief Get actual index
	 * \return Index
	 */
	inline wxDword GetActualIndex() 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		return m_iterIndex;
	}
	
	/*!
	 * \brief Ask for actual index item
	 * \return 
	 */
	inline bool IsActualValid()
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		return ((m_iterIndex < m_validItem.size())&&(m_validItem[m_iterIndex] == VALID));
	}
	/*!
	 * \brief Move iterator to next item
	 * \retval FWG_NO_ERROR on success
	 * \retval FWG_E_END_OF_SEQUENCE_INFORMATION if sequence is on last item 
	 */
	GameErrorCode Next() 
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		while ((m_iterIndex < m_validItem.size())&&(m_validItem[m_iterIndex] != 1))
		{
			m_iterIndex++;
		}
		
		if (m_iterIndex >= m_validItem.size())
		{
			return FWG_E_END_OF_SEQUENCE_INFORMATION;
		}
		
		return FWG_NO_ERROR;

	}

	inline wxDword GetSize()
	{
		wxCriticalSectionLocker lock(m_criticalSection);
		return m_itemCount;
	}
	
	
};

#endif //__MEMORY_ALLOCATOR_CONTAINER_H__