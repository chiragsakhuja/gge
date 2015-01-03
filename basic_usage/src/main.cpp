#include "main.h"

void basic_usage::startup()
{
	setupShaders();
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);
}

void basic_usage::render(double currentTime)
{
	const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);
	
	glPointSize(5.0f);
	glDrawArrays(GL_PATCHES, 0, 3);
}

void basic_usage::shutdown()
{
	delete program;
	glDeleteVertexArrays(1, &vertex_array_object);
}

void basic_usage::setupShaders()
{
	program = new Shader();
	program->addShader(GL_VERTEX_SHADER, "res/shaders/basic.vert");
	program->addShader(GL_TESS_CONTROL_SHADER, "res/shaders/basic.tcont");
	program->addShader(GL_TESS_EVALUATION_SHADER, "res/shaders/basic.teval");
	program->addShader(GL_GEOMETRY_SHADER, "res/shaders/basic.geom");
	program->addShader(GL_FRAGMENT_SHADER, "res/shaders/basic.frag");
	program->compileShader();
	program->enable();
}

DECLARE_MAIN(basic_usage);