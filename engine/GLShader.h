#pragma once

#define VERTEX_DEFAULT_SOURCE R"glsl(
    #version 150 core

	uniform mat4 u_MatrixModel;
	uniform mat4 u_MatrixView;
	uniform mat4 u_MatrixProjection;

    in vec3 position;
	in vec2 uv;
	in vec4 color;
	
	out vec2 v_UV;
	out vec4 v_Color;

    void main()
    {
		v_UV = uv;
		v_Color = color;
        gl_Position = u_MatrixProjection * u_MatrixView * u_MatrixModel * vec4(position, 1.0);
    }
)glsl"

#define FRAGMENT_UNLIT_COLOR R"glsl(
    #version 150 core

	in vec4 v_Color;

	out vec4 outColor;

	void main()
	{
		outColor = v_Color;
	}
)glsl"

#define FRAGMENT_UNLIT_TEXTURE R"glsl(
    #version 150 core

	in vec2 v_UV;
	in vec4 v_Color;

	out vec4 outColor;

	void main()
	{
		outColor = vec4(1.0, 0.0, 1.0, 1.0);
	}
)glsl"

#define FRAGMENT_ERROR_SOURCE R"glsl(
    #version 150 core

	out vec4 outColor;

	void main()
	{
		outColor = vec4(1.0, 0.0, 1.0, 1.0);
	}
)glsl"

#define SHADER_POSITION_LOCATION 0
#define SHADER_POSITION_NAME "position"
#define SHADER_UV_LOCATION 1
#define SHADER_UV_NAME "uv"
#define SHADER_COLOR_LOCATION 2
#define SHADER_COLOR_NAME "color"

#define SHADER_MATRIX_MODEL "u_MatrixModel"
#define SHADER_MATRIX_VIEW "u_MatrixView"
#define SHADER_MATRIX_PROJECTION "u_MatrixProjection"

namespace apryx {

	class GLShader {
	public:
		enum Type {
			Vertex,
			Fragment
		};

	private:
		unsigned int m_ID;
		Type m_Type;

	public:
		GLShader(Type type, const char *source);

		Type getType() const { return m_Type; }
		unsigned int getID() const { return m_ID; }
	};

}