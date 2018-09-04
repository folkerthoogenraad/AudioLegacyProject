#pragma once

#include <math.h>
#include <array>

namespace apryx {

	template<size_t N>
	class Wavetable {
		std::array<float, N> m_Data;
	public:
		Wavetable() {
			std::fill(m_Data.begin(), m_Data.end(), 0);
		}

		inline double interpolate(double phase) {
			double index = phase * N;

			float low = m_Data[(int)floor(index) % N];
			float high = m_Data[(int)ceil(index) % N];

			double distance = fmod(index, 1);

			return low + (high - low) * distance;
		}

		inline void set(int index, float value) { m_Data[index] = value; }
		inline float get(int index) const { return m_Data[index]; }
		inline int size() const { return N; }
	};

}