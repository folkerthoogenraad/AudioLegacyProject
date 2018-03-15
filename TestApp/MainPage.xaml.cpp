//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"

#include "audio/AudioUtils.h"

using namespace TestApp;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace apryx;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409



MainPage::MainPage()
{ 
	auto r = Windows::Foundation::Initialize(RO_INIT_MULTITHREADED);

	InitializeComponent();

	AudioFormat format;

	m_System = std::make_shared<AudioSystem>();
	m_Source = std::make_shared<TestSource>();

	AudioSystem system;

	system.play(format, m_Source);
	
}

void TestApp::MainPage::Button_Click(Platform::Object ^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (m_Source != nullptr) {
		m_Source->playing = true;
		m_Source->phase = 0;
	}
}

void TestApp::MainPage::Button_Release(Platform::Object ^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e)
{
	if (m_Source != nullptr) {
		m_Source->playing = false;
	}
}

bool apryx::TestSource::get(std::vector<double> &values, apryx::AudioFormat format)
{
	std::cout << "moeder" << std::endl;

	if (!playing)
		return false;

	for (int i = 0; i < values.size() / format.channels; i++) {

		double value = apryx::audioSine(phase) * 0.5;

		phase += 440.0 / (double)format.sampleRate;

		for (int j = 0; j < format.channels; j++)
			values[i + j] = value;

	}

	return true;
}
