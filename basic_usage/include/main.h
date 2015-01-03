#include "game.h"
#include "shader.h"

class basic_usage : public gge::game
{
public:
	void startup();
	void render(double currentTime);
	void shutdown();

private:
	GLuint vertex_array_object;
	Shader *program;

	void setupShaders();
};
