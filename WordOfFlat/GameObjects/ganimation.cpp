#include "ganimation.h"

void GameAnimation::AddFrame(const sf::Texture& frame, sf::Time duration)
{
	m_staticFrames.push_back(frame);
	m_staticFrameTimes.push_back(m_durationTotal);
	m_durationTotal += duration;
}

void GameAnimation::GetFrameConstant(GameAnimationFrame& frame)
{
	frame = m_framesList[m_actualFrame];
}

void GameAnimation::GetFrameLinearInterpolation(float& ratio)
{
	wxDword nextIndex = 0;
	sf::Time deltaTime = 0;
	
	if(m_actualKeyframes[0] == m_actualKeyframes[1]) return;
	
	if (m_actualKeyframes[0] < m_actualKeyframes[1])
	{
		deltaTime = m_keyFrameTimes[m_actualKeyframes[1]] - m_keyFrameTimes[m_actualKeyframes[0]];
		ratio = ((float)(m_actualTime - m_keyFrameTimes[m_actualKeyframes[0]])) / ((float) deltaTime);
	} else {
		if (m_endlessLoop)
		{
			deltaTime = m_durationTotal - m_keyFrameTimes[m_actualKeyframes[0]];
			ratio = ((float)(m_actualTime - m_keyFrameTimes[m_actualKeyframes[0]])) / ((float) deltaTime);
		} else {
			m_actualKeyframes[1] = m_actualKeyframes[0];
			ratio = 0.0f;
		}
	}
	
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::GetFrameLinearInterpolation() : Frame1[ %u ], frame2[ %u ], %0.4f")
		m_spLogger, m_actualKeyframes[0], m_actualKeyframes[1], ratio, FWGLOG_ENDVAL);
	
}

void GameAnimation::Reset()
{
	m_actualFrame = 0;
	m_actualTime = 0;
}

void GameAnimation::UpdateTime(sf::Time timeIncrement)
{
	m_actualTime += timeIncrement;
	if (!m_staticFrameTimes.empty())
	{
		if (m_staticFrameTimes.size() < 2)
		{
			m_actualFrame = 0;
			return;
		}
		
		if ((m_actualTime > m_durationTotal)&&(m_endlessLoop))
		{
			m_actualTime = m_actualTime - m_durationTotal;
			m_actualFrame = 0;
		}
			
		while((m_staticFrameTimes.size() > (m_actualFrame+1)) && (m_staticFrameTimes[m_actualFrame+1].m_keyTime < m_actualTime ))
		{
			m_actualFrame++;
		}
		
		FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : actualTime( %llu ), actualFrame( %u )"),
			m_spLogger, m_actualTime, m_actualFrame, FWGLOG_ENDVAL);
		
	} else {
		bool change = false;
		if (m_keyFrames.size() < 2)
		{
			m_actualKeyframes[0] = m_actualKeyframes[1] = 0;
			return;
		}

		if ((m_actualTime > m_durationTotal)&&(m_endlessLoop))
		{
			change = true;
			m_actualTime = m_actualTime - m_durationTotal;
			m_actualKeyframes[0] = 0;
		}
		
		while((m_keyFrameTimes.size() > (m_actualKeyframes[0]+1)) && (m_keyFrameTimes[m_actualKeyframes[0]+1].m_keyTime < m_actualTime ))
		{
			change = true;
			m_actualKeyframes[0]++;
		}
		
		if (m_endlessLoop)
		{
			m_actualKeyframes[1] = (m_keyFrameTimes.size() > (m_actualKeyframes[0]+1))? 0 : (m_actualKeyframes[0]+1);
		} else {
			m_actualKeyframes[1] = (m_keyFrameTimes.size() > (m_actualKeyframes[0]+1))? m_actualKeyframes[0] : (m_actualKeyframes[0]+1);
		}
		
		FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : actualTime( %llu ), actualKeyFrame[0]( %u ), actualKeyFrame[1]( %u )"),
			m_spLogger, m_actualTime, m_actualKeyframes[0], m_actualKeyframes[1], FWGLOG_ENDVAL);
			
		if (change) 
		{
			float ratio = 0.0f;
			GetFrameLinearInterpolation(ratio);
			RenderInterpolatedTexture(ratio);
		}
	}
}

void GameAnimation::AddKeyFrame(const sf::Texture& keyFrame, sf::Time duration)
{
	if(!m_staticFrameTimes.empty())
	{
		m_staticFrameTimes.clear();
		m_staticFrames.clear();
	}
	
	if(m_keyFrameTimes.empty())
	{
		m_durationTotal = 0.0f;
		m_actualTime = 0.0f;
		m_animationType = FRAMES_LINEAR;
	}
	
	m_keyFrames.push_back(keyFrame);
	m_keyFrameTimes.push_back(m_durationTotal);
	m_durationTotal += duration;
}

void GameAnimation::Clear()
{
	m_staticFrames.clear();
	m_staticFrameTimes.clear();
	
	m_keyFrames.clear();
	m_keyFrameTimes.clear();
	
	m_durationTotal = 0.0f;
	m_actualTime = 0.0f;
	
	m_actualFrame = 0;
	m_actualKeyframes[0] = m_actualKeyframes[1] = 0;
	
	m_animationType = FRAMES_NONE;
}

sf::Texture* GameAnimation::GetActualFrame()
{
	if(m_staticFrames.empty())
	{
		return &m_renderTexture.getTexture();
	} else {
		return &m_staticFrames[m_actualFrame];
	}
}

void GameAnimation::RenderInterpolatedTexture(float& ratio)
{
	if(!m_isInitialized) 
	{
		sf::Vector2u size = m_keyFrames[0].getSize();
		m_renderTexture.create(size.x, size.y, false);
		m_isInitialized = true;
	}
	wxDword alfaExt = static_cast<wxDword>(ratio * 255.0f);
	sf::Uint8 alfa = (alfaExt > 255)? 255 : static_cast<sf::Uint8>(alfaExt);
	sf::RenderStates renderStates;
	sf::Sprite sprite1(m_keyFrames[m_actualKeyframes[0]]);
	sf::Sprite sprite2(m_keyFrames[m_actualKeyframes[1]]);
	renderStates.blendMode = sf::BlendMultiply;
	
	m_renderTexture.clear(sf::Color(alfa, alfa, alfa, alfa));
	m_renderTexture.draw(sprite2, renderStates);
	m_renderTexture.display();
	
	sprite2.setTexture(m_renderTexture.getTexture());
	
	m_renderTexture.clear(sf::Color(255- alfa, 255- alfa, 255- alfa, 255 - alfa));
	m_renderTexture.draw(sprite1, renderStates);
	m_renderTexture.display();
	
	renderStates.blendMode = sf::BlendAdd;
	
	m_renderTexture.draw(sprite2, renderStates);
	
	m_renderTexture.display();
}
