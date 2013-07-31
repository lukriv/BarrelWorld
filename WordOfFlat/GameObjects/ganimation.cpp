

void GameAnimationList::AddFrame(const GameAnimationFrame& frame, sf::Time duration)
{
	m_frameList.push_back(frame);
	m_frameList.back().m_keyTime = m_durationTotal;
	m_durationTotal += duration;
}

void GameAnimationList::GetFrameConstant(GameAnimationFrame& frame)
{
	frame = m_framesList[m_actualFrame];
}

void GameAnimationList::GetFrameLinearInterpolation(GameAnimationFrame& frame1, GameAnimationFrame& frame2, float& ratio)
{
	wxDword nextIndex = 0;
	sf::Time deltaTime = 0;
	
	if(m_framesList.size() < 2) return;
	
	frame1 = m_framesList[m_actualFrame];

	if (m_framesList.size() > (m_actualFrame+1))
	{
		frame2 = m_framesList[m_actualFrame+1];
		deltaTime = frame2.m_keyTime - frame1.m_keyTime;
		ratio = ((float)(m_actualTime - frame1.m_keyTime)) / ((float) deltaTime);
	} else {
		if (m_endlessLoop)
		{
			frame2 = m_framesList[0];
			deltaTime = m_durationTotal - frame1.m_keyTime;
			ratio = ((float)(m_actualTime - frame1.m_keyTime)) / ((float) deltaTime);
		} else {
			frame2 = frame1;
			ratio = 0.0f;
		}
	}
	
}

void GameAnimationList::Reset()
{
	m_actualFrame = 0;
	m_actualTime = 0;
}

void GameAnimationList::UpdateTime(sf::Time timeIncrement)
{
	m_actualTime += timeIncrement;
	
	if(m_framesList.size() < 2) return;
	
	if ((m_actualTime > m_durationTotal)&&(m_endlessLoop))
	{
		m_actualTime = m_actualTime - m_durationTotal;
		m_actualFrame = 0;
	}
	
	while((m_framesList.size() > (m_actualFrame+1)) && (m_framesList[m_actualFrame+1].m_keyTime < m_actualTime ))
	{
		m_actualFrame++;
	}
}
