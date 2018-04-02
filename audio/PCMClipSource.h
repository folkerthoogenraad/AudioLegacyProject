#pragma once

#include "PCMClip.h"
#include "PCMSource.h"

namespace apryx {

	class PCMClipSource : public PCMSource{
		std::shared_ptr<PCMClip> m_Clip;

		bool m_Playing = false;
		bool m_Loop = false;

		double m_Gain = 1;

		// Index is not an integer because it can have some rounding (resampling)
		size_t m_Index = 0;
	public:
		PCMClipSource(std::shared_ptr<PCMClip> clip) : m_Clip(clip) {}

		// audioUtils toGain(double db) to get the db amount.
		void setGain(double gain) { m_Gain = gain; }
		double getGain() const { return m_Gain; }

		void play() { m_Playing = true; }
		void stop() { m_Playing = false; m_Index = 0; }
		void pause() { m_Playing = false; }
		bool isPlaying() const { return m_Playing; }

		void setLooping(bool p) { m_Loop = p; }
		bool isLooping() const { return m_Loop; }

		void setClip(std::shared_ptr<PCMClip> clip) { m_Clip = clip; }
		std::shared_ptr<PCMClip> getClip() const { return m_Clip; }

		virtual bool get(std::vector<double> &values, AudioFormat format);
	};

}