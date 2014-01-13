#ifndef __GAME_SCENE_MAP_H__
#define __GAME_SCENE_MAP_H__

#include <map>
#include <iostream>
#include <wx/defs.h>
#include <wx/string.h>
#include "actions.h"
#include "eventprop.h"

class Character;

struct ShopArticle {
	EItem m_item;
	wxInt32 m_goldCount;
public:
	ShopArticle() : m_item(ITEM_UNKNOWN), m_goldCount(0) {} 
};

class Scene {
public:
	typedef wxVector<EItem> TItemList;
	typedef wxVector<ShopArticle> TArticleVector;
private:
	typedef std::map<EItem, wxInt32> TArticleMap;
	typedef std::pair<EItem, wxInt32> TArticleMapPair;
private:
	wxInt32 m_sceneId;
	TItemList m_sceneItems;
	wxInt32 m_goldCount;
	TArticleVector m_goodsToBuy;
	TArticleMap m_goodsToSell;
public:
	wxString m_desc;
	ActionVector m_actions;
	EventVector m_events;
public:
	Scene() : m_sceneId(TARGET_UNKNOWN), m_goldCount(0) {}
	~Scene() 
	{
		for(ActionVector::iterator iter = m_actions.begin(); iter != m_actions.end(); iter++)
		{
			if(*iter != NULL)
			{
				delete (*iter);
				*iter = NULL;
			}
		}
		
		for(EventVector::iterator iter = m_events.begin(); iter != m_events.end(); iter++)
		{
			if(*iter != NULL)
			{
				delete (*iter);
				*iter = NULL;
			}	
		}
	}
	
	inline void SetSceneId(wxInt32 id) { m_sceneId = id; }
	inline wxInt32 GetSceneId() const { return m_sceneId; }
	
	/*!
	 * \brief Get Indexes to posible actions with given character
	 * \param character
	 * \param outPosibleActions
	 */
	void GetPosibleActions(Character& character, wxVector<wxDword> &outPosibleActions);
	
	
	/*!
	 * \brief Add item to scene
	 * \param item Item to add
	 */ 
	void AddItem(EItem item);
	
	/*!
	 * \brief Remove item from the scene
	 * \param item
	 */
	void RemoveItem(EItem item);
	
	/*!
	 * \brief Get copy of item list of this scene
	 * \param sceneItemList
	 */
	void GetItemList(TItemList &sceneItemList);
	
	bool ContainsItem(EItem item);
	
	/*!
	 * \brief Add item to scene
	 * \param item Item to add
	 */ 
	void AddItemToBuy(EItem item, wxInt32 price);
	
	/*!
	 * \brief Remove item from the scene
	 * \param item
	 * \return Price of removed item
	 */
	wxInt32 RemoveItemToBuy(EItem item);
	
	wxInt32 GetItemToBuyPrice(EItem item);
	
	/*!
	 * \brief Get copy of item list of this scene
	 * \param sceneItemList
	 */
	void GetItemToBuyList(TArticleVector &sceneItemList);
	
	bool ContainsItemToBuy(EItem item);
	
	/*!
	 * \brief Add item to scene
	 * \param item Item to add
	 * \return true If adding was successful, false otherwise
	 */ 
	bool AddItemToSell(EItem item, wxInt32 price);
	
	/*!
	 * \brief Remove item from the scene
	 * \param item
	 * \return Price of sell item or zero if item cannot be sell in scene
	 */
	wxInt32 GetItemToSellPrice(EItem item);
	
	/*!
	 * \brief Get copy of item list of this scene
	 * \param sceneItemList
	 */
	void GetItemToSellList(TArticleVector &sceneItemList);
	
	bool ContainsItemToSell(EItem item);
	
	bool AddGold(wxInt32 goldCount) 
	{ 
		if((m_goldCount+goldCount) < 0)
		{
			return false; 
		} else {
			m_goldCount += goldCount;
			return true;
		}
	}
	
	bool SetGold(wxInt32 goldCount) 
	{ 
		if(goldCount < 0)
		{
			return false; 
		} else {
			m_goldCount = goldCount;
			return true;
		}
	}
	
	wxInt32 GetGoldCount() { return m_goldCount;}
	
};




class SceneManager {
private:
	typedef std::map<wxDword, Scene*> TSceneMap;
	typedef std::pair<wxDword, Scene*> TSceneMapPair;
private:
	wxInt32 m_id;
	TSceneMap m_sceneMap;
	
	friend std::ostream& operator<< (std::ostream &stream, const SceneManager& scene);
	friend std::wostream& operator<< (std::wostream &stream, const SceneManager& scene);
	
public:
	SceneManager() : m_id(0) {}
	~SceneManager() 
	{
		for(TSceneMap::iterator iter = m_sceneMap.begin(); iter != m_sceneMap.end(); iter++)
		{
			if(iter->second != NULL)
			{
				delete iter->second;
			}
		}
	}
	
	void SetId(wxInt32 id) { m_id = id;}
	inline wxInt32 GetId() { return m_id;}
	
	bool AddScene(Scene* pScene);
	void RemoveScene(wxDword sceneId);
	Scene* GetScene(wxDword sceneId);
	
	//scene map testing
	/*! 
		\brief Test to chapter completness
	
		It tests to missing scene IDs - no ID should be missing and it should be incremental sequence.
		Test if all scenes is reacheable from scene 0.
		
		\retval true All testing cases pass.
		\retval false Test fail.
	*/
	bool SceneMapTest();
	
};

std::ostream& operator<< (std::ostream &stream, const Scene& scene);
std::ostream& operator<< (std::ostream &stream, const SceneManager& scene);

std::wostream& operator<< (std::wostream &stream, const Scene& scene);
std::wostream& operator<< (std::wostream &stream, const SceneManager& scene);

#endif //__GAME_SCENE_MAP_H__