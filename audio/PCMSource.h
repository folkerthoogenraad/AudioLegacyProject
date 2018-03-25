#pragma once

#include <vector>
#include <memory>

#include "AudioFormat.h"

namespace apryx {

	class PCMEffect;

	class PCMSource {
		std::vector<std::unique_ptr<PCMEffect>> m_Effects;
	public:
		virtual ~PCMSource();

		bool processSamples(std::vector<double> &values, AudioFormat format);
		virtual bool get(std::vector<double> &values, AudioFormat format) { return false; };

		PCMEffect *addEffect(std::unique_ptr<PCMEffect> effect);
	};


	class PCMEffect : public PCMSource
	{
	public:
		virtual ~PCMEffect() = default;

		virtual bool get(std::vector<double> &values, AudioFormat format) { return false; };
	};

}