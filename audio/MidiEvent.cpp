#include "MidiEvent.h"

#include "MidiEvent.h"


namespace apryx {

	static inline bool isMidiMessage(int value, int message) {
		return (value & MIDI_MESSAGE_MASK) == message;
	}


	MidiEvent::MidiEvent(std::array<unsigned char, 3> values)
		:m_Bytes(values)
	{ }

	MidiEvent::MidiEvent(std::vector<unsigned char> values)
	{
		// Reset all the values
		for (int i = 0; i < m_Bytes.size(); i++) {
			m_Bytes[i] = 0;
		}

		for (int i = 0; i < m_Bytes.size() && i < values.size(); i++) {
			m_Bytes[i] = values[i];
		}
	}

	MidiEvent::MidiEvent(unsigned char a, unsigned char b, unsigned char c)
		: m_Bytes({ a, b, c })
	{ }

	bool MidiEvent::isEmpty() const
	{
		return (m_Bytes[0] & MIDI_MESSAGE_MASK) == 0;
	}

	int MidiEvent::getMidiChannel() const
	{
		return m_Bytes[0] & MIDI_CHANNEL_MASK;
	}

	bool MidiEvent::isNoteOn() const
	{
		return isMidiMessage(m_Bytes[0], MIDI_NOTE_ON);
	}

	bool MidiEvent::isNoteOff() const
	{
		return isMidiMessage(m_Bytes[0], MIDI_NOTE_OFF) || (isNoteOn() && getVelocity() == 0);
	}

	double MidiEvent::getVelocity() const
	{
		return m_Bytes[2] / 127.0;
	}

	int MidiEvent::getKey() const
	{
		return m_Bytes[1];
	}
	double MidiEvent::getKeyFrequency() const
	{
		return pow(2, (getKey() - 69) / 12.f) * 440;
	}

	bool MidiEvent::isPitchBend() const
	{
		return isMidiMessage(m_Bytes[0], MIDI_PITCH_BEND);
	}
	double MidiEvent::getPitchBendAmount() const
	{
		unsigned int low = m_Bytes[1];
		unsigned int high = m_Bytes[2];

		unsigned int result = low | (high << 7);

		return ((double)result - 0x2000) / 0x2000;
	}

	bool MidiEvent::isControlChange() const
	{
		return isMidiMessage(m_Bytes[0], MIDI_CONTROL_CHANGE);
	}
	int MidiEvent::getControlIndex() const
	{
		return m_Bytes[1];
	}
	double MidiEvent::getControlValue() const
	{
		return m_Bytes[2] / 127.0;
	}

	bool MidiEvent::isProgramChange() const
	{
		return isMidiMessage(m_Bytes[0], MIDI_PROGRAM_CHANGE);
	}
	int MidiEvent::getProgramNumber() const
	{
		return m_Bytes[1];
	}
}
