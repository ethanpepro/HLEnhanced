#include "hud.h"
#include "cl_util.h"

#include "pm_shared.h"
#include "pm_defs.h"
#include "com_model.h"

#include "logging/CLogSystem.h"

// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>

#include "GLHelper.h"
#include "Logging.h"
#include "MaterialRegistry.h"

#include "Renderer.h"

cl_entity_t *world = nullptr;

void Renderer_Initialize() {
	renderer::log = logging::LogSystem().CreateMultiLogger("renderer");
	
	material::log = logging::LogSystem().CreateMultiLogger("material");
	
	// Clear all errors before we start
	while (glGetError() != GL_NO_ERROR);
}

void Renderer_Shutdown() {
	if (renderer::log) {
		logging::LogSystem().DropLogger(renderer::log);
		renderer::log.reset();
	}
	
	if (material::log) {
		logging::LogSystem().DropLogger(material::log);
		material::log.reset();
	}
}

void Renderer_MapInit(cl_entity_t *entity) {
	world = entity;
}

void Renderer_NewMapStarted(const char *map_name) {
	GetMaterialRegistry().UnregisterAllMaterials();
	
	material::log->debug("Processing materials for {}", map_name);
	
	GetMaterialRegistry().RegisterMaterial("Brick/brickfloor001a");
	GetMaterialRegistry().RegisterMaterial("Brick/brickfloor002a");
	GetMaterialRegistry().RegisterMaterial("Brick/brickwall001a");
}

void Renderer_V_CalcRefdef(ref_params_t *params) {
	
}

class PreserveEngineState {
public:
	PreserveEngineState() {
		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glPushMatrix();
	}
	
	~PreserveEngineState() {
		glPopMatrix();
		glPopAttrib();
	}
};

void Renderer_HUD_DrawNormalTriangles() {
	PreserveEngineState state;
}
