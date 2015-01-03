#include "main.h"

void basic_usage::startup()
{
	rendering_program = compile_shaders();
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
}

void basic_usage::render(double currentTime)
{
	const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);

	glUseProgram(rendering_program);

	glPointSize(5.0f);
	glDrawArrays(GL_PATCHES, 0, 3);
}

void basic_usage::shutdown()
{
	glDeleteProgram(rendering_program);
	glDeleteVertexArrays(1, &vertex_array_object);
}

GLuint basic_usage::compile_shaders()
{
	GLuint vertex_shader;
	GLuint tess_control_shader;
	GLuint tess_eval_shader;
	GLuint geometry_shader;
	GLuint fragment_shader;
	GLuint program;

	static const GLchar * vertex_shader_source[] = {
		"#version 400 core                                                  \n"
		"                                                                   \n"
		"void main(void)                                                    \n"
		"{                                                                  \n"
		"   const vec4 vertices[3] = vec4[3](vec4( 0.25, -0.25, 0.5, 1.0),  \n"
		"                                    vec4(-0.25, -0.25, 0.5, 1.0),  \n"
		"                                    vec4( 0.25,  0.25, 0.5, 1.0)); \n"
		"                                                                   \n"
		"   gl_Position = vertices[gl_VertexID];                            \n"
		"}                                                                  \n"
	};

	static const GLchar * tess_control_shader_source[] = {
		"#version 400 core                                                              \n"
		"                                                                               \n"
		"layout (vertices = 3) out;                                                     \n"
		"                                                                               \n"
		"void main(void)                                                                \n"
		"{                                                                              \n"
		"   if(gl_InvocationID == 0) {                                                  \n"
		"       gl_TessLevelInner[0] = 5.0;                                             \n"
		"       gl_TessLevelOuter[0] = 5.0;                                             \n"
		"       gl_TessLevelOuter[1] = 5.0;                                             \n"
		"       gl_TessLevelOuter[2] = 5.0;                                             \n"
		"   }                                                                           \n"
		"                                                                               \n"
		"   gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;   \n"
		"}                                                                              \n"
	};

	static const GLchar * tess_eval_shader_source[] = {
		"#version 400 core                                          \n"
		"                                                           \n"
		"layout (triangles, equal_spacing, cw) in;                  \n"
		"                                                           \n"
		"void main(void)                                            \n"
		"{                                                          \n"
		"   gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +  \n"
		"                  gl_TessCoord.y * gl_in[1].gl_Position +  \n"
		"                  gl_TessCoord.z * gl_in[2].gl_Position);  \n"
		"}                                                          \n"
	};

	static const GLchar * geometry_shader_source[] = {
		"#version 400 core                          \n"
		"                                           \n"
		"layout (triangles) in;                     \n"
		"layout (points, max_vertices = 3) out;     \n"
		"                                           \n"
		"void main(void)                            \n"
		"{                                          \n"
		"   int i;                                  \n"
		"                                           \n"
		"   for(i = 0; i < gl_in.length(); i++) {   \n"
		"       gl_Position = gl_in[i].gl_Position; \n"
		"       EmitVertex();                       \n"
		"   }                                       \n"
		"}                                          \n"
	};

	static const GLchar * fragment_shader_source[] = {
		"#version 400 core                                                          \n"
		"                                                                           \n"
		"out vec4 color;                                                            \n"
		"                                                                           \n"
		"void main(void)                                                            \n"
		"{                                                                          \n"
		"   color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,                    \n"
		"                cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,                    \n"
		"                sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),   \n"
		"                1.0);                                                      \n"
		"}                                                                          \n"
	};

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	tess_control_shader = glCreateShader(GL_TESS_CONTROL_SHADER);
	glShaderSource(tess_control_shader, 1, tess_control_shader_source, NULL);
	glCompileShader(tess_control_shader);

	tess_eval_shader = glCreateShader(GL_TESS_EVALUATION_SHADER);
	glShaderSource(tess_eval_shader, 1, tess_eval_shader_source, NULL);
	glCompileShader(tess_eval_shader);

	geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry_shader, 1, geometry_shader_source, NULL);
	glCompileShader(geometry_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, tess_control_shader);
	glAttachShader(program, tess_eval_shader);
	glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	glDeleteShader(vertex_shader);
	glDeleteShader(tess_control_shader);
	glDeleteShader(tess_eval_shader);
	glDeleteShader(geometry_shader);
	glDeleteShader(fragment_shader);

	return program;
}

DECLARE_MAIN(basic_usage);