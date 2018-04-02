#pragma once

#include <memory>
#include <vector>

#include "audio/MidiController.h"
#include "audio/PCMSource.h"

#include "iir/IIRFilter.h"
#include "iir/IIRDelay.h"

namespace apryx {

	struct MidiVoice {
		double wavePhase;
		double volume;

		enum {
			Attack,
			Decay,
			Sustain,
			Release
		} envelopePhase;

		double envelopeTimer;
		double frequency;

		double velocity;


		int key;
	};

	class MidiSource : public PCMSource {

		double m_EnvelopeAttack = 0.001;
		double m_EnvelopeDecay = 0.1;
		double m_EnvelopeSustain = 0.4;
		double m_EnvelopeRelease = 0.1;

		double m_FilterRes = 0.7071;
		double m_FilterCutoff = 1000;

		size_t m_DelaySamples = 1024;
		double m_DelayGain = 0.5;

		double m_Volume = 0.3;

		double m_Detune = 0;

		IIRFilter filter;
		IIRDelay delay;

		std::shared_ptr<apryx::MidiController> m_MidiIn;

		std::vector<MidiVoice> m_Voices;
	public:
		MidiSource(std::shared_ptr<apryx::MidiController> midi) : m_MidiIn(midi) {}


		virtual bool get(std::vector<double> &values, AudioFormat format);
	};

}