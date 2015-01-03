Grim Game Engine
===

Grim Game Engine (GGE) is a game engine framework that targets high end machines and uses modern OpenGL.

Current Features
* An inheritable object that handles context creation and wraps the game loop functions.

Usage
===

Currently GGE is only supported by Visual Studio 2013 (v11). The source code is theoretically platform-independent, however the only project files included are for Visual Studio.
This project includes GGE as well as a sample project that demonstrates proper usage of the framework. Upon opening the Visual Studio solution, you will see the following projects:
* GGE - the project for the actual framework.
* basic_usage - a simple project that displays the points in a tesselated triangle using GGE.