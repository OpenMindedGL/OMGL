#include "Debug.h"
#include <GL/glew.h>
#include <stdio.h>


void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		printf("OpenGL Error ( %d ) : %s, %s, %d", error, function, file, line);
		return false;
	}
	return true;
}
