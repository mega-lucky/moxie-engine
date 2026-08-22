#ifndef MESH_H
#define MESH_H

#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif

void xinit_mesh(mesh_shape *data, const void *vert_data, size_t nverts, const unsigned int *ind_data, size_t ninds, const attrib_layout *layout);
void init_mesh(mesh_shape *data, const mesh_vertex *vert_data, size_t nverts, const unsigned int *ind_data, size_t ninds);
void upload_mesh(const mesh_shape *mesh);
void destroy_mesh(mesh_shape *data);

#ifdef __cplusplus
}
#endif

#endif