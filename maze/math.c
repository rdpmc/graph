#include "include.h"

void VxAdd(Vx v, GLfloat x, GLfloat y, GLfloat z)
{
	v[X] += x;	v[Y] += y;	v[Z] += z;
}

void VxMov(Vx d, Vx s)
{
	d[X] = s[X]; d[Y] = s[Y]; d[Z] = s[Z];	
}

void LoadIdentity44(Matrix44 m)
{
	static Matrix44	identity = { 1.0f, 0.0f, 0.0f, 0.0f,
								 0.0f, 1.0f, 0.0f, 0.0f,
								 0.0f, 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 0.0f, 1.0f };

	memcpy(m, identity, sizeof(Matrix44));
}

void LoadIdentity33(Matrix33 m)
{
	static Matrix33	identity = { 1.0f, 0.0f, 0.0f ,
								 0.0f, 1.0f, 0.0f,
								 0.0f, 0.0f, 1.0f };

	memcpy(m, identity, sizeof(Matrix33));
}

static void DoTranslationMatrix(Matrix44 m, GLfloat x, GLfloat y, GLfloat z)
{ 
	LoadIdentity44(m); 
	m[12] = x; m[13] = y; m[14] = z; 
}

static void DoRotationMatrix(Matrix33 m, GLfloat a, GLfloat x, GLfloat y, GLfloat z)
{
	
	GLfloat mag, s, c;
	GLfloat xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sin(a);
	c = cos(a);

	mag = sqrt( x*x + y*y + z*z );

	// Identity matrix
	if (mag == 0.0f) {
		LoadIdentity33(m);
		return;
	}

	// Rotation matrix is normalized
	x /= mag; y /= mag; z /= mag;

	xx = x * x;	yy = y * y;	zz = z * z;	
	xy = x * y;	yz = y * z;	zx = z * x;	
	xs = x * s;	ys = y * s;	zs = z * s;

	one_c = 1.0f - c;

	M33(0,0) = (one_c * xx) + c;
	M33(0,1) = (one_c * xy) - zs;
	M33(0,2) = (one_c * zx) + ys;

	M33(1,0) = (one_c * xy) + zs;
	M33(1,1) = (one_c * yy) + c;
	M33(1,2) = (one_c * yz) - xs;

	M33(2,0) = (one_c * zx) - ys;
	M33(2,1) = (one_c * yz) + xs;
	M33(2,2) = (one_c * zz) + c;
}


void VxRotate(Vx vOut, Vx p, GLfloat a, Vx axis)
{
	Matrix33 m;

	DoRotationMatrix(m, -a, axis[X], axis[Y], axis[Z]); //-a =>CCW
    vOut[0] = m[0] * p[0] + m[3] * p[1] + m[6] * p[2];	
    vOut[1] = m[1] * p[0] + m[4] * p[1] + m[7] * p[2];	
    vOut[2] = m[2] * p[0] + m[5] * p[1] + m[8] * p[2];	
}

void VxTranslate(Vx vOut, Vx v, GLfloat x, GLfloat y, GLfloat z)
{
	Matrix44 m;

	DoTranslationMatrix(m, x, y, z);
    vOut[0] = m[0] * v[0] + m[4] * v[1] + m[8] *  v[2] + m[12]; 
    vOut[1] = m[1] * v[0] + m[5] * v[1] + m[9] *  v[2] + m[13];	
    vOut[2] = m[2] * v[0] + m[6] * v[1] + m[10] * v[2] + m[14];	
}


void _VxVirtualAdd(Vx vx, GLfloat rotang, GLfloat x, GLfloat y, GLfloat z)
{
	Vx vx1;
	Vx aY = {0.0, 1.0, 0.0};

	VxRotate(vx1, vx, rotang, aY); VxAdd(vx1, x, y, z); VxRotate(vx, vx1, -rotang, aY);
}

/* New math functions start with "m" */

/* Get vektornoe proizvedenie */
void mCross(Vx vres, Vx v1, Vx v2)
{
	vres[X] = (v1[Y] * v2[Z] - v1[Z] * v2[Y]);
	vres[Y] = (v1[Z] * v2[X] - v1[X] * v2[Z]);
	vres[Z] = (v1[X] * v2[Y] - v1[Y] * v2[X]);
}
 
/* Get vektor between 2 points: start in v1,end in v2 */
void mVector(Vx vres, Vx v1, Vx v2)
{
	vres[X] = v2[X] - v1[X];		
	vres[Y] = v2[Y] - v1[Y];		
	vres[Z] = v2[Z] - v1[Z];		
}
 
