#include <GL/glut.h>  //OpenGl/Utils      

#include "objik.h"    //objetos/hierarquias  IK_OBJECT
#include "geometry.h" //fun��es de desenho:draw_object(int) (secene.h...)


// Ponto de vista
static double  Vang=45.00,Hang=45.0,Zoom=3.0;

// vars de estado
static int xant,yant,Mouse,flag_mode=0,edit_obj=0,show_bones=0;

////////// Robot /////////////
#define ROBOT_NP (10)   //n� de pe�as do robot

#define Base      0  //uma referencia para montar o robo
#define Chest     1  
#define Neck      2
#define Head      3
#define LftUpArm  4
#define RtUpArm   5
#define LftLowArm 6
#define RtLowArm  7
#define RtHand    8
#define LftHand   9

IK_OBJECT ROBOT[ROBOT_NP];  // Robo 10 Objectos !!

void glrBase(),glrBase_edit();      //fun��es que desenham as partes do robo
void glrChest(),glrChest_edit();    //desenha modo_normal/modo_edi��o
void glrNeck(),glrNeck_edit();      //(na posi��o do estadado GL)
void glrHead(),glrHead_edit();
void glrLftUpArm(),glrLftUpArm_edit();
void glrRtUpArm(),glrRtUpArm_edit();
void glrLftLowArm(),glrLftLowArm_edit();
void glrRtLowArm(),glrRtLowArm_edit();
void glrRtHand(),glrRtHand_edit();
void glrLftHand(),glrLftHand_edit(); // implementadas abaixo

// Matriz[10][2] de todas as fun��es que desenham 
// as partes do robo (normal/edi��o) Usado para 
// mudar a fun��o de desenho dos obj no mudo edi��o
void (*glr_func[ROBOT_NP][2])(void)={{&glrBase,&glrBase_edit},
				     {&glrChest,&glrChest_edit},
				     {&glrNeck,&glrNeck_edit},
				     {&glrHead,glrHead_edit},
				     {&glrLftUpArm,&glrLftUpArm_edit},
				     {&glrRtUpArm,&glrRtUpArm_edit},
				     {&glrLftLowArm,&glrLftLowArm_edit},
				     {&glrRtLowArm,&glrRtLowArm_edit},
				     {&glrRtHand,&glrRtHand_edit},
				     {&glrLftHand,&glrLftHand_edit},
                                    };

void robo_mount(void); //monta robo (ver abaixo)
/////////// End Robot ////////////

/////////// Gl ///////////
void blue_light (void)  // luz usada em modo normal (azul)
{
  GLfloat mat[3] = {0.8, 0.8, 0.8};
  glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
  mat[0] = 0.61424; mat[1] = 0.64136; mat[2] = 6.04136;	
  glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
  mat[0] = 0.727811; mat[1] = 0.626959; mat[2] = 0.626959;
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
  glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);
}

void red_light (void)  // luz usada em modo edi��o (vermelho)
{
  GLfloat mat[3] = {0.8, 0.8, 0.8};
  glMaterialfv (GL_FRONT, GL_AMBIENT, mat);
  mat[2] = 0.61424; mat[1] = 0.64136; mat[0] = 6.04136;	
  glMaterialfv (GL_FRONT, GL_DIFFUSE, mat);
  mat[2] = 0.727811; mat[1] = 0.626959; mat[0] = 0.626959;
  glMaterialfv (GL_FRONT, GL_SPECULAR, mat);
  glMaterialf (GL_FRONT, GL_SHININESS, 0.6*128.0);
}

// Esta fun��o desenha o texto passado como argumento na posi��o x,y 
// Fun��o "roubada" de http://www.tjhsst.edu/~dhyatt/superap/opengl.html
void DisplayText(double x, double y, char *string, void *font) {

  int len, i;

  glRasterPos2f(x, y);          // Locate Raster Position in 3-space
  len = (int) strlen(string);   // Find length of string
  for (i = 0; i < len; i++) {   // Loop through plotting all characters in font style
    glutBitmapCharacter(font, string[i]);
  }
}
/////////// End Gl ///////////

