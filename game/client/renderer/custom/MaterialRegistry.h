#pragma once

#include <unordered_map>

#include "Material.h"

class MaterialRegistry {
private:
	typedef std::unordered_map<std::string, Material_t> Materials_t;
	
public:
	MaterialRegistry() = default;
	~MaterialRegistry() = default;
	
public:
	Material_t RegisterMaterial(const std::string &name);
	Material_t FindMaterial(const std::string &name);
	void UnregisterMaterial(const std::string &name);
	void UnregisterAllMaterials();
	
private:
	// Materials_t _persistent;
	Materials_t _materials;
	
private:
	MaterialRegistry(const MaterialRegistry &) = delete;
	MaterialRegistry &operator=(const MaterialRegistry &) = delete;
};

MaterialRegistry &GetMaterialRegistry();
