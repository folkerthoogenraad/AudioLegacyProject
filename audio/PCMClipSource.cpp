#include "PCMClipSource.h"

namespace apryx {

	bool PCMClipSource::get(std::vector<double>& values, AudioFormat format)
	{
		if(!m_Playing || !m_Clip)
			return false;

		PCMClip &clip = *m_Clip;
		auto &clipSamples = clip.getSamples();

		for (size_t i = 0; i < values.size(); i ++) {
			values[i] = clipSamples[m_Index];

			m_Index++;

			if (m_Index >= clipSamples.size()) {
				m_Index = 0;
				if (!m_Loop) {
					m_Playing = false;
					break;
				}
			}
		}

		return true;
	}
}
