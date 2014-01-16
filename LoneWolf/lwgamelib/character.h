#ifndef __LONE_WOLF_CHARACTER_H__
#define __LONE_WOLF_CHARACTER_H__


#include <set>
#include <wx/defs.h>
#include "lwdefs.h"
#include "lwglobres.h"
#include "scenemgr.h"


class Character;

template <class T>
class CharacterContainer {
public:
	typedef typename std::multiset<T>::iterator Iterator;
private:
	std::multiset<T> m_container;
	wxDword m_maxItems;

public:
	CharacterContainer() : m_maxItems(0) {}
	
	/*!
	 * \brief Set max items restriction
	 * 
	 * Set maximum items count. If container is full (contains non-zero values) mehtod try to resize container
	 * 
	 * 
	 * \param maxItems
	 * \retval true Setting max items was successful
	 * \retval false Container is full, you must remove some items to do this action
	 */
	bool SetMaxCount(wxDword maxItems) 
	{
		if(maxItems >= m_container.size())
		{
			m_maxItems = maxItems;
			return true;
		}
		
		return false;
	}

	inline wxDword GetMaxCount() {return m_maxItems;}
	
	/*!
	 * \brief Add item to container
	 * \param item Added item
	 * \retval true Item was successfully added to container
	 * \retval false Container is full (remove some item from container or increase max items)
	 */
	bool Add(T item)
	{
		if (m_container.size() < m_maxItems)
		{
			m_container.insert(item);
			return true;
		}
		
		return false;
	}
	
	/*!
	 * \brief Remove item from container
	 * \param item Removed item
	 * \retval true One item was successfully removed from container
	 * \retval false Item was not found in container (no item was removed)
	 */
	bool Remove(T item)
	{
		Iterator iter = m_container.find(item);
		if (iter != End())
		{
			m_container.erase(iter);
			return true;
		}
		
		return false;
	}
	
	/*!
	 * \brief Find first occurence of item
	 * \param item Item to find
	 * \retval Iterator to item
	 * \retval Iterator to end() if item is not contained in container
	 */
	inline Iterator Find(T item) { return m_container.find(item); }
	
	inline bool Contains(T item)
	{
		return (m_container.find(item) != m_container.end());
	}
	
	inline Iterator Begin() const { return static_cast<Iterator>(m_container.begin()); }
	inline Iterator End() const { return static_cast<Iterator>(m_container.end()); }	
	inline bool IsEmpty() const { return m_container.empty(); }
	inline bool IsFull() const { return m_container.size() == m_maxItems; }
	inline wxDword Size() const { return m_container.size(); }
	
	/*!
	 * \brief Empty whole container
	 */
	void Clear() 
	{
		m_container.clear();
	}
	
};


class CharacterDisciplines {
private:
	// private types
	typedef std::map<EDisciplines,DisciplineProperties> TDiscInfo;
	typedef std::pair<EDisciplines,DisciplineProperties> TDiscInfoPair;
public:
	// public types
	typedef TDiscInfo::const_iterator Iterator;

private:
	// private values
	TDiscInfo m_container;
public:
	CharacterDisciplines() {}
	
	/*!
	 * \brief Add item to container
	 * \param item Added item
	 * \retval true Item was successfully added to container
	 * \retval false Item already exists in container
	 */
	bool Add(EDisciplines disc, const DisciplineProperties& prop)
	{
		std::pair<TDiscInfo::iterator, bool> retval;
		retval = m_container.insert(TDiscInfoPair(disc, prop));
		return retval.second;
	}
	
	/*!
	 * \brief Remove item from container
	 * \param item Removed item
	 * \retval true One item was successfully removed from container
	 * \retval false Item was not found in container (no item was removed)
	 */
	bool Remove(EDisciplines item)
	{
		TDiscInfo::iterator iter = m_container.find(item);
		if (iter != End())
		{
			m_container.erase(iter);
			return true;
		}
		
		return false;
	}
	
	/*!
	 * \brief Find first occurence of item
	 * \param item Item to find
	 * \retval Iterator to item
	 * \retval Iterator to end() if item is not contained in container
	 */
	inline Iterator Find(EDisciplines item) { return m_container.find(item); }
	
	DisciplineProperties* FindValue(EDisciplines item) 
	{
		TDiscInfo::iterator iter = m_container.find(item);
		if(iter == m_container.end())
		{
			return NULL;
		}
		return &(iter->second);
	}
	
	inline bool Contains(EDisciplines item)
	{
		return (m_container.find(item) != m_container.end());
	}
	
	inline Iterator Begin() const { return static_cast<Iterator>(m_container.begin()); }
	inline Iterator End() const { return static_cast<Iterator>(m_container.end()); }	
	inline bool IsEmpty() const { return m_container.empty(); }
	
	inline wxDword Size() const { return m_container.size(); }
	/*!
	 * \brief Empty whole container
	 */
	void Clear() 
	{
		m_container.clear();
	}
};


class CharacterBody {
	EItem m_head;
	EItem m_torso;
	EItem m_back;
public:
	CharacterBody() : m_head(ITEM_UNKNOWN), m_torso(ITEM_UNKNOWN), m_back(ITEM_UNKNOWN) {}

	EItem GetHeadItem() const { return m_head; }
	EItem GetTorsoItem() const { return m_torso; }
	EItem GetBackItem() const { return m_back; }
	
	void SetHeadItem(EItem item) { m_head = item; }
	void SetTorsoItem(EItem item) { m_torso = item; }
	void SetBackItem(EItem item) { m_back = item; }
};


