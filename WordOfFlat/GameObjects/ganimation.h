#ifndef __GAME_ANIMATION_H__
#define __GAME_ANIMATION_H__

#include "../GameSystem/gdefs.h"
#include <sfml/system/Time.hpp>
#include <sfml/graphics/Texture.hpp>
#include <sfml/graphics/RenderTexture.hpp>
#include <wx/vector.h>
#include "../GameSystem/glog.h"

typedef wxVector<sf::Texture> TFrameTextureList;

/*!
 * \class GameAnimationList
 * \author Lukas
 * \date 30.7.2013
 * \file ganimation.h
 * \brief List of animation frame
 * 
 * This could be used also as lifecycle
 */
class GameAnimation {
public:
	enum InterpolationType {
		FRAMES_NONE = 0,
		FRAMES_LINEAR = 1
	};
private:

	InterpolationType m_animationType;
	
	sf::Time m_actualTime;
	sf::Time m_durationTotal;
	
	wxVector<sf::Time> m_staticFrameTimes; //!< Start frame time (index in array refer to index in m_staticFrames array)
	wxVector<sf::Time> m_keyFrameTimes; //!< Start key frame time (index in array refer to index in m_keyFrames array)
	
	wxDword m_actualFrame;
	wxDword m_actualKeyframes[2];
	
	TFrameTextureList m_staticFrames;
	TFrameTextureList m_keyFrames;
	
	sf::Texture m_intenalTexture;
	
	GameLoggerPtr m_spLogger;
	
	bool m_endlessLoop;
	bool m_repeat;
	bool m_isInitialized;
protected:
	/*!
	 * \brief Get two actual frames with 
	 * \param ratio Ratio between first and second frame (0 == first frame is displayed, 1 == second frame is displayed)
	 */
	void GetFrameLinearInterpolation(float &ratio);
	
	/*!
	 * \brief Render interpolated texture
	 * 
	 * Result is stored in m_renderTexture.
	 * 
	 * \param ratio Ratio between first and second frame
	 * \param outputTexture Texture for storing the result
	 */
	void RenderInterpolatedTexture(float& ratio, sf::Texture& outputTexture);
	
	void UpdateStaticFrameIndex();
	void UpdateKeyFramesIndex();
public:
	GameAnimation() : m_animationType(FRAMES_NONE)
						, m_actualFrame(0) 
						, m_endlessLoop(false)
						, m_repeat(false)
						, m_isInitialized(false)
	{
		m_actualKeyframes[0] = 0;
		m_actualKeyframes[1] = 0;
	}
	
	~GameAnimation() {}
	
	/*!
	 * \brief Add new frame to animation
	 * 
	 * Frame is added at the end of sequence. The start time in animation is set by the animation time.
	 * 	
	 * \param frame New frame
 	 * \param duration Duration of frame
	 */
	void AddFrame(const sf::Texture &frame, sf::Time duration);
	
	/*!
	 * \brief Add new key frame to animation
	 * 
	 * Key frame is added at the end of sequence. The start time in animation is set by the animation time.
	 * You should not mix static frames and key frames.
	 * 	
	 * \param frame New frame
 	 * \param duration Duration of frame
	 * \warning After calling this method, static frames will be cleared!
	 */
	void AddKeyFrame(const sf::Texture &keyFrame, sf::Time duration);
	
	/*!
	 * \brief Update time and internal frame couter
	 * \param timeIncrement
	 */
	void UpdateTimeIncremental(sf::Time timeIncrement);
	
	/*!
	 * \brief Get actual frame
	 * \return Actual texture
	 */
	sf::Texture* GetActualFrame();
	
	/*!
	 * \brief Reset animation to beginning
	 */
	void Reset();
	
	/*!
	 * \brief Erase all frames
	 */
	void Clear();
	
	/*!
	 * \brief Precompute frames from keyframes
	 * \param constFrameDuration Constant time duration of every frame
	 * \retval FWG_NO_ERROR On success
	 * \retval Error code on fail
	 */
	GameErrorCode PrecomputeFrames(sf::Time constFrameDuration = sf::Time::Zero);
		
	inline void SetInterpolation( InterpolationType interType) { m_animationType = interType; }
	inline void SetEndless( bool isEndless) { m_endlessLoop = isEndless; }
	inline void SetRepeat( bool repeat) { m_repeat = repeat;}
	
	inline bool IsEndless() { return m_endlessLoop;}
	inline bool IsRepeat() { return m_repeat;}
	
	inline void SetLogger( GameLogger* pLogger) { m_spLogger = pLogger; }
	
};




#endif //__GAME_ANIMATION_H__