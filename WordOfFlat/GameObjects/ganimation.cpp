#include "ganimation.h"
#include <sfml/graphics/Sprite.hpp>
#include <string>
#include <sstream>
static const char* DEBUG_IMAGE_NAME = "debugImage";
static const char* DEBUG_IMAGE_EXT = ".png";

static wxInt32 debugImageNr = 0;

void GameAnimation::AddFrame(const sf::Texture& frame, sf::Time duration)
{
	m_staticFrames.push_back(frame);
	m_staticFrameTimes.push_back(m_durationTotal);
	m_durationTotal += duration;
}

void GameAnimation::GetFrameLinearInterpolation(float& ratio)
{
	sf::Time deltaTime;
	
	if(m_actualKeyframes[0] == m_actualKeyframes[1]) return;
	
	if (m_actualKeyframes[0] < m_actualKeyframes[1])
	{
		deltaTime = m_keyFrameTimes[m_actualKeyframes[1]] - m_keyFrameTimes[m_actualKeyframes[0]];
		ratio = (m_actualTime.asSeconds() - m_keyFrameTimes[m_actualKeyframes[0]].asSeconds()) / (deltaTime.asSeconds());
	} else {
		if (m_endlessLoop)
		{
			deltaTime = m_durationTotal - m_keyFrameTimes[m_actualKeyframes[0]];
			ratio = (m_actualTime.asSeconds() - m_keyFrameTimes[m_actualKeyframes[0]].asSeconds()) / (deltaTime.asSeconds());
		} else {
			m_actualKeyframes[1] = m_actualKeyframes[0];
			ratio = 0.0f;
		}
	}
	
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::GetFrameLinearInterpolation() : Frame1[ %u ], frame2[ %u ], %0.4f"),
		m_spLogger, m_actualKeyframes[0], m_actualKeyframes[1], ratio, FWGLOG_ENDVAL);
	
}

void GameAnimation::Reset()
{
	m_actualFrame = 0;
	m_actualTime = sf::Time::Zero;
}


void GameAnimation::UpdateKeyFramesIndex()
{
	//bool change = false;
	if (m_keyFrames.size() < 2)
	{
		m_actualKeyframes[0] = m_actualKeyframes[1] = 0;
		return;
	}

	if ((m_actualTime > m_durationTotal)&&(m_repeat))
	{
		//change = true;
		m_actualTime = m_actualTime - m_durationTotal;
		m_actualKeyframes[0] = 0;
	}
		
	while((m_keyFrameTimes.size() > (m_actualKeyframes[0]+1)) && (m_keyFrameTimes[m_actualKeyframes[0]+1].asMicroseconds() < m_actualTime.asMicroseconds() ))
	{
		//change = true;
		m_actualKeyframes[0]++;
	}
		
	if (m_endlessLoop)
	{
		m_actualKeyframes[1] = (m_keyFrameTimes.size() <= (m_actualKeyframes[0]+1))? 0 : (m_actualKeyframes[0]+1);
	} else {
		m_actualKeyframes[1] = (m_keyFrameTimes.size() <= (m_actualKeyframes[0]+1))? m_actualKeyframes[0] : (m_actualKeyframes[0]+1);
	}
		
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : actualTime( %llu ), actualKeyFrame[0]( %u ), actualKeyFrame[1]( %u )"),
		m_spLogger, m_actualTime, m_actualKeyframes[0], m_actualKeyframes[1], FWGLOG_ENDVAL);
		
	{
		float ratio = 0.0f;
		GetFrameLinearInterpolation(ratio);
		RenderInterpolatedTexture(ratio, m_intenalTexture);
		FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : ratio = %.4f"),
			m_spLogger, ratio, FWGLOG_ENDVAL);
	}
}

void GameAnimation::UpdateStaticFrameIndex()
{
	if (m_staticFrameTimes.size() < 2)
	{
		m_actualFrame = 0;
		return;
	}
		
	if ((m_actualTime > m_durationTotal)&&(m_repeat))
	{
		m_actualTime = m_actualTime - m_durationTotal;
		m_actualFrame = 0;
	}
			
	while((m_staticFrameTimes.size() > (m_actualFrame+1)) && (m_staticFrameTimes[m_actualFrame+1].asMicroseconds() < m_actualTime.asMicroseconds() ))
	{
		m_actualFrame++;
	}
		
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : actualTime( %llu ), actualFrame( %u )"),
		m_spLogger, m_actualTime, m_actualFrame, FWGLOG_ENDVAL);
}

