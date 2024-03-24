#include <GL/gl.h>
#include <stdio.h>
#include <string.h>

#define IK_NAME_LENGHT 10                 //tamanho do nome
#define IK_MAX_CHILD   10                 //nº max de filhos

void default_draw (void);               //função por defeito da class


struct vec3                    // vector em 3D
{
  float x,y,z;
  vec3(double xx, double yy, double zz)
  {
    x=xx;y=yy;z=zz;
  }
  vec3()
  {
    x=0;
    y=0;
    z=0;
  }
};

vec3 operator + (vec3 &u, vec3 &v);   //operações com vec3
vec3 operator - (vec3 &u, vec3 &v);
vec3 operator * (double c, vec3 &v);
vec3 &operator += (vec3 &u, vec3 &v);
vec3 &operator -= (vec3 &u, vec3 &v);
vec3 &operator *= (vec3 &v, double c);



//class para controlar hierarquias de objectos
class IK_OBJECT                     
{
 protected:
  vec3 rot;        // rotação em XX em YY e em ZZ
  vec3 mov;        // tranlação
 private : 
  IK_OBJECT *child[IK_MAX_CHILD];   // filhos no futuro pode ser uma lista...
  void reset(void);                 // reset 
  int n;                            // no futuro nº de filhos
  void (*draw_func)(void);          // função de desenho
 public:
  char name[IK_NAME_LENGHT];        //nome do obj
  vec3 pivot;      // centro de rotação relat pos
  vec3 pos;        // pos relativa ao pai(orig) 
  
  int FLAG_LIM_XX;    // activar-1/desactivar-0(defito) limites  //rotação
  int FLAG_LIM_YY;    // activar-1/desactivar-0(defito) limites
  int FLAG_LIM_ZZ;    // activar-1/desactivar-0(defito) limites

  int FLAG_LIM_X;    // activar-1/desactivar-0(defito) limites  //translação
  int FLAG_LIM_Y;    // activar-1/desactivar-0(defito) limites
  int FLAG_LIM_Z;    // activar-1/desactivar-0(defito) limites

  vec3 rot_max,rot_min;  //limites
  vec3 mov_max,mov_min;
  
  IK_OBJECT (void); // construtores
  IK_OBJECT (void (*f)(void) );
  IK_OBJECT (char n[IK_NAME_LENGHT]);
  IK_OBJECT (void (*f)(void),char n[IK_NAME_LENGHT]);
  
  void draw (void); //desenha ele mesmo e os filhos 
  void select_draw_func (void (*f) (void) ); // nova função de desenho
  void show_bones (double scale); //desenha "ossos" 
  void new_name (char n[IK_NAME_LENGHT]); //novo nome

  int new_child(IK_OBJECT *C);  //novo filho
  int del_child(IK_OBJECT *C);  //apaga filho
  
  int rotate(vec3 rotate);      // rodar
  int rotate_xx(float angle);   
  int rotate_yy(float angle);
  int rotate_zz(float angle);
  
  int move(vec3 vector);        //mover 
  
  ~IK_OBJECT (void);            //destructor 
   //   friend void existe_soma(NODE *nd,int soma);
   //friend NODE* make_tree (char* &t);
};

