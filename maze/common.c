#include "include.h"

/*
GL_POINTS  	индивидуальные точки
GL_LINES 	вершины попарно интерпретируются как самостоятельные отрезки
GL_LINE_STRIP 	серия соединенных отрезков (ломаная)
GL_LINE_LOOP 	аналогично предыдущему, но, кроме того, автоматически добавляется отрезок, соединяющий первую и последнюю вершины (замкнутая ломаная)
GL_TRIANGLES 	каждая тройка вершин интерпретируется как треугольник
GL_TRIANGLE_STRIP 	цепочка соединенных треугольников
GL_TRIANGLE_FAN 	веер из соединенных треугольников
GL_QUADS 	каждая четверка вершин интерпретируется как четырехугольный полигон
GL_QUAD_STRIP 	цепочка соединенных четырехугольников
GL_POLYGON 	граница простого выпуклого полигона
*/

GLfloat HWbias = 0.0, HWang = 0.0;
int iup = 0, timeNoGravi = 0;
GLfloat speedGravi = 1.5, totDY = 0.0;

extern int debug, first, WinH, WinW, iGravi, iBobby; 
extern GLfloat ang;
extern GLfloat mas1 [NMAX_SPHERES * 4];
extern char *pofree, *pscene, *psfree, *PWC; 	//pointer to the EOF obj
extern GLfloat Zmax, yMov;
extern GLfloat gTime, eyeX, eyeY, eyeZ, lx, ly, lz;
extern char *regobjmas;

GLuint	texture[1];

/***************************** Maintanance routines ***************/
///////////////////////////////////////////////////////////////////////////////
// This function determines if the named OpenGL Extension is supported
// Returns 1 or 0
int IsExtSupported(const char *extension)
	{
	GLubyte *extensions = NULL;
	const GLubyte *start;
	GLubyte *where, *terminator;
	
	where = (GLubyte *) strchr(extension, ' ');
	if (where || *extension == '\0')
		return 0;
	
	extensions = (GLubyte *)glGetString(GL_EXTENSIONS);
	
	start = extensions;
	for (;;) 
		{
		where = (GLubyte *) strstr((const char *) start, extension);
		
		if (!where)
			break;
		
		terminator = where + strlen(extension);
		
		if (where == start || *(where - 1) == ' ') 
			{
			if (*terminator == ' ' || *terminator == '\0') 
				return 1;
			}
		start = terminator;
		}
	return 0;
	}


static void _output(GLfloat x, GLfloat y, int sp, void *font, char *string)
{
	char *c;
   	int x1=x;
    
	for (c = string; *c != '\0'; c++) {
		glRasterPos2f(x1, y);
		glutBitmapCharacter(font, *c);
		x1 = x1 + glutBitmapWidth(font, *c) + sp;
	}
}

void output(GLfloat x, GLfloat y, char *string)
{
	
//	glColor3f(1.0f, 1.0f, 1.0f);
	glColor3f(1.0f, 0.0f, 0.0f);
	_output(x, y, 2, GLUT_BITMAP_HELVETICA_18, string);

}
//
// {int type, int len, data}
int *GetTlvDataEnd(void *tlvbuf)
{
	int *pi;
	char *pc;
	
	for (pi = (int*)tlvbuf; pi[1] && pi[0]; pc = (char*)pi, pc += *pi, pi = (int*)pc);
	return pi;
}


/******************************** Actions ************************/
void humanwalk(int dir)
{
	int chastota = 2.0;
	if(!iBobby)
		return;
	switch(dir) {
		case KEY_U:
		    if (HWang >= 359.0f)
            	HWang = 0.0f; 
      		else               
           		HWang += 10.0;
       		HWbias = sin(ATOR(chastota * HWang));
			break;

		case KEY_D:
		    if (HWang <= 1.0f)
            	HWang = 359.0f; 
      		else               
           		HWang -= 10.0;
       		HWbias = sin(ATOR(chastota * HWang));
			break;
		case -1:
			if (iup) {
				iup = 0;
				humanwalk(KEY_U);
			} else {
				iup = 1;
				humanwalk(KEY_D);
			}
			return;
		default:
			break;
	}
	yMov += HWbias * HUMAN_WALK_DELTA;
//	dprintf("HWbias = %f\n", HWbias);
}


