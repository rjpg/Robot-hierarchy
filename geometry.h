#include <stdlib.h>
#include <math.h>
#include <GL/gl.h>
#include "scene.h"

void draw_torus(float meridian_radius, float core_radius,
				int meridian_slices, int core_slices);

void draw_sphere(float radius, int slices, int stacks);

void draw_plane(float w, float h, int rows, int cols);

void draw_object(int k);

void draw_wire_grid(int rows, int cols);

void draw_grid(int rows, int cols,
			float sx, float sy, float sz,
			float ux, float uy, float uz,
			float vx, float vy, float vz);


