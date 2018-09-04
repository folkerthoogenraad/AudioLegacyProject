#include "MidiSource.h"

#include "audio/AudioUtils.h"

#include "math/math.h"

namespace apryx {
	MidiSource::MidiSource(std::shared_ptr<apryx::MidiController> midi)
		: m_MidiIn(midi)
	{
		m_Voices.resize(20);
		m_CurrentVoiceIndex = 0;
	}
	bool MidiSource::get(std::vector<double>& values, AudioFormat format)
	{
		// Process the midi events.
		for (auto &event : m_MidiIn->poll()) {

			if (event.isControlChange()) {
				//std::cout << event.getControlIndex() << " = " << event.getControlValue() << std::endl;
			}
			if (event.isProgramChange()) {
				//std::cout << "Goto program " << event.getProgramNumber() << std::endl;
			}

			if (event.isNoteOn()) {
				MidiVoice voice;
				voice.wavePhase = random();
				voice.key = event.getKey();
				voice.envelopePhase = MidiVoice::Attack;
				voice.envelopeTimer = 0;
				voice.velocity = event.getVelocity();
				voice.frequency = event.getKeyFrequency();
				voice.enabled = true;

				m_Voices[m_CurrentVoiceIndex++] = voice;
				if (m_CurrentVoiceIndex >= m_Voices.size())
					m_CurrentVoiceIndex = 0;
			}

			if (event.isNoteOff()) {
				for (int i = 0; i < m_Voices.size(); i++) {
					if (m_Voices[i].key == event.getKey()) {
						m_Voices[i].envelopeTimer = 0;
						m_Voices[i].envelopePhase = MidiVoice::Release;
					}
				}
			}

			if (event.isControlChange()) {
				double f = event.getControlValue();

				if (event.getControlIndex() == 2) {
					double r = remap(0, 1, 40, 20000, f * f * f);

					m_FilterCutoff = r;

				}
				if (event.getControlIndex() == 3) {

					double r = remap(0, 1, 0.7071, 10, f * f * f);

					m_FilterRes = r;
				}
				if (event.getControlIndex() == 4) {

					double r = remap(0, 1, 0.0, 0.5, f);

					m_Detune = r;
				}

				if (event.getControlIndex() == 5) {

					double r = remap(0, 1, 200, 5000, f * f);

					m_DelaySamples = (size_t)r;
				}

				if (event.getControlIndex() == 6) {

					double r = remap(0, 1, -1, 1, f);
					m_DelayGain = r;
				}
			}
		}
	
		// Play the current voices
		for (auto &voice : m_Voices) {

			if (!voice.enabled)
				continue;

			for (int i = 0; i < values.size() / format.channels; i++) {

				double volume = 0;

				if (voice.envelopePhase == MidiVoice::Attack) {
					if (voice.envelopeTimer > m_EnvelopeAttack) {
						voice.envelopePhase = MidiVoice::Decay;
						voice.envelopeTimer -= m_EnvelopeAttack;
					}
					else {
						volume = audioLerp(0, 1, voice.envelopeTimer / m_EnvelopeAttack);
					}
				}
				if (voice.envelopePhase == MidiVoice::Decay) {
					if (voice.envelopeTimer > m_EnvelopeDecay) {
						voice.envelopePhase = MidiVoice::Sustain;
						voice.envelopeTimer -= m_EnvelopeDecay;
					}
					else {
						volume = audioLerp(1, m_EnvelopeSustain, voice.envelopeTimer / m_EnvelopeDecay);
					}
				}
				if (voice.envelopePhase == MidiVoice::Sustain) {
					volume = m_EnvelopeSustain;
				}
				if (voice.envelopePhase == MidiVoice::Release) {
					volume = audioLerp(voice.volume, 0, pow(voice.envelopeTimer / m_EnvelopeRelease, 0.5));

					if (voice.envelopeTimer > m_EnvelopeRelease) {
						voice.enabled = false;
						break;
					}
				}

				// Prevent clicking when releasing before sustain.
				if (voice.envelopePhase != MidiVoice::Release) {
					voice.volume = volume;
				}

				volume = audioClamp(volume);

				// Create the final output
				double gain = volume * m_Volume * toGain(-(1 - voice.velocity) * 30);
				double value = apryx::audioSawtooth(voice.wavePhase) * gain;

				value = (
					apryx::audioSine(voice.wavePhase) + 
					apryx::audioSine(voice.wavePhase*1.99) + 
					apryx::audioSine(voice.wavePhase*3) + 
					apryx::audioSine(voice.wavePhase*4)
					) * 0.5 * gain;

				// Write the values
				for (int j = 0; j < format.channels; j++)
					values[i * format.channels + j] += value;


				// This is really inefficient but I just want it working
				voice.envelopeTimer += 1.0 / (double)format.sampleRate;
				voice.wavePhase += voice.frequency / (double)format.sampleRate;
			}

		}

		delay.setDelay(m_DelaySamples);
		delay.setFeedback(m_DelayGain);
		filter.setFilter(IIRFilter::LowPass, m_FilterCutoff / (double)format.sampleRate, m_FilterRes, 0);

		for (int i = 0; i < values.size() / format.channels; i++) {
			double value = values[i * format.channels];

			value = filter.process(value);
			//value = delay1.process(value);

			for (int j = 0; j < format.channels; j++)
				values[i * format.channels + j] = value;
		}
	

		/*m_Voices.erase(std::remove_if(m_Voices.begin(),
			m_Voices.end(),
			[this](auto &voice) {
				if (voice.envelopePhase == MidiVoice::Release && voice.envelopeTimer > m_EnvelopeRelease) {
					return true;
				}
				return false; 
			}),
			m_Voices.end());*/

		return true;// m_Voices.size() > 0;
	}
}