/* Get |vektor| =length of vektor */
GLfloat mMagnitude(Vx v)
{
	return (GLfloat)sqrt( (v[X] * v[X]) + (v[Y] * v[Y]) + (v[Z] * v[Z]) );
}
 
/* Normalize vektor */ 
void mNormalize(Vx v)
{
	GLfloat magnitude = mMagnitude(v);
 
	v[X] /= magnitude;
	v[Y] /= magnitude;
	v[Z] /= magnitude;
}
 
/* Get prim3 (triangle) normal */
/* p = poligon pointer */
void mGetNormal(Vx vnorm, Vx *p)
{
	Vx v1, v2;

	/*XXX may be change v1<-->v2 ?? */
	mVector(v1, p[2], p[0]); //bot
	mVector(v2, p[1], p[0]); //lft 

	mCross(vnorm, v1, v2);
	mNormalize(vnorm);
}

// http://wingman.org.ru/opengl

/* Point and Plane*/ 
float mPlaneDistance(Vx Normal, Vx Point)
{
	GLfloat distance = 0;	// Переменная хранит дистанцию плоскости от начала координат
 
	// Используем уравнение плоскости для нахождения дистанции (Ax + By + Cz + D = 0).
	// Нам нужно найти D. Больше информации об уравнении плоскости будет ниже (в IntersectedPlane()).
	// Основное: A B C - это значения X Y Z нашей нормали, а x y z - это x y z нашей точки.
	// D - дистанция от начала координат. Итак, нам нужно воспользоваться этим уравнением, чтобы найти D.
	distance = - ((Normal[X] * Point[X]) + (Normal[Y] * Point[Y]) + (Normal[Z] * Point[Z]));
 
	return distance;	// Возвратим дистанцию
}

 
bool mIsLineIntersectPlane(Vx vPoly[], Vx vLine[], Vx vNormal, GLfloat *originDistance)
{
	GLfloat distance1=0, distance2=0;
 
	mGetNormal(vNormal, vPoly);	
 
	*originDistance = mPlaneDistance(vNormal, vPoly[X]);
 
	distance1 = ((vNormal[X] * vLine[0][X])  +					// Ax +
		         (vNormal[Y] * vLine[0][Y])  +					// Bx +
				 (vNormal[Z] * vLine[0][Z])) + *originDistance;	// Cz + D
 
	distance2 = ((vNormal[X] * vLine[1][X])  +					// Ax +
		         (vNormal[Y] * vLine[1][Y])  +					// Bx +
				 (vNormal[Z] * vLine[1][Z])) + *originDistance;	// Cz + D
 
	if(distance1 * distance2 >= 0)
	   return false;
 
	return true;
}
 
 
GLfloat mDot(Vx vVector1, Vx vVector2)
{
	return ( (vVector1[X] * vVector2[X]) + (vVector1[Y] * vVector2[Y]) + (vVector1[Z] * vVector2[Z]) );
}
 
