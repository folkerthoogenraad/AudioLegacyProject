#pragma once

#include "PCMSource.h"

namespace apryx {

	class PCMMixer : public PCMSource {
		std::vector<std::shared_ptr<PCMSource>> m_Sources;
		std::vector<double> m_Buffer;
	public:
		virtual bool get(std::vector<double> &values, AudioFormat format) override;

		void addSource(std::shared_ptr<PCMSource> source) { m_Sources.push_back(std::move(source)); }
	};

}