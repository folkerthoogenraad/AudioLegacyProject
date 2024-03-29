#pragma once

#define SEMITONE_UP 1.05946309436
#define SEMITONE_DOWN 0.94387431268
#define OCTAVE_UP 2.0
#define OCTAVE_DOWN 0.5

#define NOTE_C4			261.63
#define NOTE_Db4		277.18
#define NOTE_D4			293.66
#define NOTE_Eb4		311.13
#define NOTE_E4			329.63
#define NOTE_F4			349.23
#define NOTE_Gb4		369.99
#define NOTE_G4			392.00
#define NOTE_Ab4		415.30
#define NOTE_A4			440
#define NOTE_Bb4		466.16
#define NOTE_B4			493.88

#include <complex> // For fft and ifft
#include <vector>

#include "AudioFormat.h"

namespace apryx {
	double toGain(double db);
	double toDB(double gain);
	double toBarSeconds(double bpm, double beatsPerBar);

	size_t toSamples(double seconds, AudioFormat format);

	// Falloff in normalized distance (1 distance = normal gain)
	double gainFalloff(double distance);

	double semitonesMultiplier(double semitones);
	double intervalMultiplier(double from, double to);

	// All these waveforms have a period of 1 (not 2PI!)
	double audioSine(double input);
	double audioHalfSine(double input);
	double audioSawtooth(double input);
	double audioTriangle(double input);
	double audioSquare(double input);

	double audioMix(double in1, double in2);

	double random();

	double midiToFrequency(int midiKey);

	double audioRandom();
	double audioLerp(double a, double b, double v);
	double audioClamp(double value, double min = 0, double max = 1);

	double remap(double startMin, double startMax, double min, double max, double value);

	double sinc(double input);
	void convolute(std::vector<std::complex<double>> &a, std::vector<std::complex<double>> &b, std::vector<std::complex<double>> &out);
	void fft(std::vector<std::complex<double>> &samples);
	void ifft(std::vector<std::complex<double>> &frequencies);
}