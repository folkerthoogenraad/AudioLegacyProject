#pragma once

#define DEFAULT_VERTEX_SOURCE R"glsl(
    #version 150 core

    in vec3 position;

    void main()
    {
        gl_Position = vec4(position, 1.0);
    }
)glsl"


#define DEFAULT_FRAGMENT_SOURCE R"glsl(
    #version 150 core

	out vec4 color;

	void main()
	{
		color = vec4(1.0, 1.0, 1.0, 1.0);
	}
)glsl"

#define SHADER_POSITION_LOCATION 0
#define SHADER_POSITION_NAME "position"

namespace apryx {

	class Shader {
	public:
		enum Type {
			Vertex,
			Fragment
		};

	private:
		unsigned int m_ID;
		Type m_Type;

	public:
		Shader(Type type, const char *source);

		Type getType() const { return m_Type; }
		unsigned int getID() const { return m_ID; }
	};

}