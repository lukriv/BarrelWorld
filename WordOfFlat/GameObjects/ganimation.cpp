#include "ganimation.h"
#include <sfml/graphics/Sprite.hpp>
#include <string>
#include <sstream>
static const char* DEBUG_IMAGE_NAME = "debugImage";
static const char* DEBUG_IMAGE_EXT = ".png";

static wxInt32 debugImageNr = 0;

GameErrorCode GameAnimation::AddFrame(const sf::Texture& frame, sf::Time duration)
{
	if(m_spFrameSequence.IsEmpty())
	{
		m_spFrameSequence.Attach(new (std::nothrow) AnimFrameSequence());
		if(m_spFrameSequence.IsEmpty())
		{
			FWGLOG_ERROR_FORMAT(wxT("GameAnimation::AddFrame() : Create new animation sequence failed: 0x%08x"),
				m_spLogger, (GameErrorCode) FWG_E_MEMORY_ALLOCATION_ERROR, FWGLOG_ENDVAL);
			return FWG_E_MEMORY_ALLOCATION_ERROR;
		}
	}
	m_spFrameSequence->GetSequence().push_back(frame);
	m_frameTimes.push_back(m_durationTotal);
	m_durationTotal += duration;
	
	return FWG_NO_ERROR;
}

void GameAnimation::GetFrameLinearInterpolation(float& ratio)
{
	sf::Time deltaTime;
	
	if(m_actualFrame == m_secondaryFrame) return;
	
	if (m_actualFrame < m_secondaryFrame)
	{
		deltaTime = m_frameTimes[m_secondaryFrame] - m_frameTimes[m_actualFrame];
		ratio = (m_actualTime.asSeconds() - m_frameTimes[m_actualFrame].asSeconds()) / (deltaTime.asSeconds());
	} else {
		if (m_endlessLoop)
		{
			deltaTime = m_durationTotal - m_frameTimes[m_actualFrame];
			ratio = (m_actualTime.asSeconds() - m_frameTimes[m_actualFrame].asSeconds()) / (deltaTime.asSeconds());
		} else {
			m_secondaryFrame = m_actualFrame;
			ratio = 0.0f;
		}
	}
	
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::GetFrameLinearInterpolation() : Frame1[ %u ], frame2[ %u ], %0.4f"),
		m_spLogger, m_actualFrame, m_secondaryFrame, ratio, FWGLOG_ENDVAL);
	
}

void GameAnimation::Reset()
{
	m_actualFrame = 0;
	m_actualTime = sf::Time::Zero;
}


void GameAnimation::UpdateKeyFramesIndex()
{
	//bool change = false;
	if (m_spFrameSequence->GetSequence().size() < 2)
	{
		m_actualFrame = m_secondaryFrame = 0;
		return;
	}

	if ((m_actualTime > m_durationTotal)&&(m_repeat))
	{
		//change = true;
		m_actualTime = m_actualTime - m_durationTotal;
		m_actualFrame = 0;
	}
		
	while((m_frameTimes.size() > (m_actualFrame+1)) && (m_frameTimes[m_actualFrame+1].asMicroseconds() < m_actualTime.asMicroseconds() ))
	{
		//change = true;
		m_actualFrame++;
	}
		
	if (m_endlessLoop)
	{
		m_secondaryFrame = (m_frameTimes.size() <= (m_actualFrame+1))? 0 : (m_actualFrame+1);
	} else {
		m_secondaryFrame = (m_frameTimes.size() <= (m_actualFrame+1))? m_actualFrame : (m_actualFrame+1);
	}
		
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : actualTime( %llu ), m_actualFrame( %u ), m_secondaryFrame( %u )"),
		m_spLogger, m_actualTime, m_actualFrame, m_secondaryFrame, FWGLOG_ENDVAL);
		
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
	if (m_spFrameSequence->GetSequence().size() < 2)
	{
		m_actualFrame = 0;
		return;
	}
		
	if ((m_actualTime > m_durationTotal)&&(m_repeat))
	{
		m_actualTime = m_actualTime - m_durationTotal;
		m_actualFrame = 0;
	}
			
	while((m_frameTimes.size() > (m_actualFrame+1)) && (m_frameTimes[m_actualFrame+1].asMicroseconds() < m_actualTime.asMicroseconds() ))
	{
		m_actualFrame++;
	}
		
	FWGLOG_TRACE_FORMAT(wxT("GameAnimation::UpdateTime() : actualTime( %llu ), actualFrame( %u )"),
		m_spLogger, m_actualTime, m_actualFrame, FWGLOG_ENDVAL);
}

