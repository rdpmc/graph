#include "include.h"

/** Vars **/
/* Common */
int debug, fullscreen, hwin = 0, first=0; 

/* Features */
int iCull=0, iDepth=1, iOutline=0;
int iGravi = 0;
/* Keyboard arrows */
int keys [4] = {0, 0, 0, 0};

/* Scene: common */
GLsizei WinH, WinW;
GLfloat Xmax, Ymax, Zmax;
GLfloat dxMov=3.0, dzMov=3.0, dyMov=3.0, dScale=0.1, scale=1.0, yMov=3.0;
GLfloat xlMov=0.0, ylMov=0.0, zlMov=0.0;
GLfloat	eyeX=0.0, eyeY=400.0, eyeZ=200.0, lx=0.0f, ly=0.0f, lz=-1.0f;
GLfloat angle = M_PI / 2, dxmNold, dymNold, angleY = M_PI / 2, ang = 0.0;
int xm=-12345, ym, Xequ0=0, XequW=0, Yequ0=0, YequH=0;
GLfloat dxm=0.0, dym=0.0;

GLfloat gTime = 0; //current time(milisec) sinec game start;
/* Lighting */
GLfloat whiteLight[] = { 0.4f, 0.4f, 0.4f, 1.0f };
GLfloat sourceLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
GLfloat	lightPos[] = { 15.0f, 30.0f, 30.0f, 1.0f };

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

/********************************** FUNCTIONS ******************************/
void Memcpy(char **pbuf, void *src, size_t sz)
{
	char *pfree;
	size_t totsz, occup;
	struct membuf_hdr *phdr;

	if(!*pbuf) {
		if(!(*pbuf = malloc(sizeof(void*) + sizeof(size_t))))
			return;
		phdr = (struct membuf_hdr *)*pbuf;
		phdr->pfree = *pbuf + sizeof(void*) + sizeof(size_t);
		phdr->totsz = sizeof(void*) + sizeof(size_t);
	}
	phdr = (struct membuf_hdr *)*pbuf;
	occup = (long)(phdr->pfree) - (long)*pbuf;
	if(phdr->totsz - occup <= sz) {
		*pbuf = realloc(*pbuf, phdr->totsz + sz + REALLOC_DELTA);
		phdr = (struct membuf_hdr *)*pbuf;
		phdr->pfree = *pbuf;
		phdr->pfree += occup;
		phdr->totsz = totsz + sz + REALLOC_DELTA;
	}
	phdr = (struct membuf_hdr *)*pbuf;
	memcpy(pfree, src, sz);
	phdr->pfree += sz;
	return;
}

void *Malloc(size_t sz)
{
	int nptrs_ini_value = 64; //init number of pointers
	void *pret = NULL;
	nptrs++;
	if(!ntotptrs) {
		ntotptrs = nptrs_ini_value;
		memptrs = realloc(memptrs, ntotptrs * sizeof(char *));
	}
	if (nptrs > ntotptrs){
		ntotptrs += ntotptrs;
		if(!(memptrs = realloc(memptrs, ntotptrs * 2 * sizeof(char *))))
			return NULL;
	} 
	if (!(pret = calloc(sz, 1)))
		return pret;	
	memptrs[nptrs] = pret;
	//dprintf("nptr:%d ntot:%d allocsz:%d\n", nptrs, ntotptrs, sz);
	return pret;	
}

static void *_FindObjectOrDrawn(int type, int id, int masid)
{
	int found = 0;
	char *p0, *pend, *pc;
	struct header *p, *pret = NULL;

	if (masid == SYSID_OBJECT) {
		p0 = objects;
		pend = pofree;
	} else {
		p0 = drawns;
		pend = pdfree;
	}
	for(p = (struct header*)p0; p < (struct header*)pend; ) {
		if(p->type == type && p->id == id) {
			found = 1;
			break;
		}
		pc = (char*)p;
		pc += p->n * p->sz_data_str + p->sz_obj_str;
		p = (struct header*)pc;
	}
	if (found) 
		pret = p;
	return pret;
}


