// TODO: Just gonna throw this here as of right now, _restart is broken, don't think it's because of any of this

#include "hud.h"
#include "cl_util.h"

#include "pm_shared.h"
#include "pm_defs.h"
#include "com_model.h"

#include "logging/Logging.h"

// TODO: Sort OpenGL headers by platform
#include <OpenGL/gl.h>

#include "TextureManager.h"

#include "Renderer.h"

// TODO: Helper class to store variables

// TODO: Move to separate file (for Quake-specific, modified re-implementations)

// TODO: Move to an OpenGL helper file

void _check_gl_error(const char *file, const char *function, int line) {
	GLenum error = glGetError();
	
	while (error != GL_NO_ERROR) {
		logging::log->error("{}:{}:{}: OpenGL error: {}", file, function, line, error);
		
		error = glGetError();
	}
}

#define check_gl_error() _check_gl_error(__FILE__, __FUNCTION__, __LINE__)

// TODO: Move

Vector saved_mins;
Vector saved_maxs;

void DisableWorldDrawing(ref_params_t *params) {
	Vector world_coords;
	
	model_t *world = gEngfuncs.GetEntityByIndex(0)->model;
	
	AngleVectors(params->viewangles, params->forward, params->right, params->up);
	VectorMA(params->vieworg, -1, params->forward, world_coords);
	
	saved_mins = world->nodes[0].mins;
	world->nodes[0].mins = world_coords;
	
	saved_maxs = world->nodes[0].maxs;
	world->nodes[0].maxs = world_coords;
}

void RestoreWorldDrawing() {
	model_t *world = gEngfuncs.GetEntityByIndex(0)->model;
	
	world->nodes[0].mins = saved_mins;
	world->nodes[0].maxs = saved_maxs;
}

// TODO: Move

struct BrushVertex {
	Vector pos;
	float texture_coord[2];
	float lightmaptexture_coord[2];
};

struct BrushFace {
	int start_vertex;
	Vector normal;
	Vector tangent_s;
	Vector tangent_t;
};

GLuint world_vbo = 0;

BrushVertex *vertex_data = nullptr;
BrushFace *face_data = nullptr;

static void FreeVertexData() {
	if (world_vbo) {
		glDeleteBuffers(1, &world_vbo);
		world_vbo = 0;
	}
	
	check_gl_error();
	
	if (vertex_data) {
		delete[] vertex_data;
		vertex_data = nullptr;
	}
	
	if (face_data) {
		delete[] face_data;
		face_data = nullptr;
	}
}