void GameAnimation::UpdateTimeIncremental(sf::Time timeIncrement)
{
	m_actualTime += timeIncrement;
	if (ANIM_TYPE_STATIC == m_animationType)
	{
		UpdateStaticFrameIndex();
	} else {
		UpdateKeyFramesIndex();
	}
}


void GameAnimation::Clear()
{
	m_spFrameSequence.Release();
	
	m_frameTimes.clear();
		
	m_durationTotal = sf::Time::Zero;
	m_actualTime = sf::Time::Zero;
	
	m_actualFrame = 0;
	m_secondaryFrame = 0;
	
	m_animationType = ANIM_TYPE_STATIC;
	
	m_endlessLoop = false;
	m_repeat = false;
}

sf::Texture* GameAnimation::GetActualFrame()
{
	if(ANIM_TYPE_STATIC == m_animationType)
	{
		return &m_spFrameSequence->GetSequence()[m_actualFrame];
	} else {
		return &m_intenalTexture;
	}
}

void GameAnimation::RenderInterpolatedTexture(float& ratio, sf::Texture &outputTexture)
{

	sf::Vector2u size = m_spFrameSequence->GetSequence()[0].getSize();
	sf::RenderTexture renTex1, renTex2;
	
	if(m_actualFrame == m_secondaryFrame)
	{
		outputTexture = m_spFrameSequence->GetSequence()[m_actualFrame];
		return;
	}
	
	renTex1.create(size.x, size.y, false);
	renTex2.create(size.x, size.y, false);
	
	wxDword alfaExt = static_cast<wxDword>(ratio * 255.0f);
	sf::Uint8 alfa = (alfaExt > 255)? 255 : static_cast<sf::Uint8>(alfaExt);
	sf::RenderStates renderStates;
	sf::Sprite sprite1(m_spFrameSequence->GetSequence()[m_actualFrame]);
	sf::Sprite sprite2(m_spFrameSequence->GetSequence()[m_secondaryFrame]);
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

GameErrorCode GameAnimation::GenerateStaticAnimation(GameAnimation& staticAnim, sf::Time constFrameDuration)
{
	wxInt32 totalFramesCount = 0;
	float ratio;
	
	if(ANIM_TYPE_STATIC == m_animationType)
	{
		return FWG_E_INVALID_OPERATION_ERROR;
	}
	
	if (constFrameDuration == sf::Time::Zero)
	{
		constFrameDuration = sf::milliseconds(20); // 25 frames per second
	}
	
	totalFramesCount = (m_durationTotal.asMilliseconds() / constFrameDuration.asMilliseconds()) + 1;
	
	staticAnim.Clear();
	
	Reset();
	
	GetFrameLinearInterpolation(ratio);
	RenderInterpolatedTexture(ratio, m_intenalTexture);
	FWG_RETURN_FAIL(staticAnim.AddFrame(m_intenalTexture, constFrameDuration));
	for(wxInt32 i = 1; i < totalFramesCount; i++)
	{
		UpdateTimeIncremental(constFrameDuration);
		FWG_RETURN_FAIL(staticAnim.AddFrame(m_intenalTexture, constFrameDuration));
	}
	
	//flags same as parent
	staticAnim.m_endlessLoop = this->m_endlessLoop;
	staticAnim.m_repeat = this->m_repeat;
	
	return FWG_NO_ERROR;
	
}

GameErrorCode GameAnimation::Clone(GameAnimation& clone)
{
	clone.Clear();
	
	clone.m_actualFrame = 0;
	clone.m_secondaryFrame = 0;
	clone.m_actualTime = sf::Time::Zero;
	clone.m_animationType = m_animationType;
	clone.m_durationTotal = m_durationTotal;
	clone.m_endlessLoop = m_endlessLoop;
	wxVector<sf::Time>::iterator timeIter;
	timeIter = m_frameTimes.begin();
	while(timeIter != m_frameTimes.end())
	{
		clone.m_frameTimes.push_back(*timeIter);
		timeIter++;
	}
	clone.m_repeat = m_repeat;
	clone.m_spFrameSequence = m_spFrameSequence;
	
	clone.m_cloned = m_cloned = true;
	
	return FWG_NO_ERROR;
}

void GameAnimation::TimeScale(float scale)
{
}

void GameAnimation::TimeScale(sf::Time newTotalDuration)
{
}