void *FindObj(int type, int id)
{
	return _FindObjectOrDrawn(type, id, SYSID_OBJECT);
}


void *FindDrawn(int type, int id)
{
	return _FindObjectOrDrawn(type, id, SYSID_DRAWN);
}

static struct header* _FindLastObjectOrDrawn(int type, int masid)
{
	int n = 0, found = 0;
	struct header *p, *pret;
	char *p0, *pend, *pc;

	if (masid == SYSID_OBJECT) {
		p0 = objects;
		pend = pofree;
	} else {
		p0 = drawns;
		pend = pdfree;
	}
	for(p = (struct header*)p0; p < (struct header*)pend; ) {
		if(p->type == type) {
			found = 1;
			pret = p;
			n++;
		}
		pc = (char*)p;
		pc += p->n * p->sz_data_str + p->sz_obj_str;
		p = (struct header*)pc;
	}
	if (!found) 
		pret = NULL;
	return pret;
}

static struct header* FindLastObj(int type) 
{
	return _FindLastObjectOrDrawn(type, SYSID_OBJECT);
}
static struct header* FindLastDrawn(int type)
{
	return _FindLastObjectOrDrawn(type, SYSID_DRAWN);
}


char *CheckCoreBuffersSz(int buftype, int sz)
{
	char *p;
	int occup;
	if(buftype == SYSID_DRAWN){
		p = pdfree;
		occup = pdfree - drawns;
		if(occup + sz >= szdrawns){
			p = realloc(drawns, szdrawns + REALLOC_DELTA);
			pdfree = p + occup;
			p = pdfree;
			szdrawns += REALLOC_DELTA;
		}

	} else {
		p = pofree;
		occup = pofree - objects;
		if(occup + sz >= szobject){
			p = realloc(objects, szobject + REALLOC_DELTA);
			pofree = p + occup;
			p = pofree;
			szobject += REALLOC_DELTA;
		}
	}
	return p;
}

static void FillHeader(struct header *pnew, struct header *pold, int type, int masid)
{
	size_t sz1, sz2;
	int id = 0;
	struct header *phlast;

	if (masid == SYSID_DRAWN) {
		if((phlast = FindLastDrawn(type)))
			id = phlast->id + 1;
	} else {
		if((phlast = FindLastObj(type)))
			id = phlast->id + 1;
	}
	switch(type) {
		case TYPE_MAZE:
			if (masid == SYSID_DRAWN){ 
				sz1 = sizeof(struct drawn_maze);
				sz2 = 0;
			} else{
				sz1 = sizeof(struct obj_maze);
				sz2 = sizeof(struct obj_maze_data);
			}
			break;
		default:
			break;
	}
	pold->id = id;
	pold->sz_obj_str = sz1;
	pold->sz_data_str = sz2;
	if(masid == SYSID_DRAWN){
//		pnew = (struct header *)CheckCoreBuffersSz(SYSID_DRAWN, sz1);
		pdfree += sz1;
	}else{
//		pnew = (struct header *)CheckCoreBuffersSz(SYSID_OBJECT, sz1);
	}
	memcpy(pnew, pold, sz1);
}

static int GenObject(void *p)
{
	int ret = 0;
	struct header *ph = p;

	switch(ph->type){
		case TYPE_MAZE:
			FillHeader((struct header*)pofree, ph, ph->type, SYSID_OBJECT);
			GenMaze();
			break;
/*
		case TYPE_SPHERE:
			FillHeader((struct header*)pofree, ph, ph->type, SYSID_OBJECT);
			GenSphere();
			break;

		case TYPE_CUBE:
			FillHeader((struct header*)pofree, ph, ph->type, SYSID_OBJECT);
			GenCube();
			break;
*/
		default:
			ret = -1;
			break;
	}
	return ret;
}

