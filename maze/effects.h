/***********/
	glBegin(GL_LINES);//_STRIP);
	for(i = 0; i < 360; i+=1){
		for(j = -90; j <= 90; j++){
			x = r * cos(ATOR(j)) * sin(ATOR(i));
			y = r * cos(ATOR(j)) * cos(ATOR(i));
			z = r * sin(ATOR(j));
			glColor3f(1.0, 1.0, 1.0);
			glVertex3f(x, y, z);
			x = r * cos(ATOR(j)) * sin(ATOR(i+1));
			y = chvar1 * r * cos(ATOR(j)) * cos(ATOR(i+1));
			z = r * sin(ATOR(j+1));
			glColor3f(1.0, 0.0, 0.0);
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
/*********************/