/////////// Glut /////////////
void init(void) 
{
  GLfloat mat[3] = {0.8, 0.8, 0.8};
  GLfloat position[] = { -5.0, 7.0, 10.0, 0.0 }; //posi��o da luz

  glShadeModel (GL_SMOOTH);   //GL_FLAT +rapido

  glClearColor (0.0, 0.0, 0.1, 0.0);
  glEnable(GL_DEPTH_TEST);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  blue_light();
  glPointSize(1.1);
  glLightfv(GL_LIGHT0, GL_POSITION, position);   //posicionar luz
}

void display(void)
{
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glColor3f (1.0, 1.0, 1.0);

  ///////////// Ponto de Vista
  double xeye=20.0*sin(Vang)*cos(Hang); //coordenadas esfericas
  double yeye=20.0*sin(Vang)*sin(Hang); //Mais rapido que 2 glRotate();
  double zeye=20.0*cos(Vang);   
  glLoadIdentity();
  if (sin(Vang)>0.) // tem que estar(unica diren�a � a orienta��o z=1|-1)
    gluLookAt (xeye, yeye, zeye, //onde
	       0.0, 0.0, 0.0, //para
	       0.0, 0.0, 1.0);//orienta��o
  else
    gluLookAt (xeye, yeye, zeye, //onde
	       0.0, 0.0, 0.0, //para
	       0.0, 0.0, -1.0);//orienta��o
  glScaled( Zoom,Zoom,Zoom);
  ///////////// Fim ponto de vista
  
  ///////////// Robot /////////////
  //
  //
  glPushMatrix(); // n�o necessario
  glTranslatef (0.0, 0.0, -1.0);//mandar tudo para baixo

  //objecto que est� a ser editado: mudar a cor(fun��o de desenho) 
  ROBOT[edit_obj].select_draw_func(glr_func[edit_obj][flag_mode]);
  ROBOT[Base].draw(); // desenhar o robo(come�a pelo pai de todos: Base)
  //Voltar a por a fun��o de desenho normal no edit_obj
  ROBOT[edit_obj].select_draw_func(glr_func[edit_obj][0]);

  // mostrar "osoos"
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  if (show_bones!=0) ROBOT[Base].show_bones(0.5);  // mostrar "ossos"
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);

  glPopMatrix();
  //
  ///////////// Fim Robot /////////////

  ///////////// Referencial
  glPushMatrix(); 
  glDisable(GL_DEPTH_TEST);

  glLoadIdentity();
 
  glTranslatef (9.0, 9.0, 0.0);
  glutWireCube (2.0);            //caixinha
  if (sin(Vang)>0.)
    gluLookAt (xeye, yeye, zeye, //onde
	       0.0, 0.0, 0.0,    //para
	       0.0, 0.0, 1.0);   //orienta��o
  else
    gluLookAt (xeye, yeye, zeye, //onde
	       0.0, 0.0, 0.0,    //para
	       0.0, 0.0, -1.0);  //orienta��o
  if (flag_mode)
    glColor3f(0.5,0.5,0.5); 
  else
    glColor3f(1.0,1.0,1.0); 
  glDisable(GL_LIGHTING);
  default_draw();    // Aproveitar a fun��o default de objik.h(faz os eixos)
  glEnable(GL_LIGHTING);
  glColor3f(1.0,1.0,1.0); 

  glEnable(GL_DEPTH_TEST);
  glPopMatrix();
  ///////////// fim Ref

  ///////////// Texto /////
  //
  glPushMatrix();
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);
  glLoadIdentity();
  gluLookAt (0.0, 0.0,10.0,   //onde
	       0.0, 0.0, 0.0, //para
	       0.0, 1.0, 0.0);//orienta��o

  ////////
  glPushMatrix();
  glTranslatef (-7.5, -7.9, 0.0);
  glEnable(GL_LIGHTING);
  glScaled(1.2,1.0,1.0);
  glutWireCube (4.0);         //caixinha
  glDisable(GL_LIGHTING);  
  glTranslatef (7.5, 7.9, 0.0); 
  glPopMatrix();
  ////////

  ////////
  glTranslatef (-9.5, -9.0, 0.0);
  DisplayText(0.0, 0.0, "[*]Edit:",GLUT_BITMAP_HELVETICA_10);
  glTranslatef (2.3,0.0,0.0);
  if (flag_mode)
    { DisplayText(0.0, 0.0, "On",GLUT_BITMAP_HELVETICA_10);}
  else
    { DisplayText(0.0, 0.0, "Off", GLUT_BITMAP_HELVETICA_10);}

  glTranslatef (-2.3,0.0,0.0);
   
  ////////
  glTranslatef (0.0, 1.0, 0.0);
  DisplayText(0.0, 0.0,"[+/-]Obejct:" ,GLUT_BITMAP_HELVETICA_10);
  glTranslatef (2.3,0.0,0.0);
  if (flag_mode) glColor3f(1.0,0.3,0.3); else glColor3f(0.3,0.3,1.0);
  DisplayText(0.0, 0.0,ROBOT[edit_obj].name ,GLUT_BITMAP_HELVETICA_10);
  glColor3f(1.0,1.0,1.0);
  glTranslatef (-2.3,0.0,0.0);

  glTranslatef (0.0, 1.0, 0.0);
  DisplayText(0.0, 0.0,"[B]Bones:" ,GLUT_BITMAP_HELVETICA_10);
  glTranslatef (2.3,0.0,0.0);
  if (show_bones)
    {glColor3f(1.0,1.0,1.0); DisplayText(0.0, 0.0, "Show",GLUT_BITMAP_HELVETICA_10);}
  else
    {glColor3f(0.5,0.5,0.5); DisplayText(0.0, 0.0, "Hide", GLUT_BITMAP_HELVETICA_10);}
  glColor3f(1.0,1.0,1.0);
  glTranslatef (-2.3,0.0,0.0);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  glPopMatrix();
  //
  ///////////// Fim Texto ////

  glutSwapBuffers(); 
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho(-10.0, 10.0, -10.0, 10.0,0.0, 50.0);
   // glFrustum (-1.0, 1.0, -1.0, 1.0, 1.0, 19.0);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
  switch (key) 
    {
    case 27:
      exit(0);
      break;
    case '*':      //GLUT_KEY_INSERT :
      if(flag_mode==0) flag_mode=1;
      else flag_mode=0;
      break;
    case '+':
      edit_obj++;
      if (edit_obj==ROBOT_NP) edit_obj=0;
      break; 
    case '-':
      edit_obj--;
      if (edit_obj==-1) edit_obj=9;
      break; 
    case 'b':
      if(show_bones==0) show_bones=1;
      else show_bones=0;

      break;
    }
}


