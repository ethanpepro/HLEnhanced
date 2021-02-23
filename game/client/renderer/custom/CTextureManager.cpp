#include <memory>
#include <string.h>
#include <map>

#include "hud.h"
#include "cl_util.h"

#include "logging/Logging.h"

#include <OpenGL/gl.h>
#include <VTFLib.h>

#include "CTextureManager.h"

std::map<std::string, GLuint> textures;

void ResetMaterials() {
	for (auto& texture : textures) {
		glDeleteTextures(1, &texture.second);
	}
	
	textures.clear();
}

static void PrecacheMaterial(const std::string& material_file) {
	VTFLib::CVMTFile vmt;
	
	char material_path[MAX_PATH] = {};
	
	g_pFileSystem->GetLocalPath(material_file.c_str(), material_path, sizeof(material_path));
	
	if (!vmt.Load(material_path)) {
		logging::log->error("Bad .vmt!");
	}
	
	// TODO: Make robust parsing of all used members, also parse shader type
	const char *texture = nullptr;
	
	VTFLib::Nodes::CVMTGroupNode *root = vmt.GetRoot();
	
	for (unsigned int i = 0; i < root->GetNodeCount(); i++) {
		VTFLib::Nodes::CVMTNode *node = root->GetNode(i);
		
		if (strcmp(node->GetName(), "$basetexture") == 0) {
			texture = static_cast<VTFLib::Nodes::CVMTStringNode *>(node)->GetValue();
		}
	}
	
	VTFLib::CVTFFile vtf;
	
	std::string texture_file = std::string("materials/") + texture + std::string(".vtf");
	
	char texture_path[MAX_PATH] = {};
	
	g_pFileSystem->GetLocalPath(texture_file.c_str(), texture_path, sizeof(texture_path));
	
	if (!vtf.Load(texture_path)) {
		logging::log->error("Bad .vtf!");
	}
	
	unsigned int width = vtf.GetWidth();
	unsigned int height = vtf.GetHeight();
	
	// TODO: Image formats
	uint8_t *data = new uint8_t[vtf.ComputeImageSize(width, height, 1, IMAGE_FORMAT_RGBA8888)];
	
	if (!vtf.ConvertToRGBA8888(vtf.GetData(0, 0, 0, 0), data, width, height, vtf.GetFormat())) {
		logging::log->error("Bad conversion!");
	}
	
	GLint old_handle = 0;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &old_handle);
	
	GLuint handle = 0;
	glGenTextures(1, &handle);
	
	glBindTexture(GL_TEXTURE_2D, handle);
	
	// TODO: Image formats
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	
	glBindTexture(GL_TEXTURE_2D, old_handle);
	
	textures.emplace(material_file, handle);
	
	logging::log->debug("Adding {} ({})", material_file, handle);
}

// The rationale for calling this in MapInit is to (possibly) transfer materials in a map when connecting to a server, or
// to refresh and load materials only needed in that map
void PrecacheMaterials() {
	ResetMaterials();
	
	// TODO: Iterate over .vmt files in materials/ (dev-only) and parse needed extra textures in maps/ (regular, work on format)
	PrecacheMaterial("materials/orange.vmt");
}
