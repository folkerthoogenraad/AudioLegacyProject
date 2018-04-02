#include "BitCrusher.h"

namespace apryx {

	bool BitCrusher::get(std::vector<double>& values, AudioFormat format)
	{
		for (size_t i = 0; i < values.size(); i++) {
			double v = values[i];

			if (v > 0)
				v = ceil(v * m_Steps) / m_Steps;
			else
				v = -ceil(-v * m_Steps) / m_Steps;

			values[i] = v;
		}

		return true;
	}

}
