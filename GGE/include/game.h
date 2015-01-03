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

#ifdef WIN32
	#pragma once
	#define _CRT_SECURE_NO_WARNINGS 1

	#define WIN32_LEAN_AND_MEAN 1
	#include <windows.h>
#endif

#include "GL/gl3w.h"

#define GLFW_NO_GLU 1
#define GLFW_INCLUDE_GLCOREARB 1
#include "GL/glfw3.h"

#include <stdio.h>
#include <string.h>

#ifndef GAME_H
#define GAME_H

namespace gge
{
	static	GLFWwindow *window;

	class game
	{
	public:
		game() {}
		virtual ~game() {}
		virtual void run(gge::game *the_app);
		virtual void init();
		virtual void startup() {}
		virtual void render(double currentTime) {}
		virtual void shutdown() {}
		virtual void onResize(int w, int h);
		virtual void onKey(int key, int scancode, int action, int mods) {}
		virtual void onMouseButton(int button, int action, int mods) {}
		virtual void onMouseMove(double x, double y) {}
		virtual void onMouseWheel(double x, double y) {}
		virtual void onDebugMessage(GLenum source,
									GLenum type,
									GLuint id,
									GLenum severity,
									GLsizei length,
									const GLchar* message);
		static void getMousePosition(double& x, double& y);

		struct APPINFO
		{
			char title[128];
			int windowWidth;
			int windowHeight;
			int majorVersion;
			int minorVersion;
			int samples;
			union
			{
				struct
				{
					unsigned int fullscreen : 1;
					unsigned int vsync : 1;
					unsigned int cursor : 1;
					unsigned int stereo : 1;
					unsigned int debug : 1;
				};
				unsigned int     all;
			} flags;
		};

	protected:
		APPINFO info;
		static  gge::game *app;

		static void glfw_onResize(GLFWwindow *window, int w, int h);
		static void glfw_onKey(GLFWwindow *window, int key, int scancode, int action, int mods);
		static void glfw_onMouseButton(GLFWwindow *window, int button, int action, int mods);
		static void glfw_onMouseMove(GLFWwindow *window, double x, double y);
		static void glfw_onMouseWheel(GLFWwindow *window, double x, double y);
		void setVsync(bool enable);
		static void APIENTRY debug_callback(GLenum source,
											GLenum type,
											GLuint id,
											GLenum severity,
											GLsizei length,
											const GLchar *message,
											GLvoid *userParam);
	};

};

#if defined WIN32
	#ifdef DEBUG
		#define DECLARE_MAIN(a)								\
        gge::game *app = 0;									\
        int main(int argc, const char **argv)				\
        {													\
            a *app = new a;									\
            app->run(app);									\
            delete app;										\
            return 0;										\
        }
	#else
		#define DECLARE_MAIN(a)								\
        gge::game *app = 0;									\
        int CALLBACK WinMain(HINSTANCE hInstance,			\
                             HINSTANCE hPrevInstance,		\
                             LPSTR lpCmdLine,				\
                             int nCmdShow)					\
        {													\
            a *app = new a;									\
            app->run(app);									\
            delete app;										\
            return 0;										\
        }
	#endif
#elif defined _LINUX || defined __APPLE__
		#define DECLARE_MAIN(a)								\
        gge::game *app = 0;									\
        int main(int argc, const char **argv)				\
        {													\
            a *app = new a;									\
            app->run(app);									\
            delete app;										\
            return 0;										\
        }
#else
	#error Undefined platform!
#endif

#endif /* GAME_H */