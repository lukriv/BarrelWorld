#ifndef __GAME_ANIMATION_IMPLEMENTATION_H__
#define __GAME_ANIMATION_IMPLEMENTATION_H__


#include <sfml/system/Time.hpp>
#include <sfml/graphics/RenderTexture.hpp>
#include <wx/vector.h>
#include "../GameSystem/glog.h"

#include "ganimation.h"


typedef wxVector<sf::Texture> TFrameTextureList;


/*!
 * \class AnimFrameSequence
 * \author Lukas
 * \date 19.8.2013
 * \file ganimationimpl.h
 * \brief Shared animation frame sequence
 */
class AnimFrameSequence : public IRefObject {
	wxAtomicInt32 m_refCount;
	TFrameTextureList m_frameSequence;
public:
	AnimFrameSequence() : m_refCount(1) {}
	inline TFrameTextureList& GetSequence() { return m_frameSequence; }

public:
	virtual void addRef()
	{
		wxAtomicInc(m_refCount);
	}
	
	virtual wxInt32 release()
	{
		wxInt32 refCount = wxAtomicDec(m_refCount);
		if (refCount == 0)
		{
			delete this;
		}
		return refCount;
	}
private:
	~AnimFrameSequence()
	{
		m_frameSequence.clear();
	}
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
class GameAnimation {
public:
	enum AnimationType {
		ANIM_TYPE_STATIC = 0,
		ANIM_TYPE_LINEAR = 1
	};
private:

	AnimationType m_animationType;
	
	sf::Time m_actualTime;
	sf::Time m_durationTotal;
	
	wxVector<sf::Time> m_frameTimes; //!< Start frame time (index in array refer to index in m_staticFrames array)
	
	
	wxDword m_actualFrame; //!< actual frame
	wxDword m_secondaryFrame; //!< second frame for non-static animations 
	
	RefObjSmPtr<AnimFrameSequence> m_spFrameSequence;
		
	sf::Texture m_intenalTexture;
	
	GameLoggerPtr m_spLogger;
	
	bool m_endlessLoop;
	bool m_repeat;
	bool m_cloned;
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
	GameAnimation() : m_animationType(ANIM_TYPE_STATIC)
						, m_actualFrame(0) 
						, m_secondaryFrame(0)
						, m_endlessLoop(false)
						, m_repeat(false)
						, m_cloned(false){}
	
	~GameAnimation() {}
	
	/*!
	 * \brief Clone animation
	 * \param clone Empty animation will be filled with this animation clone
	 * \retval FWG_NO_ERROR on success
	 * \retval error on fail
	 */
	GameErrorCode Clone(GameAnimation &clone);
	
	
	/*!
	 * \brief Scale animation time
	 * 
	 * Scale animation times according to ratio. Less than one the animation will be shorter, more than one 
	 * the animation will be longer.
	 * 
	 * \param scale ratio (1 means that no change will be made)
	 */
	void TimeScale(float scale);
	
	/*!
	 * \brief Scale animation time
	 * 
	 * This version set new total animation duration and recomputes frame duration in ratio.
	 * 
	 * \param newTotalDuration Absolute animation time duration.
	 */
	void TimeScale(sf::Time newTotalDuration);
	
	/*!
	 * \brief Add new frame to animation
	 * 
	 * Frame is added at the end of sequence. The start time in animation is set by the animation time.
	 * 	
	 * \param frame New frame
 	 * \param duration Duration of frame
	 */
	GameErrorCode AddFrame(const sf::Texture &frame, sf::Time duration);
	
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
	 * \retval FWG_E_INVALID_OPERATION_ERROR When animation is already static
	 * \retval Error code on fail
	 */
	GameErrorCode GenerateStaticAnimation(GameAnimation& staticAnim, sf::Time constFrameDuration = sf::Time::Zero);
		
	inline void SetAnimType( AnimationType animType) { m_animationType = animType; }
	inline void SetEndless( bool isEndless) { m_endlessLoop = isEndless; }
	inline void SetRepeat( bool repeat) { m_repeat = repeat;}
	
	inline bool IsEndless() { return m_endlessLoop;}
	inline bool IsRepeat() { return m_repeat;}
	
	inline void SetLogger( GameLogger* pLogger) { m_spLogger = pLogger; }
	
};





#endif //__GAME_ANIMATION_IMPLEMENTATION_H__