static void GenerateObjects()
{
	struct obj_maze p1 = { {TYPE_MAZE, NMAX_MAZE_PROLETS, 0, 0, 0} };
//	struct obj_maze p2 = { {TYPE_SPHERE, 1, 0, 0, 0} };
//	struct obj_maze p3 = { {TYPE_CUBE, 1, 0, 0, 0} };
	int i;
	
	memset(regobjmas, 0, N_OBJECT_TYPES + 1);
	regobjmas[0]++;
	for (i = 0; i < NMAZES; i++){
		GenObject(&p1);
		regobjmas[TYPE_MAZE]++;
	}
	//GenObject(&p2);
	//GenObject(&p3);
}


static int DrawObject(void *p)
{
	int ret = 0;
	struct header *ph = (struct header*)p;
	char *pd_found;
	
	if (!(pd_found = FindDrawn(ph->type, ph->id))) 
		pd_found = pdfree;
	switch(ph->type){
		case TYPE_MAZE:
			if (pd_found == pdfree) 
				FillHeader((struct header *)pd_found, ph, TYPE_MAZE, SYSID_DRAWN);
			DrawMaze((struct drawn_maze *) pd_found);
			break;
		default:
			ret = -1;
			break;
	}
	return ret;
}


static void DrawObjects()
{
	struct drawn_maze s1 = { {TYPE_MAZE, 0, 0, 0, 0}, FLAG_MAZE_VDOOR, 70.0, 5.0, 5.0, 100.0, 0.0, 10.0, 0.0, NULL};
	int i;


	for (i = 0; i < NMAZES; i++){	
		DrawObject(&s1);
		s1.y += 100.0;
		s1.hdr.id++;
	}
}

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

	//humanwalk(-1);

	glTranslatef(0.0, yMov, 0.0); 
	glScalef(scale,scale,scale);
	
	/* Light */
	lightPos[0] += xlMov; lightPos[1] += ylMov;	lightPos[2] += zlMov;
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	/**** HERE WE HAVE INITIAL WORLD COORD SYSTEM *********/
	/* Scene: ground */
	DrawGround();

	/* Scene: maze */
	//DrawMaze(100, (maze*)obj, &mp);
	DrawObjects();

	/* Scene: cube1 */
	glPushMatrix();
	Block(10.0, 10.0, 10.0, 3.0, 85.0, 45.0, &ang);
	glPopMatrix();

	/* Scene: cube2 */
	glPushMatrix();
	glRotatef(ang, 1.0f, 0.0f, 0.0f);
	Block(5.0, 5.0, 5.0, 20.0, 45.0, 40.0, NULL);
	glPopMatrix();

	/* Scene: spheres */
	glPushMatrix();
	glTranslatef(0.0, 400.0, 0.0); 
	DrawSpheres(25);
	glPopMatrix();
	
	/* Ortho pictures */
	setOrtho();
	glPushMatrix();
	glLoadIdentity();
	sprintf (fps, "FPS: %f", floor(1000.0/TIMEOUT1));
	output(10.0, WinH - 30.0, fps);
	Pricel();
	glPopMatrix();
	resetPerspective();


	/* Scene params update */
	ang += 1.0;

	/* checks */
	chk1();


	glPopMatrix();
    glutSwapBuffers();
}

void Cleanup()
{
	free(objects);
	free(drawns);
	free(pscene);
}

int CountObjects(int type)
{
	int i, n = 0;
	struct header *p;
	char *pc;

	for(i = 0, p = (struct header*)objects; p < (struct header*)pofree ; ) {
		if(p->type == type)
			n++;
		pc = (char*)p;
		pc += p->n * p->sz_data_str + p->sz_obj_str;
		p = (struct header*)pc;
	}
	return n;
}

struct header *FindObjByNum(int type, int num)
{
	int i, n = 0;
	struct header *p;
	char *pc;

	for(i = 0, p = (struct header*)objects; p < (struct header*)pofree ; ) {
		if(p->type == type){
			if(n == num)
				break;
			n++;
		}
		pc = (char*)p;
		pc += p->n * p->sz_data_str + p->sz_obj_str;
		p = (struct header*)pc;
	}
	return p;
}