void Pricel()
{
	int sz = 8.0;
	int lsz = 2.0;
	GLfloat fsz[2], fgr;

	glGetFloatv(GL_LINE_WIDTH_RANGE, fsz);
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY, &fgr);

	glLineWidth(fsz[0] + floor((lsz - fsz[0]) / fgr) * fgr);
	glBegin(GL_LINES);
	glVertex2f(WinW / 2, WinH / 2 + sz); glVertex2f(WinW / 2, WinH / 2 - sz);
	glVertex2f(WinW / 2 - sz, WinH / 2); glVertex2f(WinW / 2 +  sz, WinH / 2);
	glEnd();
	glLineWidth(1.0);
	//printf("fsz=%f fgr=%f my=%f\n",fsz[1],fgr, mysz);
//	exit(0);

}

void Gravitation()
{
	GLfloat dy, t;

	if(!iGravi) {
		timeNoGravi += TIMEOUT1;
		return;
	}
	t = (GLfloat)TIMEOUT1 / 1000.0; //delta_t (sec)
	dy = t * t * GRAVI_CONST / 2 + speedGravi * t;	
	speedGravi += GRAVI_CONST * t;
	
	totDY += dy;
	dprintf("GRAVI t =%f dy = %f v = %f totDY=%f yMov=%f\n", gTime/1000.0, dy, speedGravi, totDY,yMov);
}

/******************************** Simple objects drawing (along Z+) ******************/
void DrawGround()
{
	GLfloat dxGrnd = Zmax / 200, xGrnd = Zmax, yGrnd= -100.0, i;

	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);	
	for(i = -xGrnd; i <= xGrnd; i += dxGrnd){
		glVertex3f(i, yGrnd, -xGrnd);
		glVertex3f(i, yGrnd, xGrnd);
		glVertex3f(-xGrnd, yGrnd, i);
		glVertex3f(xGrnd, yGrnd, i);
	}
	glEnd();
	/* save wc */
}

