#include "objik.h"


vec3 operator + (vec3 &u, vec3 &v)
	{
	return vec3 (u.x + v.x, u.y + v.y, u.z + v.z);
	}

vec3 operator - (vec3 &u, vec3 &v)
	{
	return vec3 (u.x - v.x, u.y - v.y, u.z - v.z);
	}

vec3 operator * (double c, vec3 &v)
	{
	return vec3 (c*v.x, c*v.y, c*v.z);
	}

vec3 &operator += (vec3 &u, vec3 &v)
	{
	u.x += v.x; u.y +=v.y; u.z+=v.z;
	return u;
	}

vec3 &operator-=(vec3 &u, vec3 &v)
	{
	u.x -= v.x; u.y -=v.y; u.z-=v.z;
	return u;
	}


vec3 &operator*=(vec3 &v, double c)
	{
	v.x *= c; v.y *= c; v.z*= c;
	return v;

	}

void  default_draw (void)
{
  //  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  //  glColor3f (1.0,1.0,1.0);
  glVertex3f(1.0,0.0,0.0);
  glVertex3f(0,0,0);
  glVertex3f(0.0,1.0,0.0);
  glVertex3f(0,0,0);
  glVertex3f(0.0,0.0,1.0);
  glVertex3f(0,0,0);
  glEnd();
  //  glEnable(GL_LIGHTING);  
/*  glBegin(GL_POINTS);
  glVertex3f(0.0,0.0,0.0);
  glEnd();

  printf("rui\n");*/
  
}

void IK_OBJECT::reset(void)
{

  FLAG_LIM_XX=0; 
  FLAG_LIM_YY=0; 
  FLAG_LIM_ZZ=0;

  FLAG_LIM_X=0;  
  FLAG_LIM_Y=0;  
  FLAG_LIM_Z=0;  

  name[0]='\0';
  for(int i=0;i<IK_MAX_CHILD;i++)
    child[i]=NULL;
  draw_func=default_draw; 

}

IK_OBJECT::IK_OBJECT(void)
{
  reset();
}

IK_OBJECT::IK_OBJECT(char n[IK_NAME_LENGHT])
{
  reset();
  strcpy(name,n);
}

IK_OBJECT::IK_OBJECT(void (*f)(void))
{
  reset();
  draw_func=f;
}

IK_OBJECT::IK_OBJECT(void (*f)(void),char n[IK_NAME_LENGHT])
{
  reset();
  draw_func=f;
  strcpy(name,n);
}

void IK_OBJECT::draw(void)
{
  //vec3 npos=pivot*-1;
  glPushMatrix();
  glTranslatef (pos.x, pos.y, pos.z);
  glTranslatef (pivot.x, pivot.y, pivot.z);
  glRotatef ((GLfloat) rot.x, 1.0, 0.0, 0.0);
  glRotatef ((GLfloat) rot.y, 0.0, 1.0, 0.0);
  glRotatef ((GLfloat) rot.z, 0.0, 0.0, 1.0);
  glTranslatef (-pivot.x, -pivot.y, -pivot.z);
  draw_func();
  for (int i=0;i<IK_MAX_CHILD;i++)
    if (child[i]!=NULL)
      child[i]->draw();
  glPopMatrix();
}

void IK_OBJECT::show_bones(double scale)
{
  vec3 aux(0.0,0.0,0.0);
  
  glPushMatrix();
  glTranslatef (pos.x, pos.y, pos.z);
  glTranslatef (pivot.x, pivot.y, pivot.z);
  glRotatef ((GLfloat) rot.x, 1.0, 0.0, 0.0);
  glRotatef ((GLfloat) rot.y, 0.0, 1.0, 0.0);
  glRotatef ((GLfloat) rot.z, 0.0, 0.0, 1.0);
  glScaled( scale,scale,scale);
  default_draw();
  glScaled( 1/scale,1/scale,1/scale);
  glTranslatef (-pivot.x, -pivot.y, -pivot.z);
  
  for (int i=0;i<IK_MAX_CHILD;i++)
    if (child[i]!=NULL)
      {
	//glTranslatef (pivot.x, pivot.y, pivot.z);
	//   glBegin(GL_LINES);
// 	  glVertex3f(0,0,0);
// 	  aux=child[i]->pos+child[i]->pivot;
// 	  aux-=pos;
// 	    //+child[i]->mov;
// 	  glVertex3f(aux.x,aux.y,aux.z);
// 	  glEnd();
// 	  //glTranslatef (-pivot.x, -pivot.y, -pivot.z);
	  child[i]->show_bones(scale);

      }

  glPopMatrix();
}
void IK_OBJECT::select_draw_func(void (*f)(void))
{
  draw_func=f;
}

