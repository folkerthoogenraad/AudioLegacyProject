#include "GLBatch.h"

#include "engine/GL.h"

namespace apryx {

	GLBatch::GLBatch()
		: m_Program(
			GLShader(GLShader::Vertex, VERTEX_DEFAULT_SOURCE),
			GLShader(GLShader::Fragment, FRAGMENT_UNLIT_TEXTURE))
	{
		// Allocate the buffer
		m_VertexBufferObject.setBufferData(GLVertexBufferObject::Dynamic, sizeof(Vertex) * m_Vertices.size(), nullptr);
	
		// Find the indices for the model and view matrix
		m_MatrixModelIndex = m_Program.getUniformLocation(SHADER_MATRIX_MODEL);
		m_MatrixViewIndex = m_Program.getUniformLocation(SHADER_MATRIX_VIEW);
		m_MatrixProjectionIndex = m_Program.getUniformLocation(SHADER_MATRIX_PROJECTION);
		m_TextureLocation = m_Program.getUniformLocation(SHADER_MAIN_TEXTURE);

		m_MatrixModel = Matrix4f::identity();
		m_MatrixView = Matrix4f::identity();
		m_MatrixProjection = Matrix4f::identity();
	}

	void GLBatch::begin()
	{
		if (m_Drawing)
			return;

		m_Drawing = true;
		m_Index = 0;
	}
	void GLBatch::end()
	{
		if (!m_Drawing)
			return;

		flush();
		m_Drawing = false;
	}
	void GLBatch::flush()
	{
		if (!m_Drawing) {
			m_Index = 0; // This is actually a bit of a hack
			return;
		}

		if (m_Index == 0)
			return;

		// Upload the data
		m_VertexBufferObject.setBufferSubData(0, sizeof(Vertex) * m_Index, &m_Vertices[0]);

		// Bind the buffer
		m_VertexBufferObject.bind();

		glEnableVertexAttribArray(SHADER_POSITION_LOCATION);
		glEnableVertexAttribArray(SHADER_COLOR_LOCATION);
		glEnableVertexAttribArray(SHADER_UV_LOCATION);

		glVertexAttribPointer(SHADER_POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, vertex));
		glVertexAttribPointer(SHADER_COLOR_LOCATION, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*) offsetof(Vertex, color));
		glVertexAttribPointer(SHADER_UV_LOCATION, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, uv));

		m_Program.use();

		m_Program.setUniform(m_MatrixModelIndex, m_MatrixModel);
		m_Program.setUniform(m_MatrixViewIndex, m_MatrixView);
		m_Program.setUniform(m_MatrixProjectionIndex, m_MatrixProjection);

		m_Program.setUniform(m_TextureLocation, 0);

		if(m_Texture != nullptr)
			m_Texture->bind();

		glDrawArrays(GL_QUADS, 0, m_Index);

		glDisableVertexAttribArray(SHADER_POSITION_LOCATION);
		glDisableVertexAttribArray(SHADER_COLOR_LOCATION);
		glDisableVertexAttribArray(SHADER_UV_LOCATION);

		m_Index = 0;
	}

	void GLBatch::setMatrixModel(Matrix4f matrix)
	{
		flush();
		m_MatrixModel = matrix;
	}

	void GLBatch::setMatrixView(Matrix4f matrix)
	{
		flush();
		m_MatrixView = matrix;
	}

	void GLBatch::setMatrixProjection(Matrix4f matrix)
	{
		flush();
		m_MatrixProjection = matrix;
	}

	void GLBatch::setSize(float width, float height) 
	{
		setMatrixProjection(Matrix4f::orthographic(0, width, height, 0, -10, 10));
	}

	void GLBatch::texture(std::shared_ptr<GLTexture> texture)
	{
		if (texture == m_Texture)
			return;

		flush();

		m_Texture = texture;
	}

	void GLBatch::vertex(Vector2f v)
	{
		vertex(Vector3f(v.x, v.y, 0));
	}

	void GLBatch::vertex(Vector3f vertex)
	{
		m_CurrentVertex.vertex = vertex;

		m_Vertices[m_Index] = m_CurrentVertex;
		
		m_Index++;

		if (m_Index >= m_Vertices.size())
			flush();
	}

	void GLBatch::uv(Vector2f uv)
	{
		m_CurrentVertex.uv = uv;
	}

	void GLBatch::color(Color32 color)
	{
		m_CurrentVertex.color = color;
	}
}
