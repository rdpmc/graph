#include "include.h"

extern int iCull, iDepth, iOutline;
extern int hwin, fullscreen,debug;
extern GLfloat gTime;
extern int keys [4];

extern GLsizei WinH, WinW;
extern GLfloat Xmax, Ymax, Zmax;
extern GLfloat dxMov, dzMov, dyMov, dScale, scale, yMov;
extern GLfloat xlMov, ylMov, zlMov;
extern GLfloat	eyeX, eyeY, eyeZ, lx, ly, lz;
extern GLfloat angle, dxmNold, dymNold, angleY; 
extern int xm, ym, Xequ0, XequW, Yequ0, YequH;
extern GLfloat dxm, dym;
extern GLfloat speedGravi;


void Idle(void)
{
//	struct timespec ts= {0,100000000};
//	nanosleep(NULL,&ts);
//	glutPostRedisplay();
	return;
}

static void moveMeFlat(int direction) {
	GLfloat speed = 6.0;
	
	speedGravi += 0.0;
	eyeX = eyeX + direction * lx * speed;
	eyeY = eyeY + direction * ly * speed;
	eyeZ = eyeZ + direction * lz * speed;
}

static void orientMe(float shift) {
	GLfloat k, k2, svob, a, a2, dz, dx;
	GLfloat speed = 3.0;
	int sign;

	speedGravi += 0.0;
	if(lz >= 0)
		sign = 1;
	else
		sign = -1;

	k = lz / lx; // z=kx+svob
	k2 = -1 / k;
	svob = eyeZ - k2 * eyeX;
	a = atan(k2);
	if(shift > 0) {
		if(a < 0){
			a = M_PI + a;
			a2 = M_PI - a;
			dz = shift * fabs(sin(a2));
			dx = -shift * fabs(cos(a2)); //NW
		}else {
			a2 = a;
			dz = -shift * fabs(sin(a2));
			dx = -shift * fabs(cos(a2)); //NE
		}
	}else{
		if(a < 0){
			a = M_PI + a;
			a2 = M_PI - a;
			dz = -shift * fabs(sin(a2)); 
			dx = shift * fabs(cos(a2)); //NW
		}else {
			a2 = a;
			dz = shift * fabs(sin(a2)); 
			dx = shift * fabs(cos(a2)); //NE
		}
	}
	eyeX += dx * shift * sign * speed;
	eyeZ += dz * shift * sign * speed;
}


void Timer(int val)
{
	gTime += (GLfloat)TIMEOUT1;

	if(val == ID_TIMER1){
		glutTimerFunc(TIMEOUT1, Timer, ID_TIMER1);
	}
	if(Xequ0 > 2) {
//		angle += dxmNold;
		if(fabs(dxmNold) < ATOR(1))
			dxmNold = -ATOR(1);
	}
	if(XequW > 2) {
//		angle += dxmNold;
		if(fabs(dxmNold) < ATOR(1))
			dxmNold = ATOR(1);
	}

	if(Yequ0 > 2) {
		if(fabs(dymNold) < ATOR(1))
			dymNold = -ATOR(1);
	}

	if(YequH>2) {
		if(fabs(dymNold) < ATOR(1))
			dymNold = ATOR(1);
	}
/*	if(angle < 0 )
		angle = ATOR(1);
	if(angle > M_PI )
		angle = M_PI - ATOR(1);
*/

	if(angleY < 0)
		angleY = ATOR(1);
	if(angleY > M_PI )
		angleY = M_PI - ATOR(1);

//dprintf(" aY = %f, a = %f\n", RTOA(angleY),RTOA(angle));


	lx = sin(angleY)*cos(angle);
	ly = cos(angleY); //-
	lz = sin(angleY)*sin(angle);
//dprintf("lx = %f, ly = %f,lz=%f\n", lx,ly,lz);
	if(keys[KEY_L] == 1){
		orientMe(-DELTA_MOVE_LR);
	}
	if(keys[KEY_R] == 1){
		orientMe(DELTA_MOVE_LR);
	}
	if (keys[KEY_U] == 1){
		moveMeFlat(DELTA_MOVE_FB);
	}
	if(keys[KEY_D] == 1) {
		moveMeFlat(-DELTA_MOVE_FB);
	}
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + lx, eyeY + ly, eyeZ + lz, 0.0f, 1.0f, 0.0f);
	glutPostRedisplay();
}

