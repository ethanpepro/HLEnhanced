#include <memory>
#include <string.h>
#include <map>

#include "hud.h"
#include "cl_util.h"

#include "logging/Logging.h"

// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>
#include <VTFLib.h>

#include "TextureManager.h"

std::map<std::string, GLuint> textures;

void ResetMaterials() {
	for (auto &texture : textures) {
		glDeleteTextures(1, &texture.second);
		texture.second = 0;
	}
	
	textures.clear();
}

// TODO: Sort into engine-necessary textures (that are loaded once) and
// then get map-specific textures on map load (once name is known)

// TODO: See how Source does this, this can't be remotely right

// TODO: Error handling
static void PrecacheMaterial(const std::string &material_file) {
	// TODO: Redundancy
	
	VTFLib::CVMTFile vmt;
	
	char material_path[MAX_PATH] = {};
	
	g_pFileSystem->GetLocalPath(material_file.c_str(), material_path, sizeof(material_path));
	
	if (!vmt.Load(material_path)) {
		logging::log->error("Bad .vmt! {}", vlGetLastError());
	}
	
	// TODO: Make robust parsing of all used members, also parse shader type
	const char *texture = nullptr;
	
	VTFLib::Nodes::CVMTGroupNode *root = vmt.GetRoot();
	
#if 0
	logging::log->info("{} uses {}", material_file, root->GetName());
#endif
	
	for (unsigned int i = 0; i < root->GetNodeCount(); i++) {
		VTFLib::Nodes::CVMTNode *node = root->GetNode(i);
		const char *name = node->GetName();
		
		switch (node->GetType()) {
			case NODE_TYPE_STRING:
				if (strcmp(name, "$basetexture") == 0) {
					texture = static_cast<VTFLib::Nodes::CVMTStringNode *>(node)->GetValue();
				}
				break;
			default:
				break;
		}
	}
	
	VTFLib::CVTFFile vtf;
	
	std::string texture_file = std::string("materials/") + texture + std::string(".vtf");
	
	char texture_path[MAX_PATH] = {};
	
	g_pFileSystem->GetLocalPath(texture_file.c_str(), texture_path, sizeof(texture_path));
	
	if (!vtf.Load(texture_path)) {
		logging::log->error("Bad .vtf! {}", vlGetLastError());
	}
	
	unsigned int width = vtf.GetWidth();
	unsigned int height = vtf.GetHeight();
	
	// TODO: Image formats
	unsigned int data_size = vtf.ComputeImageSize(width, height, 1, IMAGE_FORMAT_RGBA8888);
	uint8_t *data = new uint8_t[data_size];
	
	if (!vtf.ConvertToRGBA8888(vtf.GetData(0, 0, 0, 0), data, width, height, vtf.GetFormat())) {
		logging::log->error("Bad conversion! {}", vlGetLastError());
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
	
	delete[] data;
	
	glBindTexture(GL_TEXTURE_2D, old_handle);
	
	textures.emplace(material_file, handle);
	
	logging::log->info("Adding {} ({})", material_file, handle);
}

// The rationale for calling this in MapInit is to (possibly) transfer materials in a map when connecting to a server, or
// to refresh and load materials only needed in that map
void PrecacheMaterialsForMap(const char *map_name) {
	ResetMaterials();
	
	logging::log->info("Processing materials for {}", map_name);
	
	// TODO: Iterate over .vmt files in materials/ (dev-only) and parse needed extra textures in maps/ (regular, work on format)
	PrecacheMaterial("materials/Brick/brickfloor001a.vmt");
	PrecacheMaterial("materials/Brick/brickfloor002a.vmt");
	PrecacheMaterial("materials/Brick/brickwall001a.vmt");
	PrecacheMaterial("materials/orange.vmt");
}