/* Block */
void Block(GLfloat dxB, GLfloat dyB, GLfloat dzB, GLfloat xB, GLfloat yB,
		    GLfloat zB, GLfloat *pang)
{


/*
	Vx bmas [] = {{0.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,1.0,1.0}, {0.0,0.0,1.0},
		      {1.0,0.0,0.0}, {1.0,1.0,0.0}, {1.0,1.0,1.0}, {1.0,0.0,1.0}};
	GLubyte imas [] = {0,3,2,1,  0,4,7,3,  4,5,6,7, 1,2,6,5, 2,3,7,6, 0,1,5,4};
			   
	glVertexPointer(3, GL_FLOAT, 0, bmas);

	glColor3f(0.0f, 1.0f, 0.0f);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 2);
	glColor3f(0.0f, 1.0f, 1.0f);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 3);
	glColor3f(1.0f, 0.0f, 1.0f);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 4);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 5);
*/

	glTranslatef(xB, yB, zB); 
	if(pang)
		glRotatef(*pang, 1.0f, 0.0f, 1.0f);

	glScalef(dxB, dyB, dzB);

	glBegin(GL_QUADS);
	glColor3f(0.0f, 1.0f, 0.0f);
	//lft
	glNormal3f(-1.0,0.0,0.0);
	glTexCoord2f(0.0,0.0); glVertex3f(0.0,0.0,0.0);
	glTexCoord2f(1.0,0.0); glVertex3f(0.0,0.0,1.0);
	glTexCoord2f(1.0,1.0); glVertex3f(0.0,1.0,1.0); 
	glTexCoord2f(0.0,1.0); glVertex3f(0.0,1.0,0.0);
		
	//bot
	glColor3f(1.0f, 0.0f, 0.0f);
	glNormal3f(0.0,-1.0,0.0);
	glVertex3f(1.0,0.0,0.0);glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);glVertex3f(1.0,0.0,1.0);
	
	//rite
	glColor3f(0.0f, 0.0f, 1.0f);
	glNormal3f(1.0,0.0,0.0);
	glVertex3f(1.0,1.0,1.0);glVertex3f(1.0,0.0,1.0);
	glVertex3f(1.0,0.0,0.0);glVertex3f(1.0,1.0,0.0);
	
	
	//top
	glColor3f(0.0f, 1.0f, 1.0f);
	glNormal3f(0.0,1.0,0.0);
	glVertex3f(1.0,1.0,0.0);glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,1.0,1.0);glVertex3f(1.0,1.0,1.0);
		
	//back
	glColor3f(1.0f, 0.0f, 1.0f);
	glNormal3f(0.0,0.0,-1.0);
	glVertex3f(1.0,1.0,0.0);glVertex3f(1.0,0.0,0.0);
	glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,1.0,0.0);
	
	//front
	glColor3f(1.0f, 1.0f, 1.0f);
	glNormal3f(0.0,0.0,1.0);
	glVertex3f(1.0,1.0,1.0);glVertex3f(0.0,1.0,1.0);
	glVertex3f(0.0,0.0,1.0);glVertex3f(1.0,0.0,1.0);
	glEnd();

}

/********************************** Complex objects ********************/

////////////////////////////////////////////////////////////////////
// Allocate memory and load targa bits. Returns pointer to new buffer,
// height, and width of texture, and the OpenGL format of data.
// Call free() on buffer when finished!
// This only works on pretty vanilla targas... 8, 24, or 32 bit color
// only, no palettes, no RLE encoding.
GLbyte *LoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *iComponents, GLenum *eFormat, GLint *iDepth)
	{
    FILE *pFile;			// File pointer
    TGAHEADER tgaHeader;		// TGA file header
    unsigned long lImageSize;		// Size in bytes of image
    short sDepth;			// Pixel depth;
    GLbyte	*pBits = NULL;          // Pointer to bits
    
    *iWidth = 0;
    *iHeight = 0;
    *eFormat = GL_BGR_EXT;
    *iComponents = GL_RGB8;
  	*iDepth = 8;
  
    if(!(pFile = fopen(szFileName, "rb")))
        return NULL;
	
    if(!fread(&tgaHeader, sizeof(tgaHeader), 1, pFile))
		return NULL;
    
    *iWidth = tgaHeader.width;
    *iHeight = tgaHeader.height;
    sDepth = tgaHeader.bits / 8;
  	*iDepth = tgaHeader.bits;
    
    if(tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
        return NULL;
	
    lImageSize = tgaHeader.width * tgaHeader.height * sDepth;
    
    if((!(pBits = (GLbyte*)malloc(lImageSize * sizeof(GLbyte)))))
        return NULL;
    
    // Read in the bits
    // Check for read error. This should catch RLE or other 
    // weird formats that I don't want to recognize
    if(fread(pBits, lImageSize, 1, pFile) != 1){
        free(pBits);
        return NULL;
	}
    
    // Set OpenGL format expected
    switch(sDepth)
		{
        case 3:     // Most likely case
            *eFormat = GL_BGR_EXT;
            *iComponents = GL_RGB8;
            break;
        case 4:
            *eFormat = GL_BGRA_EXT;
            *iComponents = GL_RGBA8;
            break;
        case 1:
            *eFormat = GL_LUMINANCE;
            *iComponents = GL_LUMINANCE8;
            break;
		};
	
    
    // Done with File
    fclose(pFile);
	
    // Return pointer to image data
    return pBits;
}

