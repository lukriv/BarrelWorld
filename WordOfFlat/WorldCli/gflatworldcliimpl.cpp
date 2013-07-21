#include "gflatworldcliimpl.h"
#include "../GameSystem/gdefs.h"


static const wxDword RESERVE_CONSTANT = 10000;

GameFlatWorldClient::GameFlatWorldClient() : m_wrldId(0), m_pRenderTarget(NULL) {
	m_characters.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_characters.begin(); iter != m_characters.end(); iter++) { *iter = NULL; }
	
	m_landscape.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_landscape.begin(); iter != m_landscape.end(); iter++) { *iter = NULL; }
	
	m_moveAbleObj.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_moveAbleObj.begin(); iter != m_moveAbleObj.end(); iter++) { *iter = NULL; }
	
	m_senzors.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_senzors.begin(); iter != m_senzors.end(); iter++) { *iter = NULL; }
	
	m_staticObj.reserve(RESERVE_CONSTANT);
	for(TGameEntityList::iterator iter = m_staticObj.begin(); iter != m_staticObj.end(); iter++) { *iter = NULL; }
}



GameErrorCode GameFlatWorldClient::AIStep()
{
	return FWG_NO_ERROR;
}


GameErrorCode GameFlatWorldClient::DrawStep()
{
	TSceneGraph::iterator iter;
	GameSceneObject *pSceneObj = NULL;
	
	for (iter = m_objectMap.begin(); iter != m_objectMap.end(); iter++)
	{
		pSceneObj = *iter;
		pSceneObj->draw(*m_pRenderTarget, sf::RenderStates());
	}
	
	m_apWorld->DrawDebugData();
	return FWG_NO_ERROR;
}

void GameFlatWorldClient::EventProcess(sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
		{
			switch (event.key.code)
			{
				case sf::Keyboard::Left:
					m_controls.SetLeft();
					break;
				case sf::Keyboard::Right:
					m_controls.SetRight();
					break;
				case sf::Keyboard::Up:
					m_controls.SetUp();
					break;
				case sf::Keyboard::Down:
					m_controls.SetDown();
					break;
				default:
					break;
			}
			break;
		}
		
		case sf::Event::KeyReleased:
		{
			switch (event.key.code)
			{
				case sf::Keyboard::Left:
					m_controls.RelLeft();
					break;
				case sf::Keyboard::Right:
					m_controls.RelRight();
					break;
				case sf::Keyboard::Up:
					m_controls.RelUp();
					break;
				case sf::Keyboard::Down:
					m_controls.RelDown();
					break;
				default:
					break;
			}
			break;
		}
		
		case sf::Event::MouseButtonPressed:
		{
			switch (event.mouseButton.button)
			{
				case sf::Mouse::Left:
					m_controls.SetMouseLeft();
					m_controls.SetLastMousePosition(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					
					FWGLOG_TRACE_FORMAT(wxT("GameFlatWorldClient::EventProcess() : Mouse left butt pressed[ %d, %d]"),
					m_spLogger, event.mouseButton.x, event.mouseButton.y, FWGLOG_ENDVAL);
					break;
				case sf::Mouse::Right:
					m_controls.SetMouseRight();
					m_controls.SetLastMousePosition(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
					
					FWGLOG_TRACE_FORMAT(wxT("GameFlatWorldClient::EventProcess() : Mouse right butt pressed [ %d, %d]"),
					m_spLogger, event.mouseButton.x, event.mouseButton.y, FWGLOG_ENDVAL);
					break;
				default:
					break;
			}
			break;
		}
		
		case sf::Event::MouseButtonReleased:
		{
			switch (event.mouseButton.button)
			{
				case sf::Mouse::Left:
					m_controls.RelMouseLeft();
					break;
				case sf::Mouse::Right:
					m_controls.RelMouseRight();
					break;
				default:
					break;
			}
			break;
		}
		
		default:
			break;
				
	}
}

GameFlatWorldID GameFlatWorldClient::GetFWId()
{
	return m_wrldId;
}

GameErrorCode GameFlatWorldClient::GetUpdateList(GameUpdateStruct**& updList, wxDword& listSize)
{
	FWG_UNREFERENCED_PARAMETER(updList);
	FWG_UNREFERENCED_PARAMETER(listSize);
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::SetWorldSize(const b2AABB wrldAABB)
{
	FWG_UNREFERENCED_PARAMETER(wrldAABB);
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::SimulationStep()
{
	GameErrorCode result = FWG_NO_ERROR;
	m_apWorld->Step(m_timeStep, m_velocityIter, m_positionIter);
	
	TGameEntityList::iterator iter;
	
	for (iter = m_moveAbleObj.begin(); iter != m_moveAbleObj.end(); iter++)
	{
		(**iter).UpdatePosition();
		//(**iter).TraceLogInfo(m_spLogger);
	}
	
	return result;
}

GameFlatWorldClient::~GameFlatWorldClient()
{
}

GameErrorCode GameFlatWorldClient::Initialize(sf::RenderWindow *pTarget, GameLogger* pLogger )
{

	m_spLogger = pLogger;
	
	m_timeStep = 1.0f / 60.0f;
	m_velocityIter = 8;
	m_positionIter = 3;
	m_gravity.Set(0.0f, -10.0f);
	m_apWorld.reset(new (std::nothrow) b2World(m_gravity));
	
	m_pRenderTarget = pTarget;
	
	m_isInitialized = true;
	return FWG_NO_ERROR;
}

GameErrorCode GameFlatWorldClient::AddCharacterEntity(GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	m_characters.push_back(pEntity);
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddLandscapeEntity( GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	m_landscape.push_back(pEntity);
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddMoveableEntity(GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	m_moveAbleObj.push_back(pEntity);
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddSenzorEntity(GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	m_senzors.push_back(pEntity);
	m_objectMap.push_back(pEntity);

	return result;
}

GameErrorCode GameFlatWorldClient::AddStaticEntity(GameEntityBase* pEntity)
{
	GameErrorCode result = FWG_NO_ERROR;	
	m_staticObj.push_back(pEntity);
	m_objectMap.push_back(pEntity);

	return result;
}

GameEntityBase* GameFlatWorldClient::GetCharacterEntity(GameObjectId objId)
{
	return m_characters[objId];
}

GameEntityBase* GameFlatWorldClient::GetLandscapeEntity(GameObjectId objId)
{
	return m_landscape[objId];
}

GameEntityBase* GameFlatWorldClient::GetMoveableEntity(GameObjectId objId)
{
	return m_moveAbleObj[objId];
}

GameEntityBase* GameFlatWorldClient::GetSenzorEntity(GameObjectId objId)
{
	return m_senzors[objId];
}

GameEntityBase* GameFlatWorldClient::GetStaticEntity(GameObjectId objId)
{
	return m_staticObj[objId];
}

GameErrorCode GameFlatWorldClient::EventStep()
{
	
	if (m_controls.IsMouseLeftPressed())
	{
		sf::Vector2i vec(sf::Mouse::getPosition(*m_pRenderTarget));
		MoveViewport(m_controls.GetLastMousePosition() - vec);
		m_controls.SetLastMousePosition(vec);
	}
	
	return FWG_NO_ERROR;
}

void GameFlatWorldClient::EnableDebugDraw()
{
	if (m_pRenderTarget != NULL)
	{
		m_pB2DebugDraw = new (std::nothrow) DebugDraw(m_pRenderTarget);
		if (m_pB2DebugDraw == NULL) return;
		m_apWorld->SetDebugDraw(m_pB2DebugDraw);
	}
}
