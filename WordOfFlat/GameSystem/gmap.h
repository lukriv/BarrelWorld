#ifndef __GAME_MAP_CONTAINER_H__
#define __GAME_MAP_CONTAINER_H__


#include <map>
#include "gerror.h"

template<typename key, typename value, typename compare = std::less<key>>
class GameBasMap {
	typedef std::map<key,value,compare> TInternalMap;
	typedef std::pair<key,value> TInternalMapPair;
public:
	typedef typename std::map<key,value,compare>::iterator Iterator;
	typedef typename std::map<key,value,compare>::const_iterator ConstIterator;

private:
	TInternalMap m_innerMap;
	
public:
	GameBasMap() {}
	~GameBasMap() {}
	
	/*!
	 * \brief Query on key exists
	 * \param Key Unique key
	 * \return True if key exists, false otherwise
	 */
	inline bool Exists(const key& Key)
	{
		return (m_innerMap.find(Key) != m_innerMap.end());	
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
	inline GameErrorCode Insert(const key& Key, const value& Value)
	{
		std::pair<typename TInternalMap::iterator, bool> retval;
			
		retval = m_innerMap.insert(TInternalMapPair(Key, Value));
		if(!retval.second)
		{
			if(Exists(Key))
			{
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			} else {
				return FWG_E_MEMORY_ALLOCATION_ERROR;
			}
		}
		
		return FWG_NO_ERROR;
	}
	
	inline GameErrorCode Insert(const key& Key, const value& Value, Iterator &iter)
	{
		std::pair<typename TInternalMap::iterator, bool> retval;
			
		retval = m_innerMap.insert(TInternalMapPair(Key, Value));
		if(!retval.second)
		{
			if(Exists(Key))
			{
				return FWG_E_OBJECT_ALREADY_EXISTS_ERROR;
			} else {
				return FWG_E_MEMORY_ALLOCATION_ERROR;
			}
		}
		
		iter = static_cast<Iterator>(retval.first);
		
		return FWG_NO_ERROR;
	}
	
	inline void Remove(const key& Key)
	{
		m_innerMap.erase(Key);
	}
	
	inline void Remove(Iterator iter)
	{
		m_innerMap.erase(iter);
	}
	
	inline void Remove(ConstIterator iter)
	{
		m_innerMap.erase(iter);
	}
	
	inline Iterator Find(const key& Key)
	{
		return static_cast<Iterator>(m_innerMap.find(Key));
	}
	
	inline value* FindValue(const key& Key)
	{
		typename TInternalMap::iterator iter = m_innerMap.find(Key);
		if(iter == m_innerMap.end())
		{
			return NULL;
		}
		
		return &(iter->second);	
	}
	
	inline ConstIterator Begin() const { return static_cast<ConstIterator>(m_innerMap.begin()); }
	inline ConstIterator End() const { return static_cast<ConstIterator>(m_innerMap.end()); }
	
	inline Iterator Begin() { return static_cast<Iterator>(m_innerMap.begin()); }
	inline Iterator End() { return static_cast<Iterator>(m_innerMap.end()); }
	
	inline bool IsEmpty() { return m_innerMap.empty(); }
	
	inline size_t Size() { return m_innerMap.size(); }
	
	inline void Clear()
	{
		m_innerMap.clear();
	}
	

};

#endif // __GAME_MAP_CONTAINER_H__
