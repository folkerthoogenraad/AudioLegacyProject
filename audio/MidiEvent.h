#pragma once

#define MIDI_MESSAGE_MASK				0b11110000
#define MIDI_CHANNEL_MASK				0b00001111

#define MIDI_NOTE_ON					0b10010000
#define MIDI_NOTE_OFF					0b10000000

#define MIDI_PITCH_BEND					0b11100000

#define MIDI_PROGRAM_CHANGE				0b11000000

#define MIDI_CONTROL_CHANGE				0b10110000

#include <array>
#include <vector>

namespace apryx {
	class MidiEvent {
		std::array<unsigned char, 3> m_Bytes;

	public:
		MidiEvent(std::array<unsigned char, 3> values);
		MidiEvent(std::vector<unsigned char> values);
		MidiEvent(unsigned char a, unsigned char b, unsigned char c);

		// Emptyness
		bool isEmpty() const;

		// Returns the midi channel
		int getMidiChannel() const;

		// =================================================== //
		// Note events
		// =================================================== //
		bool isNoteOn() const;
		bool isNoteOff() const;
		double getVelocity() const; // Gets the velocity in 0 to 1 range
		int getKey() const;// Get the pressed key (where 0 is C1, and 69 is A6 (440Hz) )
		double getKeyFrequency() const;// Gets the converted frequency of the current note (helper function)


		// =================================================== //
		// Program events
		// =================================================== //
		bool isProgramChange() const;
		int getProgramNumber() const;


		// =================================================== //
		// Pitch bend
		// =================================================== //
		bool isPitchBend() const;
		double getPitchBendAmount() const; // Returns the amount of pitch bend in -1 to 1 range


		// =================================================== //
		// Control change
		// =================================================== //
		bool isControlChange() const;
		int getControlIndex() const; // Returns the index of the changed controller
		double getControlValue() const; // Returns control value in 0 to 1 range
	};
}