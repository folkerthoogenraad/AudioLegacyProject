#pragma once

#include <memory>
#include <vector>

#include "audio/MidiController.h"
#include "audio/PCMSource.h"

#include "iir/IIRFilter.h"

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

	class MidiSource : public apryx::PCMSource {

		double m_EnvelopeAttack = 0.01;
		double m_EnvelopeDecay = 0.2;
		double m_EnvelopeSustain = 0.7;
		double m_EnvelopeRelease = 0.1;

		double m_FilterRes = 0.7071;
		double m_FilterCutoff = 2000;

		double m_Volume = 0.7;

		IIRFilter filter;

		std::shared_ptr<apryx::MidiController> m_MidiIn;

		std::vector<MidiVoice> m_Voices;
	public:
		MidiSource(std::shared_ptr<apryx::MidiController> midi) : m_MidiIn(midi) {}


		virtual bool get(std::vector<double> &values, AudioFormat format);
	};

}