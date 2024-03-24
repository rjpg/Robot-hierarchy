/*
	Copyright by Thomas Baier (thomas.baier@stmuc.com)
	Created by OpenGL Export Plugin 1.0 at Sat Dec 13 19:20:17 2003
	OpenGL-Structure 
*/

extern float WorldBoundMin[3];
extern float WorldBoundMax[3];

#define SCENE_NAME "OPENGL.3DS"
/* BackGround Definitions */
extern float background[3];
/* Light Definitions */
typedef struct
{
	float		color[3];
	float		position[3];
	int			isSpot;
	float		direction[3];
	float		radius;
	float		falloff;
}Light_Def;
#define MAX_LIGHTS (3)
extern Light_Def lights[MAX_LIGHTS];
/* Material Definitions */
typedef struct
{
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float emission[3];
	float alpha;
	int   texture;
}Material_Def;
#define MAX_MATERIALS (1)
extern Material_Def materials[MAX_MATERIALS];
/* Texture Map Definitions */
typedef struct
{
	char *name;
	int  id;
}Map_Def;
#define MAX_TEXTUREMAPS (0)
extern Map_Def texture_maps[1];
/* Object Definitions */
typedef struct 
{
	char  *obj_name;
	int   *face_indicies;
	int   face_number;
	float *verticies;
	int   vertex_number;
	float *normals;
	int   normal_number;
	float *textures;
	int   texture_number;
	int   mat_ref;
}Object_Def;
#define MAX_OBJECTS (10)
extern Object_Def objects[MAX_OBJECTS];
