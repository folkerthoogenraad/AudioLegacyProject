#pragma once

#include "PCMSource.h"

namespace apryx {

	class BitCrusher : public PCMEffect{
		double m_Steps = 100;

	public:
		BitCrusher() = default;

		void setSteps(double steps) { m_Steps = steps; };

		virtual bool get(std::vector<double> &values, AudioFormat format);
	};

}