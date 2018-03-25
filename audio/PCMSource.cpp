#include "PCMSource.h"

namespace apryx {

	PCMSource::~PCMSource()
	{
	}

	bool PCMSource::processSamples(std::vector<double>& values, AudioFormat format)
	{
		bool res = get(values, format);

		if (m_Effects.size() == 0) {
			return res;
		}
		else {
			bool out = res;

			for (auto &effects : m_Effects) {
				if (effects->processSamples(values, format))
					out = true;
			}

			return out;
		}

	}

	PCMEffect * PCMSource::addEffect(std::unique_ptr<PCMEffect> effect)
	{
		PCMEffect *p = effect.get();

		m_Effects.push_back(std::move(effect));

		return p;
	}

}