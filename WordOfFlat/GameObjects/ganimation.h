#ifndef __GAME_ANIMATION_H__
#define __GAME_ANIMATION_H__

#include "../GameSystem/gdefs.h"
#include <sfml/system/Time.hpp>

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
	bool m_keyFramesEnabled;
	bool m_endlessLoop;
	
	sf::Time m_actualTime;
	sf::Time m_durationTotal;
	
	wxVector<sf::Time> m_staticFrameTimes;
	wxVector<sf::Time> m_keyFrameTimes;
	
	wxDword m_staticFrameOffset;
	wxDword m_keyFrameOffset;
	
	wxDword m_actualFrame;
	wxDword m_actualKeyframes[2];
	
	TFrameTextureList *m_pStaticFrames;
	TFrameTextureList *m_pKeyFrames;
	
	sf::RenderTexture m_renderTexture;
	
public:
	GameAnimation() : m_keyFramesEnabled(false)
						, m_endlessLoop(true)
						, m_actualTime(0.0f)
						, m_durationTotal(0.0f)
						, m_actualFrame(0) {}
	~GameAnimation() {}
	
	/*!
	 * \brief Add new frame to animation
	 * 
	 * Frame is added at the end of sequence. The start time in animation is set by the animation time.
	 * 	
	 * \param frame New frame
 	 * \param duration Duration of frame
	 */
	void AddFrame(const GameAnimationFrame &frame, sf::Time duration);
	
	/*!
	 * \brief Update time and internal frame couter
	 * \param timeIncrement
	 */
	void UpdateTime(sf::Time timeIncrement);
	
	/*!
	 * \brief Get actual frame
	 * \param frame 
	 */
	void GetFrameConstant(GameAnimationFrame &frame);
	
	/*!
	 * \brief Get two actual frames with 
	 * \param frame1
	 * \param frame2
	 * \param ratio
	 */
	void GetFrameLinearInterpolation(GameAnimationFrame &frame1, GameAnimationFrame &frame2, float &ratio);
	
	/*!
	 * \brief Reset animation to beginning
	 */
	void Reset();
	
	
	/*!
	 * \brief Erase all frames
	 */
	void Clear();
		
	inline void SetKeyFrameEnabled( bool keyFramesEnabled) { m_keyFramesEnabled = keyFramesEnabled; }
	inline void SetEndless( bool isEndless) { m_endlessLoop = isEndless; }
	
	inline bool IsKeyFramesList() { return m_keyFramesEnabled;}
	inline bool IsEndless() { return m_endlessLoop;}
	
	
	
};




#endif //__GAME_ANIMATION_H__