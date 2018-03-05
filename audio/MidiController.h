#pragma once

#include "rtmidi\RtMidi.h"

#include "MidiEvent.h"

namespace apryx {

	class MidiController {
		RtMidiIn m_MidiIn;
		std::vector<MidiEvent> m_Events;
	public:
		MidiController();
		~MidiController();

		// Basically mapping to RtMidi here.
		int getPortCount();
		void openPort(int port);
		void closePort();

		const std::vector<MidiEvent> &poll();

		const std::vector<MidiEvent> &getEvents() const { return m_Events; };
	};

}