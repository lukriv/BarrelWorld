#ifndef __MEMORY_ALLOCATOR_CONTAINER_H__
#define __MEMORY_ALLOCATOR_CONTAINER_H__

template <class T>
class MemoryAllocContainer {
private:
	static const wxByte VALID = 1;
	static const wxByte INVALID = 0;
private:
	wxVector<T> m_itemVector;
	wxVector<wxDword> m_freeGaps;
	wxVector<wxByte> m_validItem;
	wxDword m_iterIndex;
public:
	MemoryAllocContainer(wxDword expectedSize) {
		
	}
	
	~MemoryAllocContainer() {}
	
	/*!
	 * \brief Prepare new place for item
	 * \param index Returned valid index
	 * \return ErrorCode with error. (FWG_NO_ERROR means success)
	 */
	GameErrorCode AllocNewItem(wxDword &index) {}
	/*!
	 * \brief Return pointer to item with given index
	 * \param index Item index
	 * \return Return NULL if method fails, else it returns valid pointer.
	 */
	T* GetItem(wxDword index) {}
	/*!
	 * \brief Mark item as invalid
	 * \param index Selected item
	 */
	void FreeItem(wxDword index) {}
	/*!
	 * \brief free whole memory
	 */
	void FreeAll() {}
	
	/*!
	 * \brief Reset iterator to first item
	 */
	void Reset() {}
	/*!
	 * \brief Get actual iterator value
	 * \return Value
	 */
	T& GetActualValue() {}
	/*!
	 * \brief Get actual index
	 * \return Index
	 */
	wxDword GetActualIndex() {}
	/*!
	 * \brief Move iterator to next item
	 * \retval FWG_NO_ERROR on success
	 * \retval FWG_E_END_OF_SEQUENCE_INFORMATION if sequence is on last item 
	 */
	GameErrorCode Next() {}
	/*!
	 * \brief Move iterator to previous item
	 * \retval FWG_NO_ERROR on success
	 * \retval FWG_E_END_OF_SEQUENCE_INFORMATION if sequence is on first item 
	 */
	GameErrorCode Prev() {}
	
	
};

#endif //__MEMORY_ALLOCATOR_CONTAINER_H__