#pragma once

namespace apryx {

	class VBO {
	private:
		unsigned int m_ID;

	public:

		void bind();
		void unbind();
	};

}
