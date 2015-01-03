#include "game.h"

class basic_usage : public gge::game
{
public:
	void startup();
	void render(double currentTime);
	void shutdown();

private:
	GLuint rendering_program;
	GLuint vertex_array_object;

	GLuint compile_shaders();
};
