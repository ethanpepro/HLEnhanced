#pragma once

#include "hud.h"
#include "cl_util.h"

extern cl_entity_t *world;

// TODO: Renderer_ in a class?

void Renderer_Initialize();

void Renderer_Shutdown();

void Renderer_MapInit(cl_entity_t *entity);

void Renderer_NewMapStarted(const char *map_name);

void Renderer_V_CalcRefdef(ref_params_t *params);

void Renderer_HUD_DrawNormalTriangles();
