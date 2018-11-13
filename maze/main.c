#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "include.h"

/** Vars **/
/* Common */
int debug, fullscreen, hwin = 0, first=0; 

/* Features */
int iCull=0, iDepth=1, iOutline=0;
int iGravi = 0;
int iBobby = 0;
/* Keyboard arrows */
int keys [4] = {0, 0, 0, 0};

/* Scene: common */
GLsizei WinH, WinW;
GLfloat Xmax, Ymax, Zmax;
GLfloat dxMov=3.0, dzMov=3.0, dyMov=3.0, dScale=0.1, scale=1.0, yMov=3.0;
GLfloat xlMov=0.0, ylMov=0.0, zlMov=0.0;
GLfloat	eyeX=-10.0, eyeY=30.0, eyeZ=0.0, lx=1.0f, ly=0.0f, lz=-1.0f;
GLfloat angle = M_PI / 2, dxmNold, dymNold, angleY = M_PI / 2, ang = 0.0;
int xm=-12345, ym, Xequ0=0, XequW=0, Yequ0=0, YequH=0;
GLfloat dxm=0.0, dym=0.0;

GLfloat gTime = 0; //current time(milisec) sinec game start;
/* Lighting */
GLfloat ambLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat difLight[] = { 0.8f, 0.8f, 0.8f, 1.0f };
GLfloat sptLightDir[] = { 0.0f, 0.0f, -1.0f};
GLfloat spcLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat	lightPos[] = { 1000.0, 10.0, 1075.0, 1.0f };

/* Static buffers */
char str [400];
GLfloat mas1 [NMAX_SPHERES * 4];

/* Dynamic buffers */
int szobject, szdrawns;
/////////// Big Objects templates
char regobjmas[N_OBJECT_TYPES + 1]; // {Ntypes, type_counts}
char *objects; 	//for maze and etc...used by GenObject
					    //{gen_of_object, data_of_object;...}
char *pofree; 	//pointer to the EOF obj

//////////// Drawn Big obects
char *drawns; 	//for maze and etc...used by DrawObject
					    //{params_of_drawn_object, data_of_drawn_object;...}
char *pdfree; 	//pointer to the EOF drawn objects

////////// simple objects, inhabitants of Big Objects.
char *pscene = NULL; //{ n_big_objects, n_elements_in_bigobj, sizeof_scene_object_str, scene_obj_strs..}
char *psfree = NULL;

/////////// world coordinates of other small objects(used for building BigObjects)
char *PWC = NULL; // {struct membuf_hdr{}, data...}

void **memptrs = NULL;
int nptrs = 0;
int ntotptrs = 0;

GLubyte fire[128] = { 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00,
				   0x00, 0x00, 0x00, 0xc0,  0x00, 0x00, 0x01, 0xf0, 0x00, 0x00, 0x07, 0xf0,
				   0x0f, 0x00, 0x1f, 0xe0, 0x1f, 0x80, 0x1f, 0xc0,  0x0f, 0xc0, 0x3f, 0x80,	
				   0x07, 0xe0, 0x7e, 0x00,  0x03, 0xf0, 0xff, 0x80, 0x03, 0xf5, 0xff, 0xe0,
				   0x07, 0xfd, 0xff, 0xf8,  0x1f, 0xfc, 0xff, 0xe8,  0xff, 0xe3, 0xbf, 0x70, 
				   0xde, 0x80, 0xb7, 0x00,  0x71, 0x10, 0x4a, 0x80, 0x03, 0x10, 0x4e, 0x40,  0x02, 0x88, 0x8c, 0x20,
				   0x05, 0x05, 0x04, 0x40,  0x02, 0x82, 0x14, 0x40, 0x02, 0x40, 0x10, 0x80,  0x02, 0x64, 0x1a, 0x80,
				   0x00, 0x92, 0x29, 0x00,  0x00, 0xb0, 0x48, 0x00,  0x00, 0xc8, 0x90, 0x00,  0x00, 0x85, 0x10, 0x00,
				   0x00, 0x03, 0x00, 0x00,  0x00, 0x00, 0x10, 0x00 };
	

