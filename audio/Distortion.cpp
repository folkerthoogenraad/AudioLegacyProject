#include "Distortion.h"

#include "AudioUtils.h"

namespace apryx {

	bool Distortion::get(std::vector<double>& values, AudioFormat format)
	{
		switch (m_Algorithm) {
		case Algorithm::HardClip:
			for (size_t i = 0; i < values.size(); i++) {
				double value = values[i] * m_PreAmp;

				if (value > 1)
					value = 1;

				if (value < -1)
					value = -1;

				values[i] = value * m_PostAmp;
			}

			return true;
			break;

		case Algorithm::SineFold:
			for (size_t i = 0; i < values.size(); i++) {
				double value = values[i] * m_PreAmp;


				values[i] = audioSine(value * 2) * m_PostAmp;
			}

			return true;
			break;

		default:
			return false;
		}


	}

}
