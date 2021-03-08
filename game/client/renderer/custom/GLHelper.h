#pragma once

// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>
#include <VTFLib.h>

void _check_gl_error(const char *file, const char *function, int line);

#define check_gl_error() _check_gl_error(__FILE__, __FUNCTION__, __LINE__)

GLuint UploadVTF(std::unique_ptr<VTFLib::CVTFFile> &vtf);