void ProcessMenu(int value)
{
	switch(value)
		{
		case 1:
			iDepth = !iDepth;
			break;
		case 2:
			iCull = !iCull;
			break;
		case 3:
			iOutline = !iOutline;
		default:
			break;
		}
}

void Mouse(int button, int state,int x, int y)
{
	switch(button) {
		case GLUT_LEFT_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		default:
			break;
	}
}


void MouseMove(int x, int y)
{
	
	 GLfloat dxmN, dymN, speed = 2.0;

	if(xm != -12345) {
		dxm = x - xm;
		dym = y - ym;
	}
	xm = x;
	ym = y;

	/** XOZ **/
	dxmN = dxm / WinW;
	dxmN *= speed;
	dxmNold = (dxmN) ? dxmN : dxmNold;
	dymN = dym / WinH;
	dymN *= speed;
	dymNold = (dymN) ? dymN : dymNold;
	/* LEFT */
	if(x < 1) {
		Xequ0++;
		if(Xequ0 > 2)
			dxmN = dxmNold;
	} else if (x >= WinW - 1) {
	/* RIGHT */
		XequW++;
		if(XequW > 2)
			dxmN = dxmNold;
	} else {
		Xequ0 = 0;
		XequW = 0;
	}

	/** YOZ **/
	
	/* UP */
	if(y < 1) {
		Yequ0++;
		if(Yequ0 > 2)
			dymN = dymNold;
	} else if (y >= WinH - 1) {
		YequH++;
		if(YequH > 2)
			dymN = dymNold;
	} else {
		Yequ0 = 0;
		YequH = 0;
	}
	angle += dxmNold;
	angleY += dymN;

}

void SpecKeyUp(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT : 
			keys[KEY_L] = 0; break;
		case GLUT_KEY_RIGHT : 
			keys[KEY_R] = 0; break;
		case GLUT_KEY_UP : 
			humanwalk(KEY_U);
			keys[KEY_U] = 0; break;
		case GLUT_KEY_DOWN : 
			humanwalk(KEY_D);
			keys[KEY_D] = 0; break;
		default:
			break;
	}
}

void SpecKey(int key, int x, int y)
{
	switch(key) {
		case GLUT_KEY_LEFT : 
			keys[KEY_L] = 1; break;
		case GLUT_KEY_RIGHT : 
			keys[KEY_R] = 1; break;
		case GLUT_KEY_UP : 
			humanwalk(KEY_U);
			keys[KEY_U] = 1; break;
		case GLUT_KEY_DOWN : 
			humanwalk(KEY_D);
			keys[KEY_D] = 1; break;
		default:
			break;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	if(key == 0x1b){
		if(fullscreen){
			exit(0);
		}else{
			glutDestroyWindow(hwin);
		}
		return;
	}
	switch(key){

		/* Light mpve */
		case 'w':
			ylMov += dyMov;
			break;
		case 's':
			ylMov -= dyMov;
			break;
		case 'a':
			xlMov -= dxMov;
			break;
		case 'd':
			xlMov += dxMov;
			break;
		case 'r':
			zlMov += dzMov;
			break;
		case 't':
			zlMov -= dzMov;
			break;
		
		/* Up/down */
		case 'q':
			yMov += dyMov;
			break;
		case 'e':
			yMov -= dyMov;
			break;

		/* Scale */
		case 'z':
			scale -= dScale;
			scale = (scale < 0) ? 0.0 : scale;
			break;
		case 'x':
			scale += dScale;
			break;
		default:
			break;
	}
}


void Resize3d(GLsizei w, GLsizei h)
{
	GLfloat nRange = EKR;
	GLfloat aspect;

	if(fullscreen) {
		w = WinW; 
		h = WinH;
	}
	WinW = w;
	WinH = h;
	if(h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	aspect = w / h;
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FANG, aspect, ZNEAR, ZFAR);
	
	if (w <= h) {
		Xmax = nRange; 
		Ymax = nRange * h / w;
	} else {
		Ymax = nRange; 
		Xmax = nRange * w / h;
	}
	Zmax = ZFAR;
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eyeX, eyeY, eyeZ, eyeX + lx,eyeY + ly,eyeZ + lz,0.0f,1.0f,0.0f);
}