class Character {
public:
	typedef CharacterContainer<EItem> CharacterBackpack;
	typedef CharacterContainer<EItem> CharacterWeapons;
	typedef CharacterContainer<EItem> CharacterSpecialItems;
private:
	// disciplines
	GlobalResourceManager *m_pResMgr;
	
	wxString m_charName;
	
	wxInt32 m_pouch;
	wxInt32 m_maxGoldCount;
	
	wxInt32 m_baseAttackSkill;
	wxInt32 m_actualAttackSkill;
	
	wxInt32 m_baseCondition;
	wxInt32 m_maxCondition;
	wxInt32 m_actualCondition;

	CharacterBackpack m_backpack;
	CharacterWeapons m_weapons;
	CharacterDisciplines m_disciplines;
	CharacterSpecialItems m_specialItems;
	CharacterBody m_body;
	
public:
	Character(): m_pResMgr(NULL)
				, m_pouch(0)
				, m_maxGoldCount(0)
				, m_baseAttackSkill(0)
				, m_actualAttackSkill(0)
				, m_baseCondition(0)
				, m_maxCondition(0)
				, m_actualCondition(0) {}
	~Character() {}

	bool Initialize(GlobalResourceManager *pResMgr);
	bool GenerateNewCharacter();
	bool AddNewCharacterDiscipline(EDisciplines disc, DisciplineProperties *pDiscProp = NULL);
	
	inline wxInt32 GetMaxGoldCount() {return m_maxGoldCount;}
	inline wxInt32 GetGoldCount() { return m_pouch; }
	
	inline void SetCharacterName(const wxString& name) { m_charName = name; }
	inline const wxString& GetCharacterName() const { return m_charName; }
	
	/*!
	 * \brief Add gold to pouch
	 * \param goldCount
	 * \return Count of gold that you cannot add to pouch because of its capacity (if returns negative values you dont hava enough money in pouch)
	 */
	wxInt32 AddGold(wxInt32 goldCount);
	
	/*!
	 * \brief Set gold to pouch
	 * \param goldCount
	 * \return Count of gold that you cannot add to pouch because of its capacity
	 */
	wxInt32 SetGold(wxInt32 goldCount);
	
	inline wxInt32 AddActualCondition(wxInt32 condPoints) 
	{
		m_actualCondition = ((m_actualCondition + condPoints) > m_maxCondition)? m_maxCondition : (m_actualCondition + condPoints); 
		return m_actualCondition;
	}
	
	inline void SetBaseConditions(wxInt32 baseCond) 
	{
		m_actualCondition = baseCond;
		m_baseCondition = baseCond;
		m_maxCondition = baseCond;
	}
	
	inline void SetBaseAttackSkill(wxInt32 baseAttack)
	{
		m_actualAttackSkill = baseAttack;
		m_baseAttackSkill = baseAttack;
	}
	
	inline wxInt32 GetBaseConditions() {return m_baseCondition;}
	inline wxInt32 GetMaxConditions() { return m_maxCondition; }
	inline wxInt32 GetActualConditions() { return m_actualCondition;}
		
	inline wxInt32 GetBaseAttackSkill() {return m_baseAttackSkill;}
	
	
	// fight methods
	
	/*!
	 * \brief Reset attact skill to default
	 * 
	 * This is used at beginnig of fight turn, before all events and skill are applied on character
	 * 
	 */
	inline void ResetActualAttackSkill() { m_actualAttackSkill = m_baseAttackSkill; }
	
	
	/*!
	 * \brief Apply skills during non-fight scene
	 */
	void ApplySkills();
	
	/*!
	 * \brief Apply all skill modificators during figth
	 *
	 * Effect of skill can be canceled by enemy skill or item.
	 *
	 * \param enemyCharacter
	 */
	void ApplySkills(Character &enemyCharacter);
	
	/*!
	 * \brief Apply items which affects attack skill
	 */
	void ApplyFightItems();
	
	/*!
	 * \brief Get result attack skill affter application all events and skills
	 * \return 
	 */
	inline wxInt32 GetActualAttackSkill() {return m_actualAttackSkill;}
	
	/*!
	 * \brief Try all containers if any has required item
	 * \param item
	 * \return true if item is present
	 */
	bool ContainsItem(EItem item);
	
	/*!
	 * \brief Check if character has weapon with given weapon type
	 * \param weapon
	 * \return 
	 */
	bool ContainsWeaponClass(EWeaponClass weapon);
	
	/*!
	 * \brief Get list of items which can be used freely
	 * \param itemList
	 */
	void GetFreeUseItemList(wxVector<EItem> &itemList);
	
	bool BuyItem(EItem item, Scene& scene);
	bool SellItem(EItem item, Scene& scene);
	bool PickUpItem(EItem item, Scene& scene);
	bool DropItem(EItem item, Scene& scene);
	/*!
	 * \brief Pick up all gold from the scene which character can
	 * \param scene
	 * \return Amount of picked up gold
	 */
	wxInt32 PickUpGold(Scene& scene);
	
	bool AddItem(EItem item);
	bool LoseItem(EItem item);
	bool UseItem(EItem item);

	inline CharacterBackpack& GetBackPack() {return m_backpack;} 
	inline CharacterWeapons& GetWeapons() {return m_weapons;}
	inline CharacterDisciplines& GetDisciplines() {return m_disciplines;}
	inline CharacterSpecialItems& GetSpecialItems() {return m_specialItems;}
	inline CharacterBody& GetBody() { return m_body; }
	
	bool ApplyEvent(Event& event);
	
	void RecomputeState();
	
};




#endif //__LONE_WOLF_CHARACTER_H__