double mAngleBetweenVectors(Vx Vector1, Vx Vector2)
{
	GLfloat dotProduct = mDot(Vector1, Vector2);
	GLfloat vectorsMagnitude = mMagnitude(Vector1) * mMagnitude(Vector2) ;
	double angle = acos( dotProduct / vectorsMagnitude );

	if(_isnan(angle))
		return 0;

	return( angle );
}
 
 
/* Line and Plane */ 
Vx mLPIntersectionPoint(Vx vNormal, Vx vLine[], double distance)
{
	Vx vPoint = {0}, vLineDir = {0};		// Переменные для точки пересечения и направления линии
	double Numerator = 0.0, Denominator = 0.0, dist = 0.0;
 
 
	// Здесь немного сложная часть. Нам нужно найти 3д точку, находящуюся на плоскости.
	// Вот шаги для реализации этого:
 
	// 1) Сначала нам нужно получить вектор нашей линии, затем нормализовать его, чтобы длинна была 1
	vLineDir = mVector(vLine[1], vLine[0]);		// Получим вектор линии
	vLineDir = mNormalize(vLineDir);			// Нормализуем его
 
 
	// 2) Используем формулу плоскости (дистанция = Ax + By + Cz + D) чтобы найти дистанцию от одной из
	// точек до плоскости. Делаем дистанцию отрицательной, т.к. нам нужно идти НАЗАД от нашей точки
	// к плоскости. Это действие просто возвращает назад к плоскости, чтобы найти точку пересечения.
 
	Numerator = - (vNormal[X] * vLine[0].x +		// Используем формулу плоскости с нормалью и линией
				   vNormal.y * vLine[0].y +
				   vNormal.z * vLine[0].z + distance);
 
	// 3) Если мы получим Dot product между вектором нашей линии и нормалью полигона,
	// это даст нам косинус угла между 2мя (т.к. они обе нормализованы - длинна 1).
	// Затем мы разделим Numerator на это значение чтобы найти отстояние плоскости от начальной точки.
 
	Denominator = mDot(vNormal, vLineDir);	// Получаем Dot pruduct между линией и нормалью
 
	// Так как мы используем деление, нужно уберечься от ошибки деления на ноль. Если мы получим 0, 
	// это значит, это НЕДОСТИЖИМАЯ точка, т.к. линая находится на плоскости (нормаль перпендикулярна
	// к линии - (Normal.Vector = 0)).
	// В этом случае просто вернем любую точку на линии.
 
	if( Denominator == 0.0)		// Проверим, не делим ли мы на ноль
		return vLine[0];	// Вернем любую точку линии
 
 
	// Мы делим (дистанция от отчки до плоскости) на (dot product), чтобы получить дистанцию
	// (dist), которая нужна нам для движения от начальной точки линии. Нам нужно умножить эту дистанцию (dist)
	// на вектор линии (направление). Когда вы умножаете scalar на ветор, вы двигаетесь вдоль
	// этого вектора. Это и есть то, что мы делаем. Мы двигаемся от нашей начальной точки, выбранной
	// на линии, НАЗАД к плоскости вдоль вектора линии. Логично было бы просто получить Numerator,
	// который является дистанцией от точки до линии, а потом просто двигатся назад вдоль вектора линии.
	// Дистанция от плоскости - имеется в виду САМАЯ КОРОТКАЯ дистанция. Что если линия почти параллельна
	// полигону, и не пересекается с ним на протяжении своей длинны? Расстояние до плоскости мало, но 
	// расстояние до точки пересечения вектора линии с плоскостью очень велико. Если мы разделим
	// дистанцию на dot product из вектора линии и нормали плоскости, то получим правильную длинну.
 
	dist = Numerator / Denominator;	
 
	// Теперь, как и говорилось выше, делим дистанцию на вектор, потом добавляем точку линии.
	// Это переместит точку вдоль вектора на некую дистанцию. Это в свою очередь даст
	// нам точку пересечения.
 
	vPoint[X] = (GLfloat)(vLine[0][X] + (vLineDir[X] * dist));
	vPoint[Y] = (GLfloat)(vLine[0][Y] + (vLineDir[Y] * dist));
	vPoint[Z] = (GLfloat)(vLine[0][Z] + (vLineDir[Z] * dist));
 
	return vPoint;			// Вернем точку пересечения.
}
 
 
bool mIsPointInsidePolygon(Vx vIntersection, Vx Poly[], long verticeCount)
{
	const double MATCH_FACTOR = 0.9999;		// Исп. для покрытия ошибки плавающей точки
	double Angle = 0.0;				// Инициализируем угол
	Vx vA, vB;				// Временные векторы
 
	for (int i = 0; i < verticeCount; i++)		// Проходим циклом по каждой вершине и складываем их углы
	{
		vA = mVector(Poly[i], vIntersection);	// Вычитаем точку пересечения из текущей вершины //0,1,2
		// Вычитаем точку пересечения из следующей вершины:
		vB = mVector(Poly[(i + 1) % verticeCount], vIntersection); //1,2,0
		// Находим угол между 2мя векторами и складываем их все
		Angle += mAngleBetweenVectors(vA, vB);	
	}
 
	// Теперь имея сумму углов, нам нужно проверить, равны ли они 360. Так как мы используем
	// Dot product, мы работаем в радианах, так что проверим, равны ли углы 2*PI. PI мы обьявили в 3dmath.h.
	// Вы заметите, что мы используем MATH_FACTOR. Мы используем его из-за неточности в рассчетах 
	// с плавающей точкой. Обычно результат не будет ровно 2*PI, так что нужно учесть маленькую
	// погрешность. Я использовал .9999, но вы можете изменить это на ту погрешность, которая вас 
	// устроит.
 
	if(Angle >= (MATCH_FACTOR * (2.0 * PI)) )	// Если угол >= 2PI (360 градусов)
		return true;				// Точка находится внутри полигона
 
	return false;		// Иначе - снаружи
}
 
 
bool mIsLineIntersectPolygon(Vx vPoly[], Vx vLine[], int verticeCount)
{
	Vx vNormal = {0}, vIntersection;
	GLfloat originDistance = 0;
 
	if(!mIsLineIntersectPlane(vPoly, vLine, vNormal, &originDistance))
		return false;
 
	vIntersection = mLPIntersectionPoint(vNormal, vLine, originDistance);
 
	if(mIsPointInsidePolygon(vIntersection, vPoly, verticeCount))
		return true;
	return false;
}


