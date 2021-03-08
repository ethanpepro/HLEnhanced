#pragma once

#include <KeyValues.h>
// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>
#include <VTFLib.h>

class Material {
public:
	Material(const std::string &name, std::shared_ptr<KeyValues> &kv, GLuint texture_id);
	~Material();
	
private:
	std::string _name;
	std::shared_ptr<KeyValues> _kv;
	GLuint _texture_id;
	
private:
	Material(const Material &) = delete;
	Material &operator=(const Material &) = delete;
};

typedef std::shared_ptr<Material> Material_t;
