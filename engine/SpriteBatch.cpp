#include "SpriteBatch.h"

#include "engine/GL.h"

namespace apryx {

	SpriteBatch::SpriteBatch()
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

	void SpriteBatch::begin()
	{
		if (m_Drawing)
			return;

		m_Drawing = true;
		m_Index = 0;
	}
	void SpriteBatch::end()
	{
		if (!m_Drawing)
			return;

		flush();
		m_Drawing = false;
	}
	void SpriteBatch::flush()
	{
		if (!m_Drawing) {
			m_Index = 0; // This is actually a bit of a hack
			return;
		}

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

	void SpriteBatch::setMatrixModel(Matrix4f matrix)
	{
		flush();
		m_MatrixModel = matrix;
	}

	void SpriteBatch::setMatrixView(Matrix4f matrix)
	{
		flush();
		m_MatrixView = matrix;
	}

	void SpriteBatch::setMatrixProjection(Matrix4f matrix)
	{
		flush();
		m_MatrixProjection = matrix;
	}

	void SpriteBatch::texture(std::shared_ptr<GLTexture> texture)
	{
		if (texture == m_Texture)
			return;

		m_Texture = texture;
		
		flush();
	}

	void SpriteBatch::vertex(Vector3f vertex)
	{
		m_CurrentVertex.vertex = vertex;

		m_Vertices[m_Index] = m_CurrentVertex;
		
		m_Index++;

		if (m_Index >= m_Vertices.size())
			flush();
	}

	void SpriteBatch::uv(Vector2f uv)
	{
		m_CurrentVertex.uv = uv;
	}

	void SpriteBatch::color(Color32 color)
	{
		m_CurrentVertex.color = color;
	}
}