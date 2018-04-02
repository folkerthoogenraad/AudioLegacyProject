#include "SpaceDelay.h"

#include "AudioUtils.h"

// Speed of sound in m/s
#define SPEED_OF_SOUND 343

namespace apryx {

	SpaceDelay::SpaceDelay(Vector2f size, Vector2f emitterPosition, Vector2f targetPosition, AudioFormat format)
	{
		recalculate(size, emitterPosition, targetPosition, format);
	}

	void SpaceDelay::recalculate(Vector2f size, Vector2f emitterPosition, Vector2f targetPosition, AudioFormat format)
	{
		Vector2f leftEarPosition = targetPosition + Vector2f(-0.05, 0);
		Vector2f rightEarPosition = targetPosition + Vector2f(0.05, 0);

		double leftDistance = (leftEarPosition - emitterPosition).magnitude();
		double rightDistance = (rightEarPosition - emitterPosition).magnitude();

		Reflection leftReflection;
		Reflection rightReflection;

		leftReflection.gain = gainFalloff(leftDistance);
		rightReflection.gain = gainFalloff(rightDistance);

		leftReflection.offset = leftDistance / SPEED_OF_SOUND * format.sampleRate;
		rightReflection.offset = rightDistance / SPEED_OF_SOUND * format.sampleRate;

		StereoReflection totalReflection;

		totalReflection.left = leftReflection;
		totalReflection.right = rightReflection;

		m_History.resize(format.sampleRate);

		m_Reflections.clear();
		m_Reflections.push_back(totalReflection);
	}

	bool SpaceDelay::get(std::vector<double>& values, AudioFormat format)
	{
		if (format.channels != 2)
			return false;

		for (size_t i = 0; i < values.size(); i += 2) {

			m_History[m_Index] = values[i];
			m_History[m_Index + 1] = values[i + 1];
			
			m_Index += 2;

			if (m_Index >= m_History.size()) {
				m_Index = 0;
			}

			for (auto reflection : m_Reflections) {
				values[i] = m_History[clampIndex(m_Index - (reflection.left.offset * 2))] * reflection.left.gain;
				values[i + 1] = m_History[clampIndex(m_Index - (reflection.right.offset * 2 + 1))] * reflection.right.gain;
			}

		}

		return true;
	}
}
