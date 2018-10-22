#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
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
#define DEF_W 1280
#define DEF_H 1024
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

// Define targa header. This is only used locally.
#pragma pack(1) //align =1
typedef struct _tgaheader
{
    GLbyte	identsize;              // Size of ID field that follows header (0)
    GLbyte	colorMapType;           // 0 = None, 1 = paletted
    GLbyte	imageType;              // 0 = none, 1 = indexed, 2 = rgb, 3 = grey, +8=rle
    unsigned short	colorMapStart;          // First colour map entry
    unsigned short	colorMapLength;         // Number of colors
    unsigned char 	colorMapBits;   // bits per palette entry
    unsigned short	xstart;                 // image x origin
    unsigned short	ystart;                 // image y origin
    unsigned short	width;                  // width in pixels
    unsigned short	height;                 // height in pixels
    GLbyte	bits;                   // bits per pixel (8 16, 24, 32)
    GLbyte	descriptor;             // image descriptor
} TGAHEADER;
#pragma pack(8) //align =8


/********************************** MATH *************************************/
typedef GLfloat Vx[3];
typedef GLfloat Matrix33[9];
typedef GLfloat Matrix44[16];
#define M33(row,col)  m[col*3 + row]


/************************************ Functions ******************************/
/* Debug */
void output(GLfloat x, GLfloat y, char *string);
void chk1(void);


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


/* Actions */
void humanwalk(int dir);
void Gravitation(void);
GLbyte *LoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat,GLint *depth);
