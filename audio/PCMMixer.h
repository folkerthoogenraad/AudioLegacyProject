#pragma once

#include "PCMSource.h"

namespace apryx {

	class PCMMixer : public PCMSource{
	public:
		virtual void get(std::vector<double> &values, AudioFormat format);
	};

}