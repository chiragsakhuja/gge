#include "shader.h"

Shader::Shader()
{
	program = glCreateProgram();

	if(!program) {
		fprintf(stderr, "Error creating shader program\n");
	}
}

void Shader::addShader(GLenum type, const char *file_name)
{
	FILE *file = fopen(file_name, "r");

	if(!file) {
		fprintf(stderr, "Error loading %s\n", file_name);
	}

	GLuint shader = glCreateShader(type);

	if(!shader) {
		fprintf(stderr, "Error creating shader type %d\n", type);
	}

	fseek(file, 0, SEEK_END);
	GLint file_size[1];
	file_size[0] = ftell(file);
	rewind(file);
	char *source = (char*) calloc(sizeof(char), file_size[0]);
	fread(source, 1, file_size[0], file);
	const GLchar *data[1];
	data[0] = source;

	fclose(file);

	glShaderSource(shader, 1, data, file_size);
	glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if(!status) {
		GLchar log[1024];
		glGetShaderInfoLog(shader, 1024, NULL, log);
		fprintf(stderr, "Error compiling shader type %d\n%s", type, log);
	}

	glAttachShader(program, shader);

	free(source);
}

void Shader::compileShader()
{
	GLint status = 0;
	GLchar log[1024] = {0};

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if(!status) {
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		fprintf(stderr, "Error linking shader program\n%s", log);
	}

	glValidateProgram(program);
	glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
	if(!status) {
		glGetProgramInfoLog(program, sizeof(log), NULL, log);
		fprintf(stderr, "Invalid shader program\n%s", log);
	}
}

void Shader::addUniform(std::string name)
{
	uniforms[name] = glGetUniformLocation(program, name.c_str());

	if(uniforms[name] == (GLuint) -1) {
		fprintf(stderr, "Invalid uniform %s\n", name.c_str());
	}
}

void Shader::addUniformArray(std::string match, const unsigned int *counts, unsigned int depth)
{
	unsigned int *scratch = (unsigned int *) malloc(depth * sizeof(unsigned int));
	char failure[128] = {0};
	addUniformArrayRec(match, counts, depth, 0, scratch, failure);
	free(scratch);

	if(strlen(failure) != 0) {
		fprintf(stderr, "Invalid uniform %s\n", failure);
	}
}

void Shader::addUniformArrayRec(std::string match, const unsigned int *counts, unsigned int depth, unsigned int current, unsigned int *scratch, char *failure)
{
	if(current == depth) {
		std::string copy(match);
		for(unsigned int i = 0; i < depth; i++) {
			size_t pos = copy.find("%d");

			std::string left = copy.substr(0, pos + 2);
			std::string right = copy.substr(pos + 2);

			char buffer[128] = {0};
			sprintf(buffer, left.c_str(), scratch[i]);
			copy = std::string(buffer) + right;
		}

		uniforms[copy] = glGetUniformLocation(program, copy.c_str());

		if(uniforms[copy] == (GLuint) -1) {
			strcpy(failure, copy.c_str());
		}

		return;
	}

	for(unsigned int i = 0; i < counts[current]; i++) {
		scratch[current] = i;

		addUniformArrayRec(match, counts, depth, current + 1, scratch, failure);

		if(strlen(failure) != 0) {
			return;
		}
	}
}

void Shader::enable()
{
	glUseProgram(program);
}

void Shader::disable()
{
	glUseProgram(GL_NONE);
}
