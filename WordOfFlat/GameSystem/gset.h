#ifndef __GAME_SET_CONTAINER_H__
#define __GAME_SET_CONTAINER_H__

	
#include <set>
#include "gerror.h"

template<typename value>
class GameBasSet {
	typedef std::set<value> TInternalSet;
public:
	typedef typename std::set<value>::iterator Iterator;
	typedef typename std::set<value>::const_iterator ConstIterator;

private:
	TInternalSet m_innerSet;
	
public:
	GameBasSet() {}
	~GameBasSet() {}
	
	/*!
	 * \brief Query on key exists
	 * \param Key Unique key
	 * \return True if key exists, false otherwise
	 */
	inline bool Exists(const value& Key)
	{
		return (m_innerSet.find(Key) != m_innerSet.end());	
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
	inline GameErrorCode Insert(const value& Value)
	{
		std::pair<typename TInternalSet::iterator, bool> retval;
			
		retval = m_innerSet.insert(Value);
		if(!retval.second)
		{
			if(Exists(Value))
			{
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			} else {
				return FWG_E_MEMORY_ALLOCATION_ERROR;
			}
		}
		
		return FWG_NO_ERROR;
	}
	
	inline GameErrorCode Insert(const value& Value, Iterator &iter)
	{
		std::pair<typename TInternalSet::iterator, bool> retval;
			
		retval = m_innerSet.insert(Value);
		if(!retval.second)
		{
			if(Exists(Value))
			{
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			} else {
				return FWG_E_MEMORY_ALLOCATION_ERROR;
			}
		}
		
		iter = static_cast<Iterator>(retval.first);
		
		return FWG_NO_ERROR;
	}
	
	inline void Remove(const value& Value)
	{
		m_innerSet.erase(Value);
	}
	
	inline void Remove(Iterator iter)
	{
		m_innerSet.erase(iter);
	}
	
	inline void Remove(ConstIterator iter)
	{
		m_innerSet.erase(iter);
	}
	
	inline Iterator Find(const value& Value)
	{
		return static_cast<Iterator>(m_innerSet.find(Value));
	}
	
	inline value* FindValue(const value& Value)
	{
		typename TInternalSet::iterator iter = m_innerSet.find(Value);
		if(iter == m_innerSet.end())
		{
			return nullptr;
		}
		
		return &(iter->second);	
	}
	
	
	inline Iterator Begin() { return static_cast<Iterator>(m_innerSet.begin()); }
	inline Iterator End() { return static_cast<Iterator>(m_innerSet.end()); }
	
	inline bool IsEmpty() { return m_innerSet.empty(); }
	
	inline void Clear()
	{
		m_innerSet.clear();
	}

};

#endif // __GAME_SET_CONTAINER_H__