void setOrtho() {

	// switch to projection mode
 	glMatrixMode(GL_PROJECTION);
	// save previous matrix which contains the 
	//settings for the perspective projection
	glPushMatrix();
	// reset matrix
	glLoadIdentity();
	// set a 2D orthographic projection
	gluOrtho2D(0, WinW, 0, WinH);
	glMatrixMode(GL_MODELVIEW);
}

static void resetPerspective() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}


void Raster(void)
{
	int x, y;
	
	// Loop through 16 rows and columns
	for(y = 0; y < 4; y++)
		{
		// Set raster position for this "square"
		glRasterPos2i(0, y * 32); // in pixels, (0,0) - low left corner
		for(x = 0; x < 4; x++)
			// Draw the "fire" bitmap, advance raster position
			glBitmap(32, 32, 0.0, 0.0, 32.0, 0.0, fire); // 32x32 bits; 0.0,0.0 - origin in bitmap
														// change cur_raster_pos: (x+32.0,y+0.0)
		}
		
}


/* TEXTURES:
1.loading:
1.1 TexImage(1-3)D: mem->T
1.2 CopyTexImage(1-2)D: COLOR_BUF->T

2. updating:
2.1 TexSubImage(1-3)D: mem->T
2.2: CopyTexSubImage(1-3)D: COLOR_BUF->T

3. coords:
3.1 TexCoord(1-3)f
*/

void LoadTexture(const char *img)
{
	GLbyte *pTex = NULL;
	GLint iWidth, iHeight, iComponents, depth, border = 0;
	GLenum eFormat;
	int lod = 0;

	if((pTex = LoadTGA(img, &iWidth, &iHeight, &iComponents, &eFormat, &depth))){ 
        glTexImage2D(GL_TEXTURE_2D, lod, iComponents, iWidth, iHeight, border, eFormat, GL_UNSIGNED_BYTE, pTex);
		free(pTex);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
	}	
    //glCopyTexImage2D(GL_TEXTURE_2D, GL_RGB, WinW/2, WinH/2, iWidth, iHeight, border);
}

void Image(const char *img, int x, int y, int fscr)
{
	GLbyte *pImage = NULL;
	GLint iWidth, iHeight, iComponents;
	GLenum eFormat;
	GLint vp[4], depth;	
	
    // Targa's are 1 byte aligned
    // Load the TGA file, get width, height, and component/format information
	if((pImage = LoadTGA(img, &iWidth, &iHeight, &iComponents, &eFormat, &depth))){ 
	    // Use Window coordinates to set raster position
		glRasterPos2i(x, y);
	    // Draw the pixmap
		if(fscr) {
			glGetIntegerv(GL_VIEWPORT, vp);
			printf("vp2 = %d wid= %d\n", vp[2], iWidth);
			glPixelZoom(vp[2] / iWidth, vp[3] / iHeight);
		}
        glDrawPixels(iWidth, iHeight, eFormat, GL_UNSIGNED_BYTE, pImage);
	}	
    // Don't need the image data anymore
	free(pImage);
}

void CopyPix(int dstX, int dstY, int srcX, int srcY, int wid, int hei)
{
	glRasterPos2i(dstX , dstY );
	glCopyPixels(srcX, srcY, wid, hei, GL_COLOR);
//	glCopyPixels(srcX, srcY, wid, hei, GL_STENCIL);
//	glCopyPixels(srcX, srcY, wid, hei, GL_DEPTH);
	//cpx = (!flg) ? cpx + 1 : cpx - 1;
	//cpy = (!flg) ? cpy + 1 : cpy - 1;
	//flg = ((cpx > WinW - wid) || !cpx || !cpy || (cpy > WinH - hei)) ? flg ^ 1 : flg;
	return;
}