void mouse(int button, int state, int x, int y)
{
  if (state== GLUT_DOWN)   //presiona um button para
    Mouse=button;
  else
    Mouse=-1;

  xant=x;
  yant=y;
}

void motion(int x, int y)
{
  if (flag_mode==0)
    {
      switch(Mouse)
	{
	case GLUT_LEFT_BUTTON:
	  Vang-=((double)(y-yant))/20;
	  Hang+=((double)(x-xant))/20;
	  break;
	case GLUT_RIGHT_BUTTON:
	  Zoom-=((double)(y-yant))/20;
	  break;
	}
    }
  else
    {
       switch(Mouse)
	{
	case GLUT_LEFT_BUTTON:
	    ROBOT[edit_obj].rotate_yy(((double)(y-yant)));
	    ROBOT[edit_obj].rotate_xx(((double)(x-xant)));
	    break;
	case GLUT_RIGHT_BUTTON:
	    ROBOT[edit_obj].rotate_zz(((double)(x-xant)));
	  break;
	}
    }
  xant=x;
  yant=y; 
}

void deamon(void)
{
  glutPostRedisplay();
}
/////////// End Glut ////////////

/////////// Main /////////////
int main(int argc, char** argv)
{
  robo_mount();//fazer robo
  glutInit(&argc, argv);
  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500, 500); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow (argv[0]);
  init ();
  glutDisplayFunc(display); 
  glutReshapeFunc(reshape);
  
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  
  glutKeyboardFunc(keyboard);
  glutIdleFunc(deamon); 
  glutMainLoop();
 
  return 0;
}

