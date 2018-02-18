#pragma once

namespace apryx {

	class GLVertexBufferObject {
	public:
		enum DrawType {
			Stream,
			Static,
			Dynamic
		};
	private:
		unsigned int m_ID;
	public:
		GLVertexBufferObject();

		void bind();
		void unbind();

		void setBufferData(DrawType type, size_t size, const void *data);
		void setBufferSubData(size_t offset, size_t size, const void *data);
	};

}
