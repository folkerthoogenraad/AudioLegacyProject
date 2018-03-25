#pragma once

#include "PCMSource.h"

namespace apryx {

	class Distortion : public PCMEffect
	{
	public:
		enum Algorithm {
			HardClip,

			SineFold,
		};

	private:
		double m_PreAmp = 1;
		double m_PostAmp = 1;

		Algorithm m_Algorithm = Algorithm::HardClip;
	public:
		void setPreAmp(double pa) { m_PreAmp = pa; }
		void setPostAmp(double pa) { m_PostAmp = pa; }
		void setAlgorithm(Algorithm alg) { m_Algorithm = alg; }

		virtual bool get(std::vector<double> &values, AudioFormat format);
	};
}