/////////// Robot /////////////
void robo_mount(void)
{
  //Associar nomes e fun�oes de desenho aos IK_OBJECT'S
  //Base
  ROBOT[Base].select_draw_func(&glrBase); // fun��o que desenha
  ROBOT[Base].new_name("Base");           //nome do obj(n�o importante)
  // Peito
  ROBOT[Chest].select_draw_func(&glrChest);
  ROBOT[Chest].new_name("Chest");
  // Pesco�o
  ROBOT[Neck].select_draw_func(&glrNeck);
  ROBOT[Neck].new_name("Neck");
  // Cabe�a
  ROBOT[Head].select_draw_func(&glrHead);
  ROBOT[Head].new_name("Head");
  // Antebra�o esquerdo
  ROBOT[LftUpArm].select_draw_func(&glrLftUpArm);
  ROBOT[LftUpArm].new_name("LftUpArm");
  // Antebra�o Direito
  ROBOT[RtUpArm].select_draw_func(&glrRtUpArm);
  ROBOT[RtUpArm].new_name("RtUpArm");
  // Bra�o esquerdo
  ROBOT[LftLowArm].select_draw_func(&glrLftLowArm);
  ROBOT[LftLowArm].new_name("LftLowArm");
  // Bra�o Direito
  ROBOT[RtLowArm].select_draw_func(&glrRtLowArm);
  ROBOT[RtLowArm].new_name("RtLowArm");
  // M�o direita
  ROBOT[RtHand].select_draw_func(&glrRtHand);
  ROBOT[RtHand].new_name("RtHand");
  // M�o esquerda
  ROBOT[LftHand].select_draw_func(&glrLftHand);
  ROBOT[LftHand].new_name("LftHand");
  
  ////////////////////////// "Bones" ////////////////////////
  //  Aqui � a parte onde se diz as hierarquias, a localiza��o 
  //de um objecto em rela��o ao pai e o seu pr�prio eixo.
  //
  //  As limita��es de angulos e transla��es ocupam muitas 
  //linhas de codigo mas n�o s�o importantes.               
  //
  // Base (n�o roda nem mexe)
  ROBOT[Base].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[Base].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[Base].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                  //fica tudo em 0,0,0
  ROBOT[Base].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[Base].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[Base].FLAG_LIM_Z=1;       //limitar tranla��o em z
  
  ROBOT[Base].new_child(&ROBOT[Chest]); // peito: filho da base

  // Peito (roda alguma coisa mas n�o mexe)
  ROBOT[Chest].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[Chest].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[Chest].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                   //fica tudo em 0,0,0
  ROBOT[Chest].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[Chest].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[Chest].FLAG_LIM_Z=1;       //limitar tranla��o em z

  ROBOT[Chest].pos=vec3(0.0,0.0,2.0);      //Posi��o em rela��o ao pai
  ROBOT[Chest].pivot=vec3(0.0,0.0,-1.0);   //Centro de rota��o em rela��o a pos
  
  ROBOT[Chest].rot_max=vec3(20.0,90.0,90.0); // defenir novos limites rota��o
  ROBOT[Chest].rot_min=vec3(-20.0,-20.0,-90.0);
  
  ROBOT[Chest].new_child(&ROBOT[Neck]);    // Neck filho de peito

  // Pesco�o 
  ROBOT[Neck].FLAG_LIM_XX=1;        //limitar rota��o em xx
  ROBOT[Neck].FLAG_LIM_YY=1;        //limitar rota��o em yy
  ROBOT[Neck].FLAG_LIM_ZZ=1;        //limitar rota��o em zz
                                    //fica tudo em 0,0,0
  ROBOT[Neck].FLAG_LIM_X=1;         //limitar tranla��o em x
  ROBOT[Neck].FLAG_LIM_Y=1;         //limitar tranla��o em y
  ROBOT[Neck].FLAG_LIM_Z=1;         //limitar tranla��o em z

  ROBOT[Neck].pos=vec3(0.0,0.0,1.0);      //Posi��o em rela��o ao pai
  ROBOT[Neck].pivot=vec3(0.0,0.0,-0.2);   //Centro de rota��o em rela��o a pos
  
  ROBOT[Neck].rot_max=vec3(20.0,55.0,45.0); // defenir novos limites rota��o
  ROBOT[Neck].rot_min=vec3(-20.0,-25.0,-45.0);
  
  ROBOT[Neck].new_child(&ROBOT[Head]);  // Head filho de neck
  
  // Cabe�a
  ROBOT[Head].FLAG_LIM_XX=1;        //limitar rota��o em xx
  ROBOT[Head].FLAG_LIM_YY=1;        //limitar rota��o em yy
  ROBOT[Head].FLAG_LIM_ZZ=1;        //limitar rota��o em zz
                                    //fica tudo em 0,0,0
  ROBOT[Head].FLAG_LIM_X=1;         //limitar tranla��o em x
  ROBOT[Head].FLAG_LIM_Y=1;         //limitar tranla��o em y
  ROBOT[Head].FLAG_LIM_Z=1;         //limitar tranla��o em z

  ROBOT[Head].pos=vec3(0.0,0.0,0.55);      //Posi��o em rela��o ao pai
  ROBOT[Head].pivot=vec3(0.0,0.0,-0.45);   //Centro de rota��o em rela��o a pos 

  ROBOT[Head].rot_max=vec3(20.0,25.0,45.0); // defenir novos limites rota��o
  ROBOT[Head].rot_min=vec3(-20.0,-25.0,-45.0);
  
  // Antebra�o esquerdo 
  ROBOT[Chest].new_child(&ROBOT[LftUpArm]); // filho de chest

  ROBOT[LftUpArm].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[LftUpArm].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[LftUpArm].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                      //fica tudo em 0,0,0
  ROBOT[LftUpArm].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[LftUpArm].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[LftUpArm].FLAG_LIM_Z=1;       //limitar tranla��o em z

  ROBOT[LftUpArm].pos=vec3(0.0,0.85,0.0);      //Posi��o em rela��o ao pai
  ROBOT[LftUpArm].pivot=vec3(0.0,-0.10,0.50);   //Centro de rota��o em rela��o a pos 

  ROBOT[LftUpArm].rot_max=vec3(190.0,80.0,90.0); // defenir novos limites rota��o
  ROBOT[LftUpArm].rot_min=vec3(-45.0,-180.0,-90.0);

  // Bra�o esquerdo
  ROBOT[LftUpArm].new_child(&ROBOT[LftLowArm]); // low filho de up

  ROBOT[LftLowArm].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[LftLowArm].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[LftLowArm].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                            //fica tudo em 0,0,0
  ROBOT[LftLowArm].FLAG_LIM_X=1;      //limitar tranla��o em x
  ROBOT[LftLowArm].FLAG_LIM_Y=1;      //limitar tranla��o em y
  ROBOT[LftLowArm].FLAG_LIM_Z=1;      //limitar tranla��o em z

  ROBOT[LftLowArm].pos=vec3(0.0,0.15,-1.20);  //Posi��o em rela��o ao pai
  ROBOT[LftLowArm].pivot=vec3(0.0,0.0,0.55);  //Centro de rota��o em rela��o a pos 

  ROBOT[LftLowArm].rot_max=vec3(0.0,0.0,0.0); // defenir novos limites rota��o
  ROBOT[LftLowArm].rot_min=vec3(0.0,-170.0,0.0);

  //M�o esquerda 
  ROBOT[LftLowArm].new_child(&ROBOT[LftHand]); // Hand filho de LowArm

  ROBOT[LftHand].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[LftHand].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[LftHand].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                     //fica tudo em 0,0,0
  ROBOT[LftHand].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[LftHand].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[LftHand].FLAG_LIM_Z=1;       //limitar tranla��o em z

  ROBOT[LftHand].pos=vec3(0.0,0.10,-1.10);     //Posi��o em rela��o ao pai
  ROBOT[LftHand].pivot=vec3(0.0,-0.05,0.45);   //Centro de rota��o em rela��o a pos 

  ROBOT[LftHand].rot_max=vec3(45.0,90.0,0.0);  // defenir novos limites rota��o
  ROBOT[LftHand].rot_min=vec3(-45.0,-90.0,-180.0);

  // Antebra�o dierito
  ROBOT[Chest].new_child(&ROBOT[RtUpArm]); // filho de chest

  ROBOT[RtUpArm].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[RtUpArm].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[RtUpArm].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                     //fica tudo em 0,0,0
  ROBOT[RtUpArm].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[RtUpArm].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[RtUpArm].FLAG_LIM_Z=1;       //limitar tranla��o em z

  ROBOT[RtUpArm].pos=vec3(0.0,-0.85,0.0);     //Posi��o em rela��o ao pai
  ROBOT[RtUpArm].pivot=vec3(0.0,0.10,0.50);   //Centro de rota��o em rela��o a pos 

  ROBOT[RtUpArm].rot_max=vec3(45.0,80.0,90.0); // defenir novos limites rota��o
  ROBOT[RtUpArm].rot_min=vec3(-190.0,-180.0,-90.0);

  // Bra�o direito
  ROBOT[RtUpArm].new_child(&ROBOT[RtLowArm]); // low filho de up

  ROBOT[RtLowArm].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[RtLowArm].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[RtLowArm].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                      //fica tudo em 0,0,0
  ROBOT[RtLowArm].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[RtLowArm].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[RtLowArm].FLAG_LIM_Z=1;       //limitar tranla��o em z

  ROBOT[RtLowArm].pos=vec3(0.0,-0.15,-1.20);  //Posi��o em rela��o ao pai
  ROBOT[RtLowArm].pivot=vec3(0.0,0.0,0.55);   //Centro de rota��o em rela��o a pos 

  ROBOT[RtLowArm].rot_max=vec3(0.0,0.0,0.0); // defenir novos limites rota��o
  ROBOT[RtLowArm].rot_min=vec3(0.0,-170.0,0.0);

  //M�o direita
  ROBOT[RtLowArm].new_child(&ROBOT[RtHand]); // Hand filho de LowArm

  ROBOT[RtHand].FLAG_LIM_XX=1;      //limitar rota��o em xx
  ROBOT[RtHand].FLAG_LIM_YY=1;      //limitar rota��o em yy
  ROBOT[RtHand].FLAG_LIM_ZZ=1;      //limitar rota��o em zz
                                    //fica tudo em 0,0,0
  ROBOT[RtHand].FLAG_LIM_X=1;       //limitar tranla��o em x
  ROBOT[RtHand].FLAG_LIM_Y=1;       //limitar tranla��o em y
  ROBOT[RtHand].FLAG_LIM_Z=1;       //limitar tranla��o em z

  ROBOT[RtHand].pos=vec3(0.0,-0.10,-1.10);   //Posi��o em rela��o ao pai
  ROBOT[RtHand].pivot=vec3(0.0,0.05,0.45);   //Centro de rota��o em rela��o a pos 

  ROBOT[RtHand].rot_max=vec3(45.0,90.0,180.0); // defenir novos limites rota��o
  ROBOT[RtHand].rot_min=vec3(-45.0,-90.0,0.0);
}

