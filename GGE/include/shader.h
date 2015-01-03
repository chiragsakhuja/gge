#ifndef SHADER_H
#define SHADER_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <map>

#include "GL/gl3w.h"

#ifdef WIN32
	#pragma once
	#define _CRT_SECURE_NO_WARNINGS 1
#endif

class Shader
{
private:
	void addUniformArrayRec(std::string match, const unsigned int *counts, unsigned int depth, unsigned int current, unsigned int *scratch, char *failure);

public:
	GLuint program;
	std::map<std::string, GLuint> uniforms;

	Shader();

	void addShader(GLenum type, const char *file_name);
	void compileShader();
	void addUniform(std::string name);
	void addUniformArray(std::string match, const unsigned int *counts, unsigned int depth);
	void enable();
	void disable();
};

#endif