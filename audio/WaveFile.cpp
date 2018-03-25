#include "WaveFile.h"

#include <fstream>
#include <stdint.h>
#include <stdexcept>

#include <limits.h>

#define RIFF (uint32_t) 0x52494646
#define WAVE (uint32_t) 0x57415645

#define FMT	 (uint32_t) 0x666d7420
#define DATA (uint32_t) 0x64617461

namespace apryx {

	WaveFile::WaveFile(std::string file)
	{
		std::ifstream stream(file, std::ios::in | std::ios::binary);

		if (!stream) {
			throw std::runtime_error("Coundn't find wavefile.");
		}

		auto readUint8 = [&](uint8_t &v) {
			stream.read((char*)&v, sizeof(uint8_t));

			if (!stream) {
				throw std::runtime_error("Reading error.");
			}
		};

		auto readBigEndian = [&](uint32_t &v) {
			uint8_t a, b, c, d;

			readUint8(a);
			readUint8(b);
			readUint8(c);
			readUint8(d);

			v = a << 24 | b << 16 | c << 8 | d;
		};

		auto readUint32 = [&](uint32_t &v) {
			uint8_t a, b, c, d;

			readUint8(a);
			readUint8(b);
			readUint8(c);
			readUint8(d);

			v = d << 24 | c << 16 | b << 8 | a;
		};

		auto readUint16 = [&](uint16_t &v) {
			uint8_t a, b;

			readUint8(a);
			readUint8(b);

			v = b << 8 | a;
		};

		auto readInt16 = [&](int16_t &v) {
			uint8_t a, b;

			readUint8(a);
			readUint8(b);

			v = b << 8 | a;
		};


		// ======================================//
		// Wave header
		// ======================================//
		uint32_t header, chunkSize, format;

		readBigEndian(header);

		if (header != RIFF){
			throw std::runtime_error("Not a wave file");
		}

		readUint32(chunkSize);

		readBigEndian(format);

		if (format != WAVE) {
			throw std::runtime_error("Not a wave file");
		}

		// ======================================//
		// FMT chunk
		// ======================================//

		uint32_t fmtChunkID, fmtChunkSize;
		uint16_t audioCompressionFormat;

		readBigEndian(fmtChunkID);

		if (fmtChunkID != FMT) {
			throw std::runtime_error("Incorrectly formatted wave file");
		}

		readUint32(fmtChunkSize);

		if (fmtChunkSize != 16) {
			throw std::runtime_error("Unsupported format");
		}

		readUint16(audioCompressionFormat);

		if (audioCompressionFormat != 1) {
			throw std::runtime_error("Unsupported compression (only PCM)");
		}

		uint16_t numChannels, bitsPerSample, blockAlign;
		uint32_t sampleRate, byteRate;

		readUint16(numChannels);
		readUint32(sampleRate);

		readUint32(byteRate);

		readUint16(blockAlign);
		readUint16(bitsPerSample);

		uint32_t bytesPerSample = bitsPerSample / 8;

		if (sampleRate * numChannels * bytesPerSample != byteRate) {
			throw std::runtime_error("Byterate does not fit the specified sample rate and channels");
		}

		// Set the stuff correctly.
		m_Format.sampleRate = sampleRate;
		m_Format.channels = numChannels;


		// ======================================//
		// Data chunk
		// ======================================//

		uint32_t dataHeader;

		readBigEndian(dataHeader);

		if (dataHeader != DATA) {
			throw std::runtime_error("Incorrectly formatted wave file");
		}

		if (bytesPerSample != 2) {
			throw std::runtime_error("Currently unsupported format");
		}

		uint32_t dataByteSize;
		
		readUint32(dataByteSize);

		uint32_t sampleCount = dataByteSize / bytesPerSample;


		m_Samples.resize(sampleCount);

		// TODO no indivitual reads but chunk reading :)

		for (size_t i = 0; i < sampleCount; i++){
			int16_t value;

			readInt16(value);

			m_Samples[i] = ((float)value) / SHRT_MAX;
		}
	}

}