void GenerateVertexDataForMap(cl_entity_t *entity) {
	FreeVertexData();
	
	size_t total_faces = 0;
	size_t total_vertices = 0;
	
	model_t *world = entity->model;
	msurface_t *surfaces = world->surfaces;
	
	for (size_t i = 0; i < world->numsurfaces; i++) {
		msurface_t surface = surfaces[i];
		if (surface.flags & (SURF_DRAWSKY | SURF_DRAWTURB)) {
			continue;
		}
		
		glpoly_t *poly = surface.polys;
		if (poly->numverts > 0) {
			total_faces++;
			total_vertices += poly->numverts;
		}
	}
	
	// TODO: Associate surfaces->texinfo->texture (or material replacement) with an active shader instance
	
	logging::log->info("Total visible faces: {}", total_faces);
	logging::log->info("Total visible vertices: {}", total_vertices);
	
	vertex_data = new BrushVertex[total_vertices];
	face_data = new BrushFace[total_faces];
	
	size_t current_vertex = 0;
	size_t current_face = 0;
	
	for (size_t i = 0; i < world->numsurfaces; i++) {
		msurface_t surface = surfaces[i];
		if (surface.flags & (SURF_DRAWSKY | SURF_DRAWTURB)) {
			continue;
		}
		
		glpoly_t *poly = surface.polys;
		if (poly->numverts > 0) {
			float *vertex = poly->verts[0];
			
			int packed = (current_face << 16);
			packed |= (poly->flags & 0xffff);
			poly->flags = packed;
			
			BrushFace *extra = &face_data[current_face];
			
			extra->start_vertex = current_vertex;
			
			extra->tangent_s = reinterpret_cast<Vector *>(&surface.texinfo->vecs)[0];
			VectorNormalize(extra->tangent_s);
			
			extra->tangent_t = reinterpret_cast<Vector *>(&surface.texinfo->vecs)[1];
			VectorNormalize(extra->tangent_t);
			
			extra->normal = surface.plane->normal;
			if (surface.flags & SURF_PLANEBACK) {
				extra->normal = -extra->normal;
			}
			
			for (size_t j = 0; j < poly->numverts; j++, vertex += VERTEXSIZE) {
#if 0
				vertex_data[current_vertex].pos[0] = vertex[0];
				vertex_data[current_vertex].pos[1] = vertex[1];
				vertex_data[current_vertex].pos[2] = vertex[2];
#else
				vertex_data[current_vertex].pos = reinterpret_cast<Vector *>(&vertex)[0];
#endif
				
				vertex_data[current_vertex].texture_coord[0] = vertex[3];
				vertex_data[current_vertex].texture_coord[1] = vertex[4];
				
				vertex_data[current_vertex].lightmaptexture_coord[0] = vertex[5];
				vertex_data[current_vertex].lightmaptexture_coord[1] = vertex[6];
				
				current_vertex++;
			}
			
			current_face++;
		}
	}
	
	// TODO: This data now exists on the GPU, right? Why keep vertex_data?
	glGenBuffers(1, &world_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, world_vbo);
	
	glBufferData(GL_ARRAY_BUFFER, total_vertices * sizeof(BrushVertex), vertex_data, GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	check_gl_error();
	
	logging::log->info("Finished generating extra world data");
}

void EnableVertexArray() {
	if (!world_vbo || !vertex_data || !face_data) {
		logging::log->error("No extra world data!");
		return;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, world_vbo);
	
	glVertexPointer(3, GL_FLOAT, sizeof(BrushVertex), (void *)offsetof(BrushVertex, pos));
	
	check_gl_error();
}

void DisableVertexArray() {
	if (!world_vbo || !vertex_data || !face_data) {
		logging::log->error("No extra world data!");
		return;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	
	check_gl_error();
}

void DrawPolyFromArray(glpoly_t *poly) {
	int index = (poly->flags >> 16) & 0xffff;
	BrushFace *extra = &face_data[index];
	glDrawArrays(GL_TRIANGLE_FAN, extra->start_vertex, poly->numverts);
}

void DrawPolyFromArray(msurface_t *surface) {
	DrawPolyFromArray(surface->polys);
}

// TODO: Move

// TODO: Move these to an API

bool renderer_prepared_for_this_frame = false;
bool new_frame = false;
size_t frame_count = 0;

void RenderWorld() {
	if (!renderer_prepared_for_this_frame) {
		return;
	}
	
	// Would this work?
	// TODO: Iterate over all entities wth GetEntityByIndex(n) for all (renderable) entities and render here, world goes first, of course
	
	RestoreWorldDrawing();
	
	renderer_prepared_for_this_frame = false;
}

void PrepareRenderer(ref_params_t *params) {
	if (params->onlyClientDraw) {
		return;
	}
	
	DisableWorldDrawing(params);
	
	renderer_prepared_for_this_frame = true;
}

// TODO: This is all that should be in Renderer.cpp

void Renderer_V_CalcRefdef(ref_params_t *params) {
	new_frame = true;
	frame_count++;
	
	PrepareRenderer(params);
}

void Renderer_HUD_DrawNormalTriangles() {
	if (!new_frame) {
		return;
	}
	
	RenderWorld();
	
	new_frame = true;
}