void IK_OBJECT::new_name(char n[IK_NAME_LENGHT])
{
  strcpy(name,n);
}

int IK_OBJECT::new_child(IK_OBJECT *C)
{
  for(int i=0;i<IK_MAX_CHILD;i++)
    if(child[i]==NULL) 
      {
	child[i]=C;
	return 0;
      }
      
  return -1;

}

int IK_OBJECT::del_child(IK_OBJECT *C)
{
  for(int i=0;i<IK_MAX_CHILD;i++)
    if(child[i]==C) 
      {
	child[i]=NULL;
	return 0;
      }
      
  return -1;

}


int IK_OBJECT::rotate(vec3 rotate)
{
  int x=0;
  if(FLAG_LIM_XX==0)
    rot.x+=rotate.x;
  else
    if (rot.x+rotate.x>rot_max.x)
      {rot.x=rot_max.x;x=-1;}
    else if (rot.x+rotate.x<rot_min.x)
      {rot.x=rot_min.x;x=-1;}
    else
      rot.x+=rotate.x;
  
  if(FLAG_LIM_YY==0)
    rot.y+=rotate.y;
  else 
    if (rot.y+rotate.y>rot_max.y)
      {rot.y=rot_max.y;x=-1;}
    else if (rot.y+rotate.y<rot_min.y)
      {rot.y=rot_min.y;x=-1;}
    else
      rot.y+=rotate.y;
  
  if(FLAG_LIM_ZZ==0)
    rot.z+=rotate.z;
  else  
    if (rot.z+rotate.z>rot_max.z)
      {rot.z=rot_max.z;x=-1;}
    else if (rot.z+rotate.z<rot_min.z)
      {rot.z=rot_min.z;x=-1;}
    else
      rot.z+=rotate.z;
  
  return x;
}

int IK_OBJECT::rotate_xx(float angle)
{
  return rotate(vec3(angle,0.0,0.0));
}

int IK_OBJECT::rotate_yy(float angle)
{
  return rotate(vec3(0.0,angle,0.0));
}

int IK_OBJECT::rotate_zz(float angle)
{
  return rotate(vec3(0.0,0.0,angle));
}

int IK_OBJECT::move(vec3 vec)
{
  int x=0;
   if(FLAG_LIM_X==0)
     mov.x+=vec.x;
   else 
     if (mov.x+vec.x>mov_max.x)
       {mov.x=mov_max.x;x=-1;}
     else if (mov.x+vec.x<mov_min.x)
       {mov.x=mov_min.x;x=-1;}
     else
       mov.x+=vec.x;

   if(FLAG_LIM_Y==0)
     mov.y+=vec.y;
   else 
     if (mov.y+vec.y>mov_max.y)
       {mov.y=mov_max.y;x=-1;}
     else if (mov.y+vec.y<mov_min.y)
       {mov.y=mov_min.y;x=-1;}
     else
       mov.y+=vec.y;
   
   if(FLAG_LIM_Z==0)
     mov.z+=vec.z;
   else 
     if (mov.z+vec.z>mov_max.z)
       {mov.z=mov_max.z;x=-1;}
     else if (mov.z+vec.z<mov_min.z)
       {mov.z=mov_min.z;x=-1;}
     else
       mov.z+=vec.z;
   
   return x;
}

IK_OBJECT::~IK_OBJECT(void)
{
  
}
