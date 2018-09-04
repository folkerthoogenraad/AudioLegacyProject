#include "PCMMixer.h"

namespace apryx {

	bool PCMMixer::get(std::vector<double>& values, AudioFormat format)
	{
		if (values.size() != m_Buffer.size())
			m_Buffer.resize(values.size());

		std::fill(m_Buffer.begin(), m_Buffer.end(), 0);

		for (auto &v : m_Sources) {
			v->processSamples(m_Buffer, format);
			
			for (int i = 0; i < values.size(); i++) {
				values[i] += m_Buffer[i];
				m_Buffer[i] = 0;
			}
		}

		return true;
	}
}
