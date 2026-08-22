#include "./types.h"

#ifdef __cplusplus
extern "C" {
#endif

void init_shader(shader_data *data, const char *vert_src, const char *frag_src);
void init_shader_from_file(shader_data *data, const char *vert_path, const char *frag_path);

#ifdef __cplusplus
}
#endif