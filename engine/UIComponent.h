#pragma once

#include "ApryxUI.h"

#include "math/Rectangle.h"

namespace apryx {

	// TODO rename
	template<typename T>
	struct UIFourside{
		T top;
		T bottom;
		T left;
		T right;
	};

	struct UIValue {
		enum Unit{
			Pixels,
			DensityPixels,
			Percentage
		} unit;
		float value;

		static UIValue px(float px) { return { Pixels, px }; };
		static UIValue dp(float dp) { return { DensityPixels, dp}; };
		static UIValue percent(float percentage) { return { Percentage, percentage }; };
	};

	template<typename T>
	struct UILayout {
		T width;
		T height;

		UIFourside<T> margin;
		UIFourside<T> padding;
		UIFourside<T> borderWidth;
	};


	class UIComponent {
		UILayout<UIValue> m_LayoutParams;
		UILayout<float> m_EvaluatedParams;

	public:

		void updateLayout(const UIComponent *parent, const UIComponent *previousChild);
	};

}