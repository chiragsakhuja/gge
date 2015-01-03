/*
* The following is a modification of a file from the OpenGL SuperBible,
* 6th Edition.
* The Copyright notice is reproduced below.
*
*
* Copyright 2012-2013 Graham Sellers
*
* This code is part of the OpenGL SuperBible, 6th Edition.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice (including the next
* paragraph) shall be included in all copies or substantial portions of the
* Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
* DEALINGS IN THE SOFTWARE.
*/

#include "game.h"

gge::game *gge::game::app = 0;

void gge::game::run(gge::game *the_app)
{
	bool running = true;
	app = the_app;

	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

	init();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, info.majorVersion);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, info.minorVersion);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	glfwWindowHint(GLFW_STEREO, info.flags.stereo ? GL_TRUE : GL_FALSE);
	if(info.flags.fullscreen) {
		window = glfwCreateWindow(info.windowWidth, info.windowHeight, "OpenGL Window", glfwGetPrimaryMonitor(), NULL);
		glfwSwapInterval((int) info.flags.vsync);
	} else {
		window = glfwCreateWindow(info.windowWidth, info.windowHeight, "OpenGL Window", NULL, NULL);
	}

	if(!window) {
		fprintf(stderr, "Failed to open window\n");
		return;
	}

	glfwMakeContextCurrent(window);

	glfwSetWindowTitle(window, info.title);
	glfwSetWindowSizeCallback(window, glfw_onResize);
	glfwSetKeyCallback(window, glfw_onKey);
	glfwSetMouseButtonCallback(window, glfw_onMouseButton);
	glfwSetCursorPosCallback(window, glfw_onMouseMove);
	glfwSetScrollCallback(window, glfw_onMouseWheel);
	glfwSetInputMode(window, GLFW_CURSOR, info.flags.cursor ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);

	info.flags.stereo = (glfwGetWindowAttrib(window, GLFW_STEREO) ? 1 : 0);

	gl3wInit();

#ifdef DEBUG
	fprintf(stderr, "VENDOR: %s\n", (char *) glGetString(GL_VENDOR));
	fprintf(stderr, "VERSION: %s\n", (char *) glGetString(GL_VERSION));
	fprintf(stderr, "RENDERER: %s\n", (char *) glGetString(GL_RENDERER));
#endif

	if(info.flags.debug) {
		if(gl3wIsSupported(info.majorVersion, info.minorVersion)) {
			glDebugMessageCallback(debug_callback, this);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		}
	}

	startup();

	do {
		render(glfwGetTime());

		glfwSwapBuffers(window);
		glfwPollEvents();

		running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);
		running &= (glfwWindowShouldClose(window) == GL_FALSE);
	} while(running);

	shutdown();

	glfwTerminate();
}

void gge::game::init()
{
	strcpy(info.title, "OpenGL Window");
	info.windowWidth = 1280;
	info.windowHeight = 720;
#ifdef __APPLE__
	info.majorVersion = 4;
	info.minorVersion = 1;
#else
	info.majorVersion = 4;
	info.minorVersion = 0;
#endif
	info.samples = 0;
	info.flags.all = 0;
	info.flags.cursor = 1;
	info.flags.fullscreen = 0;
#ifdef DEBUG
	info.flags.debug = 1;
#endif
}

void gge::game::onResize(int w, int h)
{
	info.windowWidth = w;
	info.windowHeight = h;
}

void gge::game::onDebugMessage(GLenum source,
							   GLenum type,
							   GLuint id,
							   GLenum severity,
							   GLsizei length,
							   const GLchar* message)
{
#ifdef WIN32
	OutputDebugStringA(message);
	OutputDebugStringA("\n");
#endif
}

void gge::game::getMousePosition(double& x, double &y)
{
	glfwGetCursorPos(window, &x, &y);
}

void gge::game::glfw_onResize(GLFWwindow *window, int w, int h)
{
	app->onResize(w, h);
}

void gge::game::glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	app->onKey(key, scancode, action, mods);
}

void gge::game::glfw_onMouseButton(GLFWwindow *window, int button, int action, int mods)
{
	app->onMouseButton(button, action, mods);
}

void gge::game::glfw_onMouseMove(GLFWwindow *window, double x, double y)
{
	app->onMouseMove(x, y);
}

void gge::game::glfw_onMouseWheel(GLFWwindow *window, double x, double y)
{
	app->onMouseWheel(x, y);
}

void gge::game::setVsync(bool enable)
{
	info.flags.vsync = enable ? 1 : 0;
	glfwSwapInterval((int) info.flags.vsync);
}

void APIENTRY gge::game::debug_callback(GLenum source,
										GLenum type,
										GLuint id,
										GLenum severity,
										GLsizei length,
										const GLchar *message,
										GLvoid *userParam)
{
	reinterpret_cast<game *>(userParam)->onDebugMessage(source, type, id, severity, length, message);
}