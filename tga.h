#ifndef SOLARSYSTEM_TGA_H
#define SOLARSYSTEM_TGA_H

#ifdef _WIN32
#include <Windows.h>
#endif
#include <gl\GL.h>

// This is a class that loads TGA files into opengl textures
class TGA {
private:
	// the handle for the texture in opengl
	GLuint textureHandle;
	int num_texture = 1;
public:
	// Constructs and loads a TGA into opengl from the given image file path
	TGA(char* imagePath);

	// Returns the handle to the texture created from the image, for binding to opengl
	GLuint getTextureHandle(void);
};

#endif
