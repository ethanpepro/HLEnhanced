#pragma once

#include "hud.h"
#include "cl_util.h"

void GenerateVertexDataForMap(cl_entity_t *world_entity);

void Renderer_V_CalcRefdef(ref_params_t *params);

void Renderer_HUD_DrawNormalTriangles();