void GameAnimation::UpdateTimeIncremental(sf::Time timeIncrement)
{
	m_actualTime += timeIncrement;
	if (!m_staticFrameTimes.empty())
	{
		UpdateStaticFrameIndex();
	} else {
		UpdateKeyFramesIndex();
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
		m_durationTotal = sf::Time::Zero;
		m_actualTime = sf::Time::Zero;
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
	
	m_durationTotal = sf::Time::Zero;
	m_actualTime = sf::Time::Zero;
	
	m_actualFrame = 0;
	m_actualKeyframes[0] = m_actualKeyframes[1] = 0;
	
	m_animationType = FRAMES_NONE;
}

sf::Texture* GameAnimation::GetActualFrame()
{
	if(m_staticFrames.empty())
	{
		return &m_intenalTexture;
	} else {
		return &m_staticFrames[m_actualFrame];
	}
}

void GameAnimation::RenderInterpolatedTexture(float& ratio, sf::Texture &outputTexture)
{

	sf::Vector2u size = m_keyFrames[0].getSize();
	sf::RenderTexture renTex1, renTex2;
	
	if(m_actualKeyframes[0] == m_actualKeyframes[1])
	{
		outputTexture = m_keyFrames[m_actualKeyframes[0]];
		return;
	}
	
	renTex1.create(size.x, size.y, false);
	renTex2.create(size.x, size.y, false);
	
	wxDword alfaExt = static_cast<wxDword>(ratio * 255.0f);
	sf::Uint8 alfa = (alfaExt > 255)? 255 : static_cast<sf::Uint8>(alfaExt);
	sf::RenderStates renderStates;
	sf::Sprite sprite1(m_keyFrames[m_actualKeyframes[0]]);
	sf::Sprite sprite2(m_keyFrames[m_actualKeyframes[1]]);
	renderStates.blendMode = sf::BlendMultiply;
	
	renTex2.clear(sf::Color(alfa, alfa, alfa, alfa));
	renTex2.draw(sprite2, renderStates);
	renTex2.display();
	
	/*{
		std::ostringstream filenamestream;
		filenamestream << DEBUG_IMAGE_NAME << debugImageNr << DEBUG_IMAGE_EXT;
		debugImageNr++;
		texture2.getTexture().copyToImage().saveToFile(filenamestream.str());
	}*/

	sprite2.setTexture(renTex2.getTexture());
	
	renTex1.clear(sf::Color(255- alfa, 255- alfa, 255- alfa, 255 - alfa));
	renTex1.draw(sprite1, renderStates);
	//m_renderTexture.display();
	
	renderStates.blendMode = sf::BlendAdd;
	
	renTex1.draw(sprite2, renderStates);
	
	renTex1.display();
	
	outputTexture = renTex1.getTexture();
	
	/*{
		std::ostringstream filenamestream;
		filenamestream << DEBUG_IMAGE_NAME << debugImageNr << DEBUG_IMAGE_EXT;
		debugImageNr++;
		texture1.getTexture().copyToImage().saveToFile(filenamestream.str());	
	}*/
}

GameErrorCode GameAnimation::PrecomputeFrames(sf::Time constFrameDuration)
{
	wxInt32 totalFramesCount = 0;
	float ratio;
	if (constFrameDuration == sf::Time::Zero)
	{
		constFrameDuration = sf::milliseconds(20); // 25 frames per second
	}
	
	totalFramesCount = (m_durationTotal.asMilliseconds() / constFrameDuration.asMilliseconds()) + 1;
	
	// preallocate vector 
	m_staticFrames.resize(totalFramesCount);
	m_staticFrameTimes.resize(totalFramesCount);
	
	Reset();
	
	for(wxInt32 i = 0; i < totalFramesCount; i++)
	{
		GetFrameLinearInterpolation(ratio);
		RenderInterpolatedTexture(ratio, m_staticFrames[i]);
		m_staticFrameTimes[i] = m_actualTime;
		m_actualTime += constFrameDuration;
		UpdateKeyFramesIndex();
	}
	
	Reset();
	
	return FWG_NO_ERROR;
	
}


