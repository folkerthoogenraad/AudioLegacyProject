#include "MidiController.h"

namespace apryx {
	MidiController::MidiController()
	{
		m_MidiIn.ignoreTypes(false, false, false);

	}
	MidiController::~MidiController()
	{
		closePort();
	}

	int MidiController::getPortCount()
	{
		return m_MidiIn.getPortCount();
	}
	void MidiController::openPort(int port)
	{
		m_MidiIn.openPort(port);
	}
	void MidiController::closePort()
	{
		m_MidiIn.closePort();
	}
	const std::vector<MidiEvent>& MidiController::poll()
	{
		m_Events.clear();

		int nBytes = 0;
		do {
			std::vector<unsigned char> message;
			float stamp = m_MidiIn.getMessage(&message);


			nBytes = message.size();
			if (nBytes > 0) {
				/*std::cout << nBytes << " : ";
				for (int i = 0; i < nBytes; i++)
					std::cout << "Byte (" << i << "=" << (int)message[i] << ") ";
				std::cout << std::endl;*/
				
				apryx::MidiEvent event(message);
				m_Events.push_back(event);
			}

		} while (nBytes > 0);

		return getEvents();
	}
}
