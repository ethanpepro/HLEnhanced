#include <unordered_map>
#include <vector>

#include "hud.h"
#include "cl_util.h"

#include <OpenGL/gl.h>
#include <VTFLib.h>
#include <KeyValues.h>

#include "GLHelper.h"
#include "Logging.h"
#include "Material.h"

#include "MaterialRegistry.h"

MaterialRegistry &GetMaterialRegistry() {
	static MaterialRegistry g_MaterialRegistry;
	
	return g_MaterialRegistry;
}

Material_t MaterialRegistry::RegisterMaterial(const std::string &name) {
	if (_materials.find(name) != _materials.end()) {
		material::log->error("Attempted to re-register material {}!", name);
		return nullptr;
	}
	
	char buffer[MAX_PATH] = {};
	
	snprintf(buffer, sizeof(buffer), "materials/%s.vmt", name.c_str());
	
	std::shared_ptr<KeyValues> kv(new KeyValues("vmt"), KeyValuesDeleter());
	
	if (!kv->LoadFromFile(g_pFileSystem, buffer, "GAME")) {
		material::log->error("Unable to load .vmt for material {}!", name);
		return nullptr;
	}
	
	std::string basetexture = kv->GetString("$basetexture", nullptr);
	
	snprintf(buffer, sizeof(buffer), "materials/%s.vtf", basetexture.c_str());
	
	char path[MAX_PATH] = {};
	
	g_pFileSystem->GetLocalPath(buffer, path, sizeof(path));
	
	std::unique_ptr<VTFLib::CVTFFile> vtf = std::make_unique<VTFLib::CVTFFile>();
	
	if (!vtf->Load(path)) {
		material::log->error("Unable to load .vtf for material {}! {}", name, vlGetLastError());
		return nullptr;
	}
	
	// TODO: Image formats, frames, mipmaps, transparency, etc.
	GLuint texture_id = UploadVTF(vtf);
	
	if (!texture_id) {
		material::log->error("A problem occured when uploading texture data for material {}!", name);
		return nullptr;
	}
	
	Material_t material = std::make_shared<Material>(name, kv, texture_id);
	
	_materials[name] = material;
	
	material::log->debug("Registered material {}", name);
	
	return material;
}

Material_t MaterialRegistry::FindMaterial(const std::string &name) {
	auto iterator = _materials.find(name);
	
	if (iterator != _materials.end()) {
		return iterator->second;
	} else {
		return nullptr;
	}
}

void MaterialRegistry::UnregisterMaterial(const std::string &name) {
	_materials.erase(name);
}

void MaterialRegistry::UnregisterAllMaterials() {
	_materials.clear();
}
