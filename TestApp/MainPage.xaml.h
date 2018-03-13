//
// MainPage.xaml.h
// Declaration of the MainPage class.
//

#pragma once

#include "MainPage.g.h"

#include "audio/AudioSystem.h"

namespace apryx {
	class TestSource : public apryx::PCMSource {
	public:
		double phase;
		bool playing = false;
	public:
		virtual bool get(std::vector<double> &values, apryx::AudioFormat format);
	};
}

namespace TestApp
{
	/// <summary>
	/// An empty page that can be used on its own or navigated to within a Frame.
	/// </summary>
	public ref class MainPage sealed
	{
	public:
		MainPage();

		void Button_Click(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
		void Button_Release(Platform::Object^ sender, Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
	};

}
