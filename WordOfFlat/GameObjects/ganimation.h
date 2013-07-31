#ifndef __GAME_ANIMATION_H__
#define __GAME_ANIMATION_H__

#include "../GameSystem/gdefs.h"
#include <sfml/system/Time.hpp>

/*!
 * \class GameAnimationFrame
 * \author Lukas
 * \date 29.7.2013
 * \file ganimation.h
 * \brief Definition of one frame
 */
struct GameAnimationFrame {
	sf::Time m_keyTime;
	wxDword m_texIndex;
};


/*!
 * \class GameAnimationList
 * \author Lukas
 * \date 30.7.2013
 * \file ganimation.h
 * \brief List of animation frame
 * 
 * This could be used also as lifecycle
 */
class GameAnimationList {
private:
	bool m_keyFramesEnabled;
	bool m_endlessLoop;
	sf::Time m_actualTime;
	sf::Time m_durationTotal;
	wxVector<GameAnimationFrame> m_framesList;
	wxDword m_actualFrame;
	
public:
	GameAnimationList() : m_keyFramesEnabled(false)
						, m_endlessLoop(true)
						, m_actualTime(0.0f)
						, m_durationTotal(0.0f)
						, m_actualFrame(0) {}
	~GameAnimationList() {}
	
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