char *GenerateScene(int ptype, int id, int idelem, int ctype) // id=-1 ==All objects of this type
{
	struct scene_obj_hdr *ph = NULL;
	struct header *phead = NULL;
	struct scene_obj *ps = NULL;
	char *pret, *pc;	
	GLfloat coord [3][3] = { {VAXIS/2, VAXIS/3, VAXIS/4},
							{VAXIS/5, VAXIS/2, VAXIS/3},
							{VAXIS/3, VAXIS/4, VAXIS/2}};
	GLfloat rad [1] = {VAXIS/7};
	int i,  k, no = 1, ne = 1;

	switch(ctype){
		case TYPE_SPHERE:
			if(id == -1) {
				ne = 0;
				no = CountObjects(ptype);
				if (idelem == -1){
					for(i = 0; i < no; i++) {
						if ((phead = FindObjByNum(ptype, i))) 
							ne += phead->n;
					}
				} else
					ne = no;
			} 
			ph = realloc(pscene, ne * sizeof(struct scene_obj) + sizeof(struct scene_obj_hdr) + psfree - pscene);
			pret = (char*)ph;
			pc = (char*)ph;
			pc += psfree - pscene;
			ph = (struct scene_obj_hdr *)pc;
			ph->n = ne;
			ph->id = id;
			ph->idelem = idelem;
			ph->ctype = ctype;
			ph->ptype = ptype;
			ph++;
			ps = (struct scene_obj*)ph;
			for(i = 0; i < ne; i++){
				k = rand() % 3;
				ps[i].vx = coord[k][0];
				ps[i].vy = coord[k][1];
				ps[i].vz = coord[k][2];
				ps[i].r = rad[0];
			}
			ps += ne;
			psfree = (char*)ps;
		default:
			break;
	}
	return (char*)pret;
}

void DrawScene(struct header *pho, struct header *phd, int idel)
{
	int t = pho->type;
	struct scene_obj_hdr *p;
	char *pc;
	for(p = (struct scene_obj_hdr *)pscene; p < (struct scene_obj_hdr *)psfree; ){
		switch(t){
			case TYPE_MAZE:
		//		dprintf("OBJ: idel=%d,SCENE: p->id=%d,p->idelem=%d\n",idel,p->id,p->idelem);
				if((pho->id == p->id || p->id == -1) && (idel == p->idelem || p->idelem == -1) && p->ptype == t){
					switch(p->ctype){
						case TYPE_SPHERE:
							DrawSphere(p, pho, phd, idel);
						default:
							break;
					}
				}
				pc = (char*)p;
				pc += sizeof(struct scene_obj) * p->n + sizeof(struct scene_obj_hdr);
				p = (struct scene_obj_hdr*)pc;

				break;
			default:
				break;
		}
	}
}



static void Setup()
{
	/* Features */
	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f );
	srand(123);//time(NULL));
	glEnableClientState(GL_VERTEX_ARRAY);

	/* Light */
	glEnable(GL_LIGHTING);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,whiteLight);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,sourceLight);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPos);
	glEnable(GL_LIGHT0);

	/* Material */
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	
	/* Generating */
	GenerateObjects();
	pscene = GenerateScene(TYPE_MAZE, -1, -1, TYPE_SPHERE);
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
	objects = (char*)calloc(NMAZES * (sizeof(struct obj_maze) + sizeof(struct obj_maze_data) * NMAX_MAZE_PROLETS), 1);
	drawns = (char*)calloc(NMAZES * sizeof(struct drawn_maze), 1);
	szobject = NMAZES * (sizeof(struct obj_maze) + sizeof(struct obj_maze_data) * NMAX_MAZE_PROLETS);
	szdrawns = NMAZES * sizeof(struct drawn_maze);
#ifdef COLOR_DEPTH
	sprintf(mode, "%dx%d:%d", x, y, COLOR_DEPTH);
#else
	sprintf(mode, "%dx%d", x, y);
#endif
	if(!objects || !drawns)
		return -1;
	pofree = objects;
	pdfree = drawns;
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

	Cleanup();
	printf("%s\n",mode);
	return 0;
}
