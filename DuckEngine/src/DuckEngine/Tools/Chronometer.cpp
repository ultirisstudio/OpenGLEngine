#include "depch.h"
#include <DuckEngine/Tools/Chronometer.h>

DuckEngine::Chronometer::Chronometer(bool start) :
	m_startTime(),
	m_elapsedTime(),
	m_paused()
{
	reset();

	if(start)
		this->start();
}

void DuckEngine::Chronometer::start()
{
	m_startTime = std::chrono::system_clock::now();

	m_paused = false;
}
void DuckEngine::Chronometer::stop()
{
	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - m_startTime);

	m_elapsedTime.seconds += static_cast<float>(duration.count()) / std::chrono::microseconds::period::den;
	m_elapsedTime.milliseconds += static_cast<float>(duration.count()) / std::chrono::milliseconds::period::den;
	m_elapsedTime.microseconds += static_cast<float>(duration.count()) / std::chrono::seconds::period::den;
	
	m_paused = true;
}
void DuckEngine::Chronometer::reset()
{
	m_startTime = std::chrono::system_clock::time_point(std::chrono::microseconds::zero());

	m_elapsedTime.seconds = 0.0;
	m_elapsedTime.milliseconds = 0.0;
	m_elapsedTime.microseconds = 0.0;

	m_paused = true;
}

void DuckEngine::Chronometer::restart()
{
	reset();
	start();
}

DuckEngine::ElapsedTime DuckEngine::Chronometer::getElapsedTime() const
{
	if (m_paused)
		return m_elapsedTime;

	std::chrono::microseconds duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - m_startTime);

	return ElapsedTime(
		{
			m_elapsedTime.seconds + static_cast<float>(duration.count()) / std::chrono::microseconds::period::den,
			m_elapsedTime.milliseconds + static_cast<float>(duration.count()) / std::chrono::milliseconds::period::den,
			m_elapsedTime.microseconds + static_cast<float>(duration.count()) / std::chrono::seconds::period::den
		}
	);
}