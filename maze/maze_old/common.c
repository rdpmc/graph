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
GLfloat speedGravi = 0.0, totDY = 0.0;

extern int debug, first, WinH, WinW, iGravi; 
extern GLfloat ang;
extern GLfloat mas1 [NMAX_SPHERES * 4];
extern char *pofree, *pscene, *psfree, *PWC; 	//pointer to the EOF obj
extern GLfloat Zmax, yMov;
extern GLfloat gTime, eyeX, eyeY, eyeZ, lx, ly, lz;
extern char *regobjmas;
/***************************** Maintanance routines ***************/

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
	glColor3f(0.0f, 0.0f, 0.0f);
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

GLfloat chvar1 = 0.5, sign = 1.0;
void chk1()
{
	struct header *po, *pd;
	struct drawn_maze *p2;
	struct dd_header *pdd;
	struct dd_maze_prolet_wcoord *pwc;
	int i,n;
	struct scene_obj_hdr *psh,*psh0;
	struct scene_obj *pso;
#if 0
	GLfloat r = 50.0, x,y,z;
	int j;
/* some experiment code */
	glPushMatrix();
glTranslatef(-100.0, 400.0, 100.0);
	glRotatef(ang, 0.0f, 1.0f, 0.0f);
	glBegin(GL_LINE_STRIP);
	glColor3f(1.0, 1.0, 1.0);
	for(i = 0; i < 360; i+=1){
		for(j = -90; j <= 90; j++){
/*
			y = r * cos(ATOR(j)) * cos(ATOR(i));
			z = r * sin(ATOR(j));
			x = r * cos(ATOR(j)) * sin(ATOR(i)) + z*z/r;
			z*=2;
*/
			x=(GLfloat)i;y=(GLfloat)j;
			z=(GLfloat)sqrt(i*i + j*j)*sin(16*atan(ATOR(i)/ATOR(j)));
			glVertex3f(x, y, z);
		}
	}
	glEnd();
	glPopMatrix();
	chvar1 += sign * 0.02;
	if (chvar1 > 2.0)
		sign = -sign;
	
	if (chvar1 < 0.5)
		sign = -sign;
	return;
#endif
	po = FindObj(TYPE_MAZE, 0);
	pd = FindDrawn(TYPE_MAZE, 0);
	if(!po || !pd)
		return;
	p2 = (struct drawn_maze *) pd;
	pdd = p2->ddata;
	pdd++;
	pwc = (struct dd_maze_prolet_wcoord*)pdd;
	glColor3f(0.0f, 1.0, 0.0f);
	for(i=0;i<po->n;i++){
dprintf("i=%d wc A:%f %f %f, D:%f %f %f \n",i, pwc[i].A[X], pwc[i].A[Y], pwc[i].A[Z],pwc[i].D[X], pwc[i].D[Y], pwc[i].D[Z]);
	
		glPushMatrix();
		glTranslatef(pwc[i].A[X], pwc[i].A[Y], pwc[i].A[Z]);
		glutWireSphere(10, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(pwc[i].B[X], pwc[i].B[Y], pwc[i].B[Z]);
		glutWireSphere(10, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(pwc[i].C[X], pwc[i].C[Y], pwc[i].C[Z]);
		glutWireSphere(10, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(pwc[i].D[X], pwc[i].D[Y], pwc[i].D[Z]);
		glutWireSphere(10, 20, 20);
		glPopMatrix();
	}
	psh = (struct scene_obj_hdr*)pscene;
	for(;(char*)psh < psfree;){
		n=psh->n;
		psh0 = psh;
		psh++;
		pso = (struct scene_obj*)psh;
		for(i=0;i<n; i++){
			dprintf("%d-th obj(total %d) %d in Obj %d(idObj %d, idElem %d), obj wc: %f %f %f\n", 
				i,n, psh0->ctype, psh0->ptype, psh0->id, psh0->idelem,
				pso[i].wcc[X],	pso[i].wcc[Y], pso[i].wcc[Z]);
				glColor3f(1.0f, 1.0, 1.0f);
				glPushMatrix();
				glTranslatef(pso[i].wcc[X],	pso[i].wcc[Y], pso[i].wcc[Z]);
				glutWireSphere(5, 10, 10);
				glPopMatrix();
				
		}
		pso += psh0->n;
		psh = (struct scene_obj_hdr*)pso;
	}


}


/******************************** Other computational ************/
//in: i move from vx0 to vx1
// out: 1 if valid, 0 if not.
bool IsEyeMoveValid(Vx vx0, Vx vx1)
{
	struct drawn_maze *pd;
	struct dd_header *pdd;
	struct dd_maze_prolet_wcoord *pwc;	
	struct prim4 *p4;
//	struct prim3 *p3;
	int npr;
	Vx vxt[4];
	bool decision = true;
	Vx vxl;

	mVector(vxl, vx1, vx0);
	
	for(i = 0; i < regobjmas[0]; i++) {			//foreach type
		for(j = 0; j < regobjmas[i]; j++) {		// foreach obj of type
			if(!(pd = FindDrawn(i, j)))
				continue;
			if(vx0[Y] < pd->y || vx0[Y] > (pd->y + pd->h2))
				continue;
			pdd = pd->ddata;
			n = pdd->nfilled;
			pdd++;
			pwc = (struct dd_maze_prolet_wcoord *)pdd;
			for(k = 0; k < n; k++, pwc++) { //find prolet
				if(pwc->ugol == MAZE_L) {
					VxMov(vxt[0], (pwc - 1)->C); VxMov(vxt[1], pwc->E);
					VxMov(vxt[2], pwc->C); VxMov(vxt[3], pwc->D);
					npoly = 8;
				} else if(pwc->ugol == MAZE_R) {
					VxMov(vxt[1], pwc->E); VxMov(vxt[0], (pwc - 1)->D); 
					VxMov(vxt[2], pwc->C); VxMov(vxt[3], pwc->D);
				} else {
					VxMov(vxt[0], pwc->A); VxMov(vxt[1], pwc->B);
					VxMov(vxt[2], pwc->C); VxMov(vxt[3], pwc->D);
				}
				for(l = 0; l < 3; vxt[k][Y] = vx0[Y], l++);
				if(mIsPointInsidePolygon(vx[0], vxt, pwc->nedges)) {
					npr = pwc->nblocks * pwc->nprims;
					p4 = pwc->pprims;			
					for(l = 0; l < npr; l++, p4++){
						if(mIsLineIntersectPolygon(p4->wc, vxl, pwc->nedges)) {
							decision = false;
							goto out;
						}
					}
				}
			}
		}
	}
	out:	

	return decision;
}

/******************************** Actions ************************/
void humanwalk(int dir)
{
	int chastota = 2.0;

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


void shoot()
{
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
	Vx vx0 = {eyeX, eyeY, eyeZ}, vx1;

	if(!iGravi) {
		timeNoGravi += TIMEOUT1;
		return;
	}
	t = (GLfloat)TIMEOUT1 / 1000.0; //delta_t (sec)
	dy = t * t * GRAVI_CONST / 2 + speedGravi * t;	
	speedGravi += GRAVI_CONST * t;
	
	totDY += dy;
	VxMov(vx1, vx0);
	VxAdd(vx1, 0.0, -dy, 0.0);
	if(IsEyeMoveValid(vx0, vx1)){
		eyeY += -dy;
	}else{
		speedGravi = 0;
		totDY=0;
	}

	
	dprintf("GRAVI t =%f dy = %f v = %f totDY=%f yMov=%f\n", gTime/1000.0, dy, speedGravi, totDY,yMov);
}

/******************************** Simple objects drawing (along Z+) ******************/


/* Block */
void Block(GLfloat dxB, GLfloat dyB, GLfloat dzB, GLfloat xB, GLfloat yB,
		    GLfloat zB, GLfloat *pang, struct drawn_maze *p, int n)
{

	int sz, iwc, ind;
	struct prim4 *ppr;
	GLfloat xA, yA, zA; 
	Vx bmas [] = {{0.0,0.0,0.0}, {0.0,1.0,0.0}, {0.0,1.0,1.0}, {0.0,0.0,1.0},
		      {1.0,0.0,0.0}, {1.0,1.0,0.0}, {1.0,1.0,1.0}, {1.0,0.0,1.0}};
	GLubyte imas [] = {0,3,2,1,  0,4,7,3,  4,5,6,7, 1,2,6,5, 2,3,7,6, 0,1,5,4};
	Vx vx;	
			   
	glTranslatef(xB, yB, zB); 
	if(pang)
		glRotatef(*pang, 1.0f, 0.0f, 1.0f);

	glScalef(dxB, dyB, dzB);

	pdh = p->ddata;
	iwc = pdh->iready;
	pdh++;
	pwco = (struct dd_maze_prolet_wcoord*)pdh;
	pwco += n;
	xA = pwco->A[X]; yA = pwco->A[Y]; zA = pwco->A[Z];
	if(iwc) {
		if(!pwco->pprims){
			sz = pwco->ugol != MAZE_F ? 4 + 4 : 4;  
			pwco->pprims = Malloc(sz * pwco->nprims * sizeof(struct prim4));
		}
		ppr = pwco->pprims;
		ppr += pwco->nblocks * pwco->nprims;
		/* WC */
		for(i = 0; i < pwco->nprims; i++){
			for(j = 0; j < pwco->nedges; j++){
				ind = imas[j + i*4];
				VxMov(vx,bmas[ind]);
				ppr[i].wc[j][X] = xA + xB + vx[X]*dxB;
				ppr[i].wc[j][Y] = yA + yB + vx[Y]*dyB;
				ppr[i].wc[j][Z] = zA + zB + vx[Z]*dzB;
			}
			/* Normal */
			mGetNormal(ppr[i].norm, ppr[i].wc); //equ: Ax+By+Cz+D=0, D=-(Ax0+By0+Cz0); N{A,B,C} - normal
			/* Equation */
			//mGetPlaneEqu(ppr[i].equ, ppr[i].wc);
		}
	}
	glVertexPointer(3, GL_FLOAT, 0, bmas);

	glColor3f(0.0f, 1.0f, 0.0f);
	glDrawElemets(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 0);
	glColor3f(1.0f, 0.0f, 0.0f);
	glDrawElemets(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 1);
	glColor3f(0.0f, 0.0f, 1.0f);
	glDrawElemets(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 2);
	glColor3f(0.0f, 1.0f, 1.0f);
	glDrawElemets(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 3);
	glColor3f(1.0f, 0.0f, 1.0f);
	glDrawElemets(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 4);
	glColor3f(1.0f, 1.0f, 1.0f);
	glDrawElemets(GL_QUADS, 4, GL_UNSIGNED_BYTE, imas + 4 * 5);
/*
	glBegin(GL_QUADS);
	//lft
	glColor3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,1.0,0.0);
	glVertex3f(0.0,1.0,1.0);glVertex3f(0.0,0.0,1.0);
	
	//bot
	glColor3f(1.0f, 0.0f, 0.0f); glVertex3f(1.0,0.0,0.0);glVertex3f(0.0,0.0,0.0);
	glVertex3f(0.0,0.0,1.0);glVertex3f(1.0,0.0,1.0);

	//rite
	glColor3f(0.0f, 0.0f, 1.0f);glVertex3f(1.0,1.0,1.0);glVertex3f(1.0,1.0,0.0);
	glVertex3f(1.0,0.0,0.0);glVertex3f(1.0,0.0,1.0);

	//top
	glColor3f(0.0f, 1.0f, 1.0f);glVertex3f(1.0,1.0,0.0);glVertex3f(1.0,1.0,1.0);
	glVertex3f(0.0,1.0,1.0);glVertex3f(0.0,1.0,0.0);

	//frn
	glColor3f(1.0f, 0.0f, 1.0f);glVertex3f(1.0,1.0,0.0);glVertex3f(1.0,0.0,0.0);
	glVertex3f(0.0,0.0,0.0);glVertex3f(0.0,1.0,0.0);

	//bck
	glColor3f(1.0f, 1.0f, 1.0f);glVertex3f(1.0,1.0,1.0);glVertex3f(0.0,1.0,1.0);
	glVertex3f(0.0,0.0,1.0);glVertex3f(1.0,0.0,1.0);

	glEnd();
*/
}

void DrawSphere(struct scene_obj_hdr *phs, struct header *pho, struct header *phd, int idel)
{
	GLfloat len, w, h, w2, h2 ,xm, ym, zm, maxr, minr, dx, dy, dz, x, y, z;
	GLfloat vx,vy,vz, r;
	GLfloat rotang;
	struct scene_obj *ps = (struct scene_obj*)phs;
	struct obj_maze_data *pod;
	struct drawn_maze *pd;
	int n;
	char *pc;
	struct dd_header *pddh;
	struct dd_maze_prolet_wcoord *pwc;

	pd = (struct drawn_maze*)phd;
	pddh = pd->ddata;
	glPushMatrix();
	switch(phs->ptype){
		case TYPE_MAZE:
			if(phs->idelem == -1){
				pc = (char*)ps;
				pc += pho->n * pho->id * sizeof(struct scene_obj) + sizeof(struct scene_obj_hdr) + idel * sizeof(struct scene_obj);
				ps = (struct scene_obj*)pc;
			}else {
				pc = (char*)ps;
				pc += pho->n * pho->id * sizeof(struct scene_obj) + sizeof(struct scene_obj_hdr);
				ps = (struct scene_obj*)pc;
			}
			/* Translate virtual coord to area(prolet) coord */	
			vx = ps->vx;
			vy = ps->vy;
			vz = ps->vz;
			r = ps->r;

			n = pho->n;
			pod = (struct obj_maze_data*)pho;
			pc = (char*)pod;
			pc += pho->sz_obj_str + idel * pho->sz_data_str;
			pod = (struct obj_maze_data*)pc;
			len = pod->len;
			w = pd->w;
			h = pd->h;
			w2 = pd->w2;
			h2 = pd->h2;
			xm = pd->x;
			ym = pd->y;
			zm = pd->z;
			x = w * vx / VAXIS;
			y = h2 * vy / VAXIS;
			z = len * vz / VAXIS;
			MIN(minr, len, w, h2);
			minr = minr / 10;
			x = x < w2 ? x + w2 + minr: x;
			x = x > w - w2 ? w - w2 - minr: x;
			y = y < h ? y + h + minr: y;
			y = y > h2 - h ? h2 - h - minr: y;
			z = z > len - minr ? len - minr: z;
			z = z < minr ? minr: z;
			dx = fabs(x - w + w2) > fabs(x - w2) ? fabs(x - w2) : fabs(x - w + w2);
			dy = fabs(y - h2 + h) > fabs(y - h) ? fabs(y - h) : fabs(y - h2 + h);
			dz = fabs(z) > fabs(z - len) ? fabs(z - len) : fabs(z);
			maxr = dx > dy ? dy : dx;
			maxr = maxr > dz ? dz : maxr;
//	r = len > w ? rand() % (int)floor(w) : rand() % (int)floor(len);
//	r = r / 3; 
			r = r > maxr ? maxr : r;
			glColor3f(0.0f, 0.0f, 0.0f);
			glTranslatef(x +xm, y + ym, z + zm);
			glRotatef(ang, rand(), rand(), rand());
			glutWireSphere(r, 20, 20);

			if(pddh->iready == 1)
				break;
			/* WC of center save */
			rotang = pddh->rotang;
			pddh++;
			pwc = (struct dd_maze_prolet_wcoord *)pddh;
			pwc += idel;
			VxMov(ps->wcc, pwc->A);
			_VxVirtualAdd(ps->wcc, rotang, x, y, z);
			dprintf("SPHERE %d: x:%f y:%f z:%f\n", idel, ps->wcc[X],ps->wcc[Y],ps->wcc[Z]);
			break;
		default:
			break;
	}
	glPopMatrix();
}

/********************************** Complex objects ********************/

/* Ground :) *******************/
void DrawGround()
{
	GLfloat dxGrnd = Zmax / 300, xGrnd = Zmax, yGrnd= -30.0, i;
	struct wcoord wc = {TYPE_BLOCK, 0, 4, NULL};
	Vx coord [4] = { {-xGrnd, yGrnd, -xGrnd }, {xGrnd, yGrnd, -xGrnd },
				   {xGrnd, yGrnd, xGrnd }, {-xGrnd, yGrnd, xGrnd } };

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
/*
	wc.pwc = (Vx*) Malloc(sizeof(coord));
	memcpy (wc.pwc, coord, sizeof(coord));
	Memcpy(&PWC, (void*)&wc, sizeof(wc));
*/
}

/* Spheres ***********************/
int DrawSpheres(int n)
{
	GLfloat znak1, znak2, maxR;
	int i, max;

	if(n > NMAX_SPHERES)
		return -1;
	if(!first){
		for(i = 0; i < n; i++){
			mas1[i * 4] = rand() % 30;
			znak1 = (rand() > 32000) ? -1.0 : 1.0;
			znak2 = (rand() > 32000) ? -1.0 : 1.0;
			mas1[i*4 + 1] = znak1 * (rand() % 400);
			mas1[i*4 + 3] = znak2 * (rand() % 400);
			mas1[i*4 + 2] = rand() % 30;
			first=1;
		}
	}
	glColor3f(1.0, 0.0, 0.0);
	maxR = mas1[0];
	for(i = 0; i < n; i++){
		glPushMatrix();
		glTranslatef(mas1[i*4 + 1], mas1[i*4 + 2], mas1[i*4 + 3]);
		glRotatef(ang, mas1[i*4 + 2], mas1[i*4 + 3], mas1[i*4 + 1]);
		glutWireSphere(mas1[i*4], 30, 30);
		glPopMatrix();
		if(mas1[i*4] > maxR)
			max = i;

	}
	glPushMatrix();
		glTranslatef(mas1[max*4 + 1], mas1[max*4 + 2], mas1[max*4 + 3]);
		glPushMatrix();
			glColor3f(0.0, 0.0, 0.0);
			glRotatef(ang, 0.0, 0.0, 1.0);
			glTranslatef(mas1[max*4] / 4, 0.0, 0.0);
			glutSolidSphere(mas1[max*4] / 15, 30, 30);
		glPopMatrix();
		glRotatef(ang, mas1[max*4 + 2], mas1[max*4 + 3], mas1[max*4 + 1]);
		glColor3f(0.0, 1.0, 0.0);
		glutWireSphere(mas1[max * 4] / 5, 30, 30);
	glPopMatrix();
	return 0;
}

/* Maze *******************************/
int GenMaze()
{
	int i, d, l, prevd=-1, r, n;
	struct header *ph = (struct header *)pofree;
	struct obj_maze_data *p;

	/* obj params */
	n = ph->n;

	/* obj data */
	pofree += ph->sz_obj_str;
	p = (struct obj_maze_data*)pofree;

	if(n > NMAX_MAZE_PROLETS)
		return -1;
	
//	p = (struct obj_maze_data*) CheckCoreBuffersSz(SYSID_OBJECT, ph->sz_data_str * n);
	p = (struct obj_maze_data*) pofree;
	for(i = 0; i < n; i++){
		l = rand() % 500;
		l = (l > 50) ? l : 50;
		d = rand() % 2;
		d = (!d) ? MAZE_L : MAZE_R;
		r = rand() % 3;
		if(prevd == d && r < 1) //33% 
			d = (d == MAZE_L) ? MAZE_R : MAZE_L;
		if(!i)
			p[i].dir = MAZE_F;
		else 
			p[i].dir = d;
		p[i].len = l;
		prevd = d;

		pofree += ph->sz_data_str;
	}
	return 0;
}

static void _DrawProlet(struct drawn_maze *p, GLfloat l, int n)
{
	GLfloat w = p->w, h = p->h, w2 = p->w2, h2 = p->h2, z = p->z, y = p->y, x = p->x;

	glPushMatrix();
	Block(w, h, l, x, y + h2 - h, z, NULL, p, n); //top
	glPopMatrix();
	glPushMatrix();
	Block(w, h, l, x, y, z, NULL); //bot
	glPopMatrix();
	glPushMatrix();
	Block(w2, h2, l, x, y, z, NULL); //lft
	glPopMatrix();
	glPushMatrix();
	Block(w2, h2, l, x + w - w2, y, z, NULL); //rit
	glPopMatrix();
}

static void _DrawProletUgol(struct drawn_maze *p, int dir, int n)
{
	GLfloat w = p->w, h = p->h, w2 = p->w2, h2 = p->h2, z = p->z, y = p->y, x = p->x;

	if(dir == MAZE_L){
		glPushMatrix();
		Block(w, h, w, x, y + h2 - h, z - w, NULL); //roof
		glPopMatrix();
		glPushMatrix();
		Block(w, h, w, x, y, z - w, NULL); //floor
		glPopMatrix();
		glPushMatrix();
		Block(w, h2, w2, x, y, z - w, NULL); //front side
		glPopMatrix();
		glPushMatrix();
		Block(w2, h2, w, x+ w - w2, y, z - w, NULL); //right side
		glPopMatrix();
	} else if (dir == MAZE_R) {
		glPushMatrix();
		Block(w, h, w, x, y + h2 - h, z - w, NULL); //roof
		glPopMatrix();
		glPushMatrix();
		Block(w, h, w, x, y, z - w, NULL); //floor
		glPopMatrix();
		glPushMatrix();
		Block(w, h2, w2, x, y, z - w, NULL); //front side
		glPopMatrix();
		glPushMatrix();
		Block(w2, h2, w, x, y, z - w, NULL); //left side
		glPopMatrix();
	}
}


int _Maze_SaveWorldCoords(struct header *pho, struct header *phd, int i)
{
	struct obj_maze_data *pomd;
	struct drawn_maze *p = (struct drawn_maze *)phd;
	GLfloat w = p->w, z0 = p->z, y0 = p->y, x0 = p->x;
	int dir, len;
	struct dd_header *pddh;
	struct dd_maze_prolet_wcoord *pwc, *pwcold;
	Vx vx0;
	char *pc;
	GLfloat rotang;

	/* no update wc */
	pomd = (struct obj_maze_data*)(&pho[1]);
	len = pomd[i].len;
	dir = pomd[i].dir;
	if (!p->ddata) {
		if(!(p->ddata = Malloc(sizeof(struct dd_maze_prolet_wcoord) * pho->n +sizeof(struct dd_header))))
			return -1;			
		pddh = p->ddata;
		pddh->type = WORLD_COORDS; //why it needs?? don't know yet
		pddh->nfilled = 0;
		pddh->iready = 0;
		pddh->sz_hdr_str = sizeof(struct dd_header);
		pddh->sz_data_str = sizeof(struct dd_maze_prolet_wcoord);
	}
	pddh = p->ddata;
	if(pddh->nfilled == pho->n){ 
		pddh->iready = 1;
		return 0;
	}
	pc = (char*)pddh;
	pc += sizeof(struct dd_header) + sizeof(struct dd_maze_prolet_wcoord) * pddh->nfilled;
	pwc = (struct dd_maze_prolet_wcoord*) pc;
	pwcold = pwc - 1;

	pwc->ugol = MAZE_UN;
	pwc->E[X] = 0.0, pwc->E[Y] = 0.0, pwc->E[Z] = 0.0;
	if (!i) {
		vx0[X] = x0; vx0[Y] = y0; vx0[Z] = z0;
		pddh->rotang = 0.0;
	} else {
		vx0[X] = pwcold->D[X]; vx0[Y] = pwcold->D[Y]; vx0[Z] = pwcold->D[Z];
	}
	rotang = pddh->rotang;
	switch(dir) {
		case MAZE_F:
			VxMov(pwc->A, vx0);

			_VxVirtualAdd(vx0, rotang, w, 0.0, 0.0);
			VxMov(pwc->B, vx0);

			_VxVirtualAdd(vx0, rotang, 0.0, 0.0, len);
			VxMov(pwc->C, vx0);

			_VxVirtualAdd(vx0, rotang, -w, 0.0, 0.0);
			VxMov(pwc->D, vx0);
			break;

		case MAZE_L:
			pddh->rotang += ATOR(270.0);
			VxMov(pwc->A, vx0);
			
			_VxVirtualAdd(vx0, rotang, 0.0, 0.0, w);
			VxMov(pwc->B, vx0);

			_VxVirtualAdd(vx0, rotang, -len, 0.0, 0.0);
			VxMov(pwc->C, vx0);

			_VxVirtualAdd(vx0, rotang, 0.0, 0.0, -w);
			VxMov(pwc->D, vx0);
			if(i) {
				pwc->ugol = MAZE_L;
				VxMov(vx0, pwc->B); 
				_VxVirtualAdd(vx0, rotang, w, 0.0, 0.0);
				VxMov(pwc->E, vx0);
			}
			break;
				
		case MAZE_R:
			pddh->rotang += ATOR(90.0);
			
			_VxVirtualAdd(vx0, rotang, w, 0.0, w);
			VxMov(pwc->A, vx0);
			
			_VxVirtualAdd(vx0, rotang, 0.0, 0.0, -w);
			VxMov(pwc->B, vx0);
			
			_VxVirtualAdd(vx0, rotang, len, 0.0, 0.0);
			VxMov(pwc->C, vx0);
			
			_VxVirtualAdd(vx0, rotang, 0.0, 0.0, w);
			VxMov(pwc->D, vx0);
			if(i) {
				pwc->ugol = MAZE_R;
				VxMov(vx0, pwc->A); 
				_VxVirtualAdd(vx0, rotang, -w, 0.0, 0.0);
				VxMov(pwc->E, vx0);
			}
			break;
		default:
			break;
	}
	pddh->nfilled++;
	/* for Block() save wc */
	pddh->nedges = 4; //4 edges in primitive
	pddh->nprims = 6;	// 6 prims in block
	pddh->nblocks = 0; 	   //n blocks (inc by Block())
	pddh->pprims = NULL;
	
	/* Update wc */
	/*
	if(pddh->nfilled == pho->n)
		pddh->nfilled = 0;
	*/
	return 0;
}

int DrawMaze(struct drawn_maze *p)
{
	int i;
	struct obj_maze *pom;
	struct header *phd = (struct header*)p;
	struct header *pho;
	struct obj_maze_data *pomd;
	GLfloat x0, y0, z0;

	if (!(pom = FindObj(phd->type, phd->id))) {
		dprintf("Can not find object ID %d of type %d\n", phd->id, phd->type);
		return -1;
	}
	pho = (struct header*)pom;

	pomd = (struct obj_maze_data*)(&pho[1]);

	glPushMatrix();
	
	/* Initial (x,y,z) in world view */
	x0 = p->x; y0 = p->y; z0 = p->z;

	for(i = 0;  i < pho->n; i++){
		/* Do funcs */
		if(_Maze_SaveWorldCoords(pho, phd, i) < 0)
			return -1;
		if(!i) {
			switch(pomd[i].dir) {
				case MAZE_F:
					_DrawProlet(p, pomd[i].len, i);
					break;
				case MAZE_L:
					glRotatef(270.0, 0.0, 1.0, 0.0);
					_DrawProlet(p, pomd[i].len, i);
					break;
				case MAZE_R:
					glRotatef(90.0, 0.0, 1.0, 0.0);
					_DrawProlet(p, pomd[i].len, i);
					break;
				default:
					dprintf("Error\n");
					break;
			}
		}else { //prevdir always=F   
			p->x = 0.0;
			p->y = 0.0;
			p->z = 0.0;
			switch(pomd[i].dir) {
				case MAZE_F:
					_DrawProlet(p, pomd[i].len, i);
					break;
				case MAZE_L:
					glRotatef(270.0, 0.0, 1.0, 0.0);
					_DrawProlet(p, pomd[i].len, i);
					_DrawProletUgol(p, MAZE_L, i);
					break;
				case MAZE_R:
					glTranslatef(p->w, 0.0, p->w);
					glRotatef(90.0, 0.0, 1.0, 0.0);
					_DrawProlet(p, pomd[i].len, i);
					_DrawProletUgol(p, MAZE_R, i);
					break;
				default:
					dprintf("Error\n");
					break;
			}
		}
		DrawScene(pho, phd, i);
		glTranslatef(p->x, p->y, p->z + pomd[i].len);
	}
	glPopMatrix();
	p->x = x0;
	p->y = y0;
	p->z = z0;
	return 0;

}

void DrawMazeDoor()
{

}


