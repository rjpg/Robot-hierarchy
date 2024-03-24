#include "geometry.h"

#ifndef M_PI
#define M_PI 3.1415927f
#endif


void draw_torus(float meridian_radius, float core_radius, 
				int meridian_slices, int core_slices)
{
	int J = meridian_slices;
	int I = core_slices;
	for(int j = 0; j < J-1; j++)
	{
 		float v = j/(J-1.0f);
		float rho = v * 2.0f * M_PI;
		float v2 = (j+1)/(J-1.0f);
		float rho2 = v2 * 2.0f * M_PI;
		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < I; i++)
		{	
			float u = i/(I-1.0f);
			float theta = u * 2.0f * M_PI;
			float x,y,z,nx,ny,nz;

			x = float(core_radius*cos(theta) + meridian_radius*cos(theta)*cos(rho));
			y = float(core_radius*sin(theta) + meridian_radius*sin(theta)*cos(rho));
			z = float(meridian_radius*sin(rho));
			nx = float(cos(theta)*cos(rho));
			ny = float(sin(theta)*cos(rho));
			nz = float(sin(rho));	

			glColor3f(u, v, 0.0);
			glNormal3f(nx, ny, nz);
			glVertex3f( x,  y,  z);

			x = float(core_radius*cos(theta) + meridian_radius*cos(theta)*cos(rho2));
			y = float(core_radius*sin(theta) + meridian_radius*sin(theta)*cos(rho2));
			z = float(meridian_radius*sin(rho2));
			nx = float(cos(theta)*cos(rho2));
			ny = float(sin(theta)*cos(rho2));
			nz = float(sin(rho2));	

			glColor3f(u, v, 0.0);
			glNormal3f(nx, ny, nz);
			glVertex3f( x,  y,  z);
		}
		glEnd();
	}
}


void draw_sphere(float radius, int slices, int stacks)
{
	int J = stacks;
	int I = slices;
	for(int j = 0; j < J; j++)
	{
 		float v = j/(float) J;
		float phi = v * 2 * M_PI;
		float v2 = (j+1)/(float) J;
		float phi2 = v2 * 2 * M_PI;

		glBegin(GL_QUAD_STRIP);
		for(int i = 0; i < I; i++)
		{	
			float u = i/(I-1.0f);
			float theta = u * M_PI;
			float x,y,z,nx,ny,nz;

			nx = float(cos(theta)*cos(phi));
			ny = float(sin(theta)*cos(phi));
			nz = float(sin(phi));	
			x = radius * nx;
			y = radius * ny;
			z = radius * nz;

			glColor3f(u, v, 0.0);
			glNormal3f(nx, ny, nz);
			glVertex3f( x,  y,  z);

			nx = float(cos(theta)*cos(phi2));
			ny = float(sin(theta)*cos(phi2));
			nz = float(sin(phi2));	
			x = radius * nx;
			y = radius * ny;
			z = radius * nz;

			glColor3f(u, v+(1.0/(J-1.0f)), 0.0);
			glNormal3f(nx, ny, nz);
			glVertex3f( x,  y,  z);
		}
		glEnd();
	}
}


// draw square subdivided into quad strips
void draw_plane(float w, float h, int rows, int cols)
{
	int x, y;
	GLfloat vx, vy, s, t;
	GLfloat ts, tt, tw, th;

	ts = 1.0 / cols;
	tt = 1.0 / rows;

	tw = w / cols;
	th = h / rows;

	glNormal3f(0.0, 0.0, 1.0);

	for(y=0; y<rows; y++) {
		glBegin(GL_QUAD_STRIP);
		for(x=0; x<=cols; x++) {
			vx = tw * x -(w/2.0);
			vy = th * y -(h/2.0);
			s = ts * x;
			t = tt * y;

//			glTexCoord2f(s, t);
			glColor3f(s, t, 0.0);
			glVertex3f(vx, vy, 0.0);

			glColor3f(s, t + tt, 0.0);
//			glTexCoord2f(s, t + tt);
			glVertex3f(vx, vy + th, 0.0);
		}
		glEnd();
	}

}


// Draw a regular grid of lines in X/Y plane, unit size
void draw_wire_grid(int rows, int cols)
{
	int i, j;

	glNormal3f(0.0, 0.0, 1.0);

	// draw horizontals
	for(i=0; i<=rows; i++) {
		glBegin(GL_LINE_STRIP);
		for(j=0; j<=cols; j++) {
			glVertex3f((float) j / cols - 0.5, (float) i / rows - 0.5, 0.0);
		}
		glEnd();
	}

	// draw verticals
	for(i=0; i<=cols; i++) {
		glBegin(GL_LINE_STRIP);
		for(j=0; j<=rows; j++) {
			glVertex3f((float) i / cols - 0.5, (float) j / rows - 0.5, 0.0);
		}
		glEnd();
	}

}


void draw_grid(int rows, int cols,
			float sx, float sy, float sz,
			float ux, float uy, float uz,
			float vx, float vy, float vz)
{
	int x, y;

	//srand(0);
	for(y=0; y<rows; y++) {
		glBegin(GL_QUAD_STRIP);
		for(x=0; x<=cols; x++) {
			float u = x / (float) cols;
			float v = y / (float) rows;
			float v2 = v + (1.0 / (float) rows);

            glTexCoord2f(u, v);
			glVertex3f(sx + (u*ux) + (v*vx), sy + (u*uy) + (v*vy), sz + (u*uz) + (v*vz));

            glTexCoord2f(u, v2);
			glVertex3f(sx + (u*ux) + (v2*vx), sy + (u*uy) + (v2*vy), sz + (u*uz) + (v2*vz));
		}
		glEnd();
	}

}


/*float frand(void)
{
	return (float) rand() / RAND_MAX;
}*/


// draw object in scene.c
void draw_object(int k)
{
  int i, j;
  //int k;
  //srand(0);
  
  //for(k = 0; k < MAX_OBJECTS;k++) 
  //  {
    int *faces = objects[k].face_indicies;
    float *v = objects[k].verticies;
    float *n = objects[k].normals;
    float *t = objects[k].textures;
    
    glBegin (GL_TRIANGLES);

    
    for(i = 0;i < objects[k].face_number;i++) {
      for(j = 0;j < 3;j++) {
	int vi = faces[i * 9 + j + 0];
	
	//glColor3f(frand(), frand(), frand());
	
	if(n) {
	  int ni = faces[i * 9 + j + 3];
	  glNormal3f  (-n[ni * 3 + 0],-n[ni * 3 + 1], -n[ni * 3 + 2]);
	}
	
	if(t) {
	  //                  glTexCoord2f(t[ti * 3 + 0], t[ti * 3 + 1]);
	}
	
	glVertex3f  (v[vi * 3 + 0] * 0.1, v[vi * 3 + 1] * 0.1, v[vi * 3 + 2] * 0.1);
	//				glVertex3f  (v[vi * 3 + 0], v[vi * 3 + 1], v[vi * 3 + 2]);
	// scalecabrão  tirar depois e dimencionar o 3ds
      }
    }
    glEnd ();
    //}
  
}