static void Draw(void)
{
	char fps [50];

	if (debug) {
		memset(str, 0, sizeof(str));
		sprintf(str, "Eye=(%5.2lf %5.2lf %5.2lf) CenterRel2Eye=(%5.2lf %5.2lf %5.2lf) Scale=%5.2lf", 
				eyeX, eyeY, eyeZ, lx,ly, lz, scale);
		printf("%s\n", str);
	}

	/* Init bufers */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* Turn on/off features from Menu */
	if(iCull)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);

	if(iDepth)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	if(iOutline) 
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	/* Scale and up/down */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); //push Identity matrix

	/* Gravitation */
	Gravitation();

	humanwalk(-1);

	glTranslatef(0.0, yMov, 0.0); 
	glScalef(scale,scale,scale);
	
	/* Light */
	lightPos[0] += xlMov; lightPos[1] += ylMov;	lightPos[2] += zlMov;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	/**** HERE WE HAVE INITIAL WORLD COORD SYSTEM *********/

	DrawGround();

	glRotatef(ang, 0.0f, 1.0f, 0.0f);
	/* Scene: cube1 */
	glPushMatrix();
	glutSolidSphere(30.0, 5, 5);
	Block(2.0, 2.0, 2.0, 5.0, 30.0, 35.0, NULL);
	glPopMatrix();
	glPushMatrix();
	Block(10.0, 10.0, 10.0, 0.0, 20.0, 30.0, NULL);
	glPopMatrix();

	
	/* Ortho pictures */
	setOrtho();
	glPushMatrix();
	glLoadIdentity();
	sprintf (fps, "FPS: %f", floor(1000.0/TIMEOUT1));
	output(10.0, WinH - 30.0, fps);
	Pricel();
	Raster();
	Image("Fire.tga",0,0,0);
	Image("tex1.tga",0,800,0);
	//CopyPix(0,WinH/2,WinW/2,WinH/2,WinW/2,WinH/2);
	glPopMatrix();
	resetPerspective();



	/* Scene params update */
	ang += 1.0;

	/* checks */

	glPopMatrix();
    glutSwapBuffers();
}


static void Setup()
{
	/* Features */
	glShadeModel(GL_SMOOTH);
	//glShadeModel(GL_FLAT);
	glFrontFace(GL_CCW);
	srand(123);//time(NULL));

	/* Light */
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0,GL_AMBIENT,ambLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,difLight);
	glLightfv(GL_LIGHT0,GL_SPECULAR,spcLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
//	glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,60.0);
	glEnable(GL_LIGHT0);

	/* Material */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, spcLight);
	glMateriali(GL_FRONT, GL_SHININESS, 128); //[1;128], 
	glEnable(GL_NORMALIZE);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f );

	/* Raster */
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	/* Texrure */
	LoadTexture("tex1.tga");

	/* Generating */
}

int main(int argc, char **argv)
{
	int x,y;
	char mode [0x100];	

	/* Init screen */
	glutInit(&argc, argv); 
	x = glutGet(GLUT_SCREEN_WIDTH);
	y = glutGet(GLUT_SCREEN_HEIGHT);
#ifdef DEBUG
	debug = 1;
#else
	debug = 0;
#endif
	if(argc > 1) 
		fullscreen = 0;
	else { 
#ifdef FULLSCREEN
	fullscreen = FULLSCREEN;
#else
	fullscreen = 0;
#endif
	}

#ifdef DEF_W
	WinW = DEF_W;
	WinH = DEF_H;
#else
	WinW = x;
	WinH = y;
#endif
	if(argc > 1 && debug){
		WinW = x;
		WinH = y;
		fullscreen = 1;
	}
	glutInitWindowSize(WinW, WinH);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
#ifdef COLOR_DEPTH
	sprintf(mode, "%dx%d:%d", x, y, COLOR_DEPTH);
#else
	sprintf(mode, "%dx%d", x, y);
#endif
	if(fullscreen){
		glutGameModeString(mode);
		if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
			glutEnterGameMode();
			printf("%d", glutGameModeGet(GLUT_GAME_MODE_PIXEL_DEPTH));
		} else {
			return 1;
		}
	}else
		hwin = glutCreateWindow("MAZE");

	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);
	glutSetCursor(GLUT_CURSOR_NONE);

	/* Menu */
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("Toggle depth test",1);
	glutAddMenuEntry("Toggle cull backface",2);
	glutAddMenuEntry("Toggle outline back",3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	/* Setup */
	Setup();
	glutIdleFunc(Idle);
	glutTimerFunc(TIMEOUT1, Timer, ID_TIMER1);
	glutReshapeFunc(Resize3d);
	glutDisplayFunc(Draw);
	
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecKey);
	glutSpecialUpFunc(SpecKeyUp);
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(MouseMove);

	/* Go! */
	glutMainLoop();

	printf("%s\n",mode);
	return 0;
}
