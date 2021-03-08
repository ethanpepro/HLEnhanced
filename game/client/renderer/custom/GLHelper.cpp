#include <memory>

#include "Logging.h"

// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>
#include <VTFLib.h>

#include "GLHelper.h"

void _check_gl_error(const char *file, const char *function, int line) {
	GLenum error = glGetError();
	
	while (error != GL_NO_ERROR) {
		renderer::log->error("{}:{}:{}: OpenGL error: {}", file, function, line, error);
		
		error = glGetError();
	}
}

GLuint UploadVTF(std::unique_ptr<VTFLib::CVTFFile> &vtf) {
	GLuint texture = 0;
	
	glGenTextures(1, &texture);
	
	check_gl_error();
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	check_gl_error();
	
	size_t width = vtf->GetWidth();
	
	size_t height = vtf->GetHeight();
	
	// TODO: Image formats, frames, mipmaps, transparency, etc.
	size_t size = vtf->ComputeImageSize(width, height, 1, IMAGE_FORMAT_RGBA8888);
	
	std::unique_ptr<uint8_t []> image = std::make_unique<uint8_t []>(size);
	
	vtf->ConvertToRGBA8888(vtf->GetData(0, 0, 0, 0), image.get(), width, height, vtf->GetFormat());
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.get());
	
	check_gl_error();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	check_gl_error();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	check_gl_error();
	
	return texture;
}
