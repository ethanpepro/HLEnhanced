#include <KeyValues.h>
// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>
#include <VTFLib.h>

#include "GLHelper.h"
#include "Logging.h"

#include "Material.h"

Material::Material(const std::string &name, std::shared_ptr<KeyValues> &kv, GLuint texture_id) : _name(name), _kv(kv), _texture_id(texture_id) {
	
}

Material::~Material() {
	if (_texture_id) {
		glDeleteTextures(1, &_texture_id);
		_texture_id = 0;
	}
}