void glrBase(){ draw_object(Base);}     //desenhar pe�as com a cor do estado
void glrChest(){draw_object(Chest);}
void glrNeck(){ draw_object(Neck);}
void glrHead(){ draw_object(3);}
void glrLftUpArm(){draw_object(4);}
void glrRtUpArm(){draw_object(5);}
void glrLftLowArm(){draw_object(6);}
void glrRtLowArm(){draw_object(7);}
void glrRtHand(){draw_object(8);}
void glrLftHand(){draw_object(9);}

// Desenhar as pe�as com luz vermelha  Modo edi��o
void glrBase_edit(){ red_light(); draw_object(0); blue_light();}
void glrChest_edit(){red_light(); draw_object(1);  blue_light();}
void glrNeck_edit(){red_light();draw_object(2); blue_light();}
void glrHead_edit(){red_light();draw_object(3); blue_light();}
void glrLftUpArm_edit(){red_light();draw_object(4); blue_light();}
void glrRtUpArm_edit(){red_light();draw_object(5); blue_light();}
void glrLftLowArm_edit(){red_light();draw_object(6); blue_light();}
void glrRtLowArm_edit(){red_light();draw_object(7); blue_light();}
void glrRtHand_edit(){red_light();draw_object(8); blue_light();}
void glrLftHand_edit(){red_light();draw_object(9); blue_light();}

/////////// End Robot /////////////
