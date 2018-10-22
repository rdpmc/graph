#include <GL/glut.h>
#include <GL/gl.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/****************************** Defines ************************/
/* Global defines */


#define DEBUG 1


#if DEBUG == 1
#define FULLSCREEN 0
#if 1
#define DEF_W 1024
#define DEF_H 768
#endif
#else
#undef DEBUG
#define FULLSCREEN 1
#endif


#define TIMEOUT1 40
#define COLOR_DEPTH 32
/* Scene view defines */
#define GRAVI_CONST	  9.8
#define DELTA_MOVE_LR 2.0
#define DELTA_MOVE_FB 2.0

/* Scene objects defines */
#define N_OBJECT_TYPES 10 
#define NMAZES 5
#define NMAX_MAZE_PROLETS 10
#define NMAX_SPHERES 30

/* Scene actions defines */
#define HUMAN_WALK_DELTA 2


/* internal defines */
#define REALLOC_DELTA 4096
#define X 0
#define Y 1
#define Z 2

#define A 0
#define B 1
#define C 2
#define D 3

#define VAXIS 100.0		//sz of virtual object coord axis
#define TMPBUF_SZ	10000
#define ID_TIMER1 1
#define EKR 100.0
#define FANG 60.0
#define ZNEAR 1.0
#define ZFAR 15000.0

#define MAZE_UN -1
#define MAZE_F 0
#define MAZE_L 1
#define MAZE_R 2

#define KEY_L 0
#define KEY_R 1
#define KEY_U 2
#define KEY_D 3

#define SYSID_OBJECT 1
#define SYSID_DRAWN 2

/* Structures types*/
#define WORLD_COORDS 1


/** Objects types **/
#define TYPE_MAZE 1 //must start from 1,regobjmas
	/* Maze flags */
	#define FLAG_MAZE_VDOOR 1

#define TYPE_SPHERE 2
#define TYPE_BLOCK 3



/***************************** Macroses ***********************/
#define ATOR(x) (x * M_PI / 180)
#define RTOA(x) (x * 180 / M_PI)

#define dprintf(format, a...) \
		if (debug) { \
		printf(format, ##a); \
		fflush(stdout);}

#define MIN(min, x, y, z) \
	min = x; \
	min = min > y ? y : min; \
	min = min > z ? z : min;

/********************************** MATH *************************************/

typedef GLfloat Vx[3];
typedef GLfloat Matrix33[9];
typedef GLfloat Matrix44[16];
#define M33(row,col)  m[col*3 + row]

/******************************* Structures *************************/
struct header{
	int type;				//type of structure
	int n;
	size_t sz_obj_str;
	size_t sz_data_str;
	int id;					//object ID
};

struct membuf_hdr{
	char *pfree;
	size_t totsz;
};

							/*** TYPE_MAZE ***/
/****************************** SAVED  WORLD COORDS **********************/
struct wcoord {
	int type;
	int flags;
	int n; //N of Vx-es
	Vx  *pwc;
};
/******************************** OBJECT **********************/
struct obj_maze_data{
	int dir;
	GLfloat len;
};

struct obj_maze{
	struct header hdr;
	struct obj_maze_data od[];
};

/****************************** SCENE ********************/
struct scene_obj_hdr{
	int id;		//parent id
	int idelem;	//parent
	int ptype;  //par
	int ctype;  //child
	int n;
};

struct scene_obj {
	GLfloat vx; //virt coords ([0..100])
	GLfloat vy;
	GLfloat vz;
	GLfloat r;
	Vx wcc;		//world coord of center
};

/****************************** DRAWN ********************/
/*A -->x+ B*/
/*	|	  */
/*	|	  */
/*	v z+  */
/*D	     C*/

struct dd_header {
	int type;
	int nfilled; //at the end equ N of prolets
	GLfloat rotang; //angel of Z rotation for the last prolet
	int sz_hdr_str;
	int sz_data_str;
	int iready;		//data computed and no need recomputing
};


struct prim4 {
	Vx norm;
	Vx wc [4];
};

struct dd_maze_prolet_wcoord {
	Vx A;
	Vx B;
	Vx C;
	Vx D;
	int ugol;	
	Vx E;// ugol
	int nedges; //in primitive
	int nprims; 
	int nblocks; 
	struct prim4 *pprims;
};


struct drawn_maze {
	struct header hdr;
	int flags;
	GLfloat w; //gorisont l,r	/* Maze descr */
	GLfloat h;
	GLfloat w2;	//vertic u,d
	GLfloat h2;
	GLfloat x;
	GLfloat y;
	GLfloat z;
	void *ddata; //dd_header{}, dd_*{} [  //world coordinates of exterior]
};



/************************************ Functions ******************************/
/* Debug */
void output(GLfloat x, GLfloat y, char *string);
void chk1(void);
void shoot(void);

/* Core */
char *CheckCoreBuffersSz(int buftype, int sz);
void *FindObj(int type, int id);
void *FindDrawn(int type, int id);
void Cleanup(void);
void *Malloc(size_t sz);
void Memcpy(char **pbuf, void *src, size_t sz);

/* Callbacks */
void Idle(void);
void Timer(int val);
void ProcessMenu(int value);
void Mouse(int button, int state,int x, int y);
void MouseMove(int x, int y);
void SpecKeyUp(int key, int x, int y);
void SpecKey(int key, int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Resize3d(GLsizei w, GLsizei h);

/* Generating and Drawing */
void Pricel(void);
void Block(GLfloat dxB,GLfloat dyB,GLfloat dzB, GLfloat xB,GLfloat yB,GLfloat zB, GLfloat *pang);
void DrawGround(void);
int DrawSpheres(int n);
int GenMaze(void);
int DrawMaze(struct drawn_maze *p);
void DrawScene(struct header *pho, struct header *phd, int idel);

/*Simple objects*/
void DrawSphere(struct scene_obj_hdr *phs, struct header *pho, struct header *phd, int idel);

/* Actions */
void humanwalk(int dir);
void Gravitation(void);

/* Math */
void VxAdd(Vx v, GLfloat x, GLfloat y, GLfloat z);
void VxMov(Vx d, Vx s);
void LoadIdentity44(Matrix44 m);
void LoadIdentity33(Matrix33 m);
void VxRotate(Vx vOut, Vx p, GLfloat a, Vx axis);
void VxTranslate(Vx vOut, Vx v, GLfloat x, GLfloat y, GLfloat z);
void _VxVirtualAdd(Vx vx, GLfloat rotang, GLfloat x, GLfloat y, GLfloat z);
