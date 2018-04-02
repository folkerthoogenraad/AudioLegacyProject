#pragma once

#include <vector>

#include "AudioFormat.h"

#include "math/Vector2.h"
#include "PCMSource.h"

namespace apryx {

	// Very cool and very expensive calculation stuff
	// Only works with stereo input and output at the moment

	class SpaceDelay : public PCMEffect{
	public:
		struct Reflection {
			size_t offset = 0;
			double gain = 0;
		};

		struct StereoReflection {
			Reflection left;
			Reflection right;
		};

	private:
		std::vector<AudioSample> m_History;
		std::vector<StereoReflection> m_Reflections;

		int m_Index;
	public:
		SpaceDelay(Vector2f size, Vector2f emitterPosition, Vector2f targetPosition, AudioFormat format);

		void recalculate(Vector2f size, Vector2f emitterPosition, Vector2f targetPosition, AudioFormat format);

		virtual bool get(std::vector<double> &values, AudioFormat format);

	private:
		inline size_t clampIndex(int index) {
			if (index < 0)
				index += m_History.size();
			if (index >= m_History.size())
				index -= m_History.size();

			return (size_t) index;
		}
	};
}