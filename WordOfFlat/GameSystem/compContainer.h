#ifndef __COMPONENT_DEFINITIONS_CONTAINER_H__
#define __COMPONENT_DEFINITIONS_CONTAINER_H__


#include <map>
#include <wx/defs.h>
#include <wx/vector.h>
#include <wx/string.h>


template<class idT, class T>
class CompDefContainer {
	typedef std::map<idT,wxDword> TIdConvertTable;
	typedef std::pair<idT,wxDword> TIdConvertTablePair;
	
	typedef std::map<wxString,wxDword> TStringConvertTable;
	typedef std::pair<wxString,wxDword> TStringConvertTablePair;	

private:
	wxVector<T> m_itemsList;
	TIdConvertTable m_idConvertTable;
	TStringConvertTable m_stringConvertTable; 	

	T m_nullItem;
	
public:
	CompDefContainer() {}
	~CompDefContainer() {}
	
	/*!
	 * \brief Query on item exists
	 * \param itemId Unique item id
	 * \return True if item exists, false otherwise
	 */
	bool ItemExists(const idT& itemId)
	{
		return (m_idConvertTable.find(itemId) != m_idConvertTable.end());	
	}
	
	/*!
	 * \brief Query on item exists
	 * \param itemName Unique name of item
	 * \return True if item exists, false otherwise
	 */
	bool ItemExists(const wxString& itemName)
	{
		return (m_stringConvertTable.find(itemName) != m_stringConvertTable.end());
	}
	
	/*!
	 * \brief Add new item to container
	 * \param itemName Unique item name
	 * \param itemId Unique item id
	 * \param item Added item
	 * \retval FWG_NO_ERROR On success
	 * \retval FWG_E_MEMORY_ALLOCATION_ERROR If adding failed
	 * \retval errorcode on fail
	 */
	GameErrorCode AddItem(const wxString& itemName, const idT& itemId, const T& item)
	{
		std::pair<TIdConvertTable::iterator, bool> retvalIdConvert;
		std::pair<TStringConvertTable::iterator, bool> retvalStrConvert;
		
		if((ItemExists(itemName))||(ItemExists(itemId)))
		{
			return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
		}
		
		wxDword index = m_itemsList.size();
		m_itemsList.push_back(item);
		
		retvalIdConvert = m_idConvertTable.insert(TIdConvertTablePair(itemId, index));
		if(!retvalIdConvert.second)
		{
			m_itemsList.pop_back();
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		retvalStrConvert = m_stringConvertTable.insert(TStringConvertTablePair(itemName, index));
		if(!retvalStrConvert.second)
		{
			m_itemsList.pop_back();
			m_idConvertTable.erase(retvalIdConvert.first);
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
		
		return FWG_NO_ERROR;
	}
	
	T& GetItem(const wxString& itemName)
	{
		TStringConvertTable::iterator iter = m_stringConvertTable.find(itemName);
		wxDword index = 0;
		if(iter != m_stringConvertTable.end())
		{
			return m_itemsList[iter->second];
		}
		
		return m_nullItem;
	}
	
	
	T& GetItem(const idT& itemId)
	{
		TIdConvertTable::iterator iter = m_idConvertTable.find(itemId);
		wxDword index = 0;
		if(iter != m_idConvertTable.end())
		{
			return m_itemsList[iter->second];
		}
		
		return m_nullItem;
	}

};

#endif // __COMPONENT_DEFINITIONS_CONTAINER_H__
