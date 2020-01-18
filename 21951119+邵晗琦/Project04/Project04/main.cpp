#include <iostream>
#include <glut.h>
#include "cube.h"
float density, Z = 17;
float fogColor[4] = { 0.5,0.5,0.5,1.0 };
float ambient[nshape + 1][4] = {
	{ 0.8,0.8,0.8,0 },
	{ 0.8,0.7,0.7,1.0 },
	{ 0.0,0.7,0.8,0.4 },
	{ 0.0,0.8,0.5,1.0 },
	{ 0.0,1.0,0.0,0.7 },
	{ 0.5,1.0,0.2,0.4 },
	{ 0.5,1.8,0.7,0.7 },
	{ 0.5,1.0,1.9,0.0 },
	{ 0.5,1.0,1.0,0.9 },
	{ 0.8,0.7,0.4,0.9 },
	{ 0.9,0.1,0.0,0.9 },
	{ 1.0,0.0,0.0,1.0 },
	{ 1.0,0.5,0.5,0.9 },
	{ 1.0,0.7,0.6,0.7 },
	{ 1.0,0.8,0.5,0.4 },
	{ 1.0,0.9,0.1,0.9 },
	{ 1.0,1.0,0.9,1.0 }
};
char nk = 0;
float ambient2[] = { 0.96,0.8,0.4,0 };
float ambient3[] = { 0.5,0.5,0.6,0.7 };
float specular[][4] = { { 1,1,1,1 },{ 0.4,0.4,0.2,0.7 } };
float shininess[] = { 80 };
float position[] = { 2,4,6,1 };
float position1[] = { 4,-5,4,1 };
float a1 = 30, a2 = -15, a3;
float             fog_params[4] = { 0.5,0.5,0.5,1.0 };
static float            fog_density = 0.35;
int btn, sta;
int dt = 120, mx, my, mdx, mdy, mx0, my0;
int score = 0, MaxMagc = 500, Magc = MaxMagc, MirMagc = 200, UpMagc = 200, EscMagc = 500;
clock_t t0, t00, t1, delay = 4000;
bool gameover = false;
char buf[64];
static xd = 0, yd = 0, zd = 0;
bool stop = true;
int ele[num][num][num] = {
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,5,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
};
int shape[nshape + 1][num][num][num] = {
	{ { 0 },
	{ 0,0,0,0,
	 0,1,1,0,
   	 0,1,1,0,
	 0,0,0,0 } },
	{ { 0 },
	{ 0,0,0,0,
	 0,2,0,0,
	 0,0,0,0,
	 0,0,0,0 } },
	{ { 0 },
	{ 3,3,3,3,
	 0,0,0,0,
	 0,0,0,0,
	 0,0,0,0 } },
	{ { 0 },
	{ 0,0,2,0,
	 0,2,2,0,
	 0,2,0,0,
	 0,0,0,0 } },
	{ { 0 },
	{ 0,0,4,0,
	 0,0,4,0,
	 0,4,4,0,
	 0,0,0,0 } },
	{ { 0 },
	{ 0,0,0,0,
	 0,5,0,0,
	 5,5,5,0,
	{ { 0 },
	{ 6,6,6,6,
	 6,0,0,6,
	 6,0,0,6,
	 6,6,6,6 } },
	 { { 0 },
	 { 0,6,6,6,
	 0,0,6,6,
	 0,0,0,6,
	 0,0,0,0 } },
	{ { 0,0,0,0,
	 0,7 },
	 { 0,0,0,0,
	 0,7,7,0,
	 0,7,0,0,
	 0,7,0,0 } },
	{ { 0 },
	{ 0,0,0,0,
	 0,9,0,0,
	 0,9,0,0,
	 0,0,0,0 },
	{ 0,0,0,0,
	 0,9,9,0,
	 0,9,0,0,
	 0,0,0,0 } },
	 { { 0 },
	{ 0,0,0,0,
	 0,9,0,0,
	 0,9,0,0,
	 0,0,0,0 },
	{ 0,0,0,0,
	 0,9,9,0,
	 0,9,0,0,
	 0,0,0,0 } },
	{ { 0 },
	{ 0,0,0,0,
	 0,9,0,0,
	 0,9,0,0,
	 0,0,0,0 },
	{ 0,0,0,0,
0,9,9,0,
0,9,0,0,
0,0,0,0 } },
{ { 0 },
{ 0,0,0,0,
0,10,10,0,
0,10,0,0,
0,0,0,0 },
{ 0,0,0,0,
0,0,0,0,
0,0,0,0,
0,0,0,0 } },
{ { 0 },
{ 0,0,0,0,
0,0,0,0,
0,0,0,0,
0,0,0,0 },
{ 0,0,0,0,
11,11,11,0,
11,0,11,0,
11,11,11,0 } },
{ { 0 },
{ 0,0,0,0,
0,14,0,0,
0,14,0,0,
0,0,0,0 },
{ 0,0,0,0,
0,14,14,0,
0,14,14,0,
0,0,0,0 } },
{ { 0 },
{ 0,0,0,0,
0,15,0,0,
0,15,0,0,
0,0,0,0 },
{ 0,0,0,0,
0,15,0,0,
0,15,0,0,
0,0,0,0 } },
{ { 0 },
{ 0,0,0,0,
0,16,0,0,
0,0,0,0,
0,0,0,0 },
{ 0,0,0,0,
0,16,16,0,
0,16,0,0,
0,0,0,0 } }
};
cubes F(ele, num, num, N1 - num);
const int nL = 4;
cubes L[nL] = { F,F,F,F };
int ReadShape(char name[], int* shape, int n)
{
	FILE* infile = fopen(name, "r");
	if (infile == NULL) return -1;
	int i;
	for (i = 0; !feof(infile) && i
		fscanf(infile, "%d", shape + i);
		if (shape[i]<0) shape[i] = 0;
		if (shape[i]>nshape) shape[i] = nshape;
}
return i;
}
void fall(int space[N1][N1][N1], int k1) {
	int i, j, k;
		k = k1;
		for (i = 0; i
			for (j = 0; j
				space[i][k][j] = 0;
}
  }
  for (++k; k
	  for (i = 0; i
		  for (j = 0; j
			  space[i][k - 1][j] = space[i][k][j];
   }
  }
 }
}
int disappear(int space[N1][N1][N1])
{
	int i, j, k, kk = 0;
		for (k = 1; k
			for (i = 0; i
				for (j = 0; j
					if (!space[i][k][j]) break;
}
if (j != N1) break;
  }
  if (i == N1 && j == N1) {
	  ++kk;
		  fall(space, k);
		  _beep(1024, 220);
		  display();
		  _beep(600, 140);
  }
 }
 return kk;
}
void fall2(int space[N1][N1][N1], int k1) {
	int i, j, k;
	k = k1;
	for (i = 0; i
		for (j = 0; j
			space[i][j][k] = 0;
}
  }
  for (++k; k
	  for (i = 0; i
		  for (j = 0; j
			  space[i][j][k - 1] = space[i][j][k];
   }
  }
 }
}
int disappear2(int space[N1][N1][N1])
{
	int i, j, k, kk = 0;
		for (k = 0; k
			for (i = 0; i
				for (j = 0; j
					if (!space[i][j][k]) break;
}
if (j != N1) break;
  }
  if (i == N1 && j == N1) {
	  ++kk;
		  fall2(space, k);
		  _beep(1024, 120);
  }
 }
 return kk;
}
void keyboard(unsigned char, int, int);
void idle(void)
{
	if (gameover) return;
	t1 = clock();
	if (t1 - t0>delay) {
		t0 = clock();
		stop = !L[1].move(0, -1, 0);
		if (stop) {
			gameover = !L[1].push(space);
		}
		//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		//glutPostRedisplay();
		display();
	}
}
void mouse(int button, int state, int x, int y)
{
	btn = button; sta = state;
	if (state == GLUT_DOWN) {
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);
		switch (button) {
		case GLUT_RIGHT_BUTTON:
			//xd=L[1].xc;yd=L[1].yc;zd=L[1].zc;
			//if(L[1].moveto(2*x*N1/w-N1/2,L[1].yc,2*y*N1/h-N1/2))
			//display();
			mx0 = x; my0 = y;
			break;
		case GLUT_MIDDLE_BUTTON:
			keyboard(' ', x, y);
			break;
		case GLUT_LEFT_BUTTON:
			break;
		default:
			break;
		}
		mdx = x; mdy = y;
	}
	else if (GLUT_LEFT_BUTTON == button) {
		int w = glutGet(GLUT_WINDOW_WIDTH);
		int h = glutGet(GLUT_WINDOW_HEIGHT);
	}
	mx = x; my = y;
}
void motion(int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON) {
		int t = 0;
		int x1 = 0, x3 = 0;
		int step = 40;
		if ((x - mx0) / step>0) x1 = 1;
		else if ((x - mx0) / step<0) x1 = -1;
		if ((y - my0) / step>0) x3 = 1;
		else if ((y - my0) / step<0) x3 = -1;
		if (x1 || x3) {
			for (int i = (x - mx0) / step, j = (y - my0) / step; i || j; i -= i ? x1 : 0, j -= j ? x3 : 0) {
				bool m = L[1].move(x1, 0, x3);
				if (m) display();
				t += m;
			}
			if (t) {
				mx0 = x; my0 = y;
			}
		}
	}
	else if (btn == GLUT_LEFT_BUTTON) {
		a1 += 0.5f*(y - my); a2 += 0.5f*(x - mx);
		display();
	}
	mx = x; my = y;
}
void produce() {
	int r;
	if (nk) {
		r = nk - '0';
		nk = 0;
	}
	else r = rand()*nshape / 32767;
	int i;
	for (i = 1; i
		L[i - 1] = L[i];
}
L[nL - 1] = cubes(shape[r]);
L[nL - 1].init(num, N1 - num, -(nL - 1 + 1)*num);
L[nL - 1].mirror(rand()>16000, 0, 0);
L[nL - 1].mirror(0, rand()>16000, 0);
L[nL - 1].mirror(0, 0, rand()>16000);
for (i = 1; i
	if (!L[i].move(0, 0, num, false))
		i = i;
 }
 L[1].move(0, 0, num);
	 Magc += 5;
	 if (Magc>MaxMagc) Magc = MaxMagc;
}
void init()
{
	sprintf(buf, "score=%d", score);
		srand(time(0));
	ReadShape("shape1.txt", &shape[0][0][0][0], nshape*N1*N1*N1);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glEnable(GL_DEPTH_TEST);
	glClearColor(fog_params[0], fog_params[1], fog_params[2], 0.0);
	for (int i = 0; i
		for (int j = 0; j<1; ++j) {
			for (int k = 0; k
				space[i][j][k] = (i == 0 || j == 0 || k == 0 || i == N1 - 1 || j == N1 - 1 || k == N1 - 1) ? 2 : 0;
		}
}
 }
 for (i = 1; i
	 int r = rand()*nshape / 32767;
	 L[i] = cubes(shape[r]);
	 L[i].init(num, N1 - num, num - (i + 1)*num);
 }
 t0 = clock();
}
void DrawState() {
	float ambient[4][4] =
	{ { 0,0,1,1 },
	{ 1,0,0 },
	};
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient[0]);
	glMaterialfv(GL_BACK, GL_DIFFUSE, ambient[0]);
	glPushMatrix();
	glTranslatef(5.5, -4.5, 0);
	glScalef(1, 1, Magc*0.01f);
	glutSolidCube(1);
	glPopMatrix();
}
void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular[0]);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient[0]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient[0]);
	glLoadIdentity();
	//gluLookAt(0,0.4,-2,  0,0,0,  0,1,0);
	int x = glutGet(GLUT_WINDOW_WIDTH);
	int y = glutGet(GLUT_WINDOW_HEIGHT);
	gluPerspective(50.0, (float)x / y, 1.0, 40.0);
	glTranslatef(0, 0, -Z);
	//glDisable(GL_FOG);
	glRotatef(a1, 1, 0, 0);
	glRotatef(a2, 0, 1, 0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient[3]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient[3]);
	DrawState();
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient[5]);
	glMaterialfv(GL_BACK, GL_DIFFUSE, ambient[0]);
	glPushMatrix();
	glTranslatef(2, -4, -6);
	glRotatef(-20, 1, 0, 0);
	glutSolidTorus(0.4, 0.8, 8, 16);
	glTranslatef(3.5, -1, 3);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient[10]);
	glRotatef(-20, 1, 0.4, 0.1);
	glutSolidTorus(0.2, 0.4, 8, 16);
	glTranslatef(-10, -4.4, 6);
	glRotatef(-50, 1, 0, 0);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient[12]);
	glutSolidCone(0.3, 0.6, 8, 16);
	glPopMatrix();
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient[0]);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient[6]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient[0]);
	glPushMatrix();
	glTranslatef(3, -4.5f, 5.3);
	glutSolidSphere(0.3, 5, 5);
	glTranslatef(-6, 0, 0);
	glutSolidSphere(0.3, 5, 5);
	glTranslatef(3, 0, 0);
	glutSolidSphere(0.3, 5, 5);
	glPopMatrix();
	if (stop) {
		stop = false;
		int d = sqr(disappear(space));
		score += d;
		MaxMagc += 100 * d;
		Magc += 100 * d;
		sprintf(buf, "score=%d", score);
		if (!gameover) {
			produce();
			display();
		}
		else {
			_beep(200, 120);
			sprintf(buf, " Game Over  score=%d", score);
			_beep(300, 120);
		}
	}
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient[0]);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient[0]);
	float c = (N1 - 1)*0.5;
	for (int i = 0; i
		for (int j = 0; j
			for (int k = 0; k
				if (space[i][j][k]) {
					if (L[1].under(i, j, k)) {
						float tp[4], s = 0.5;
							tp[0] = s * ambient[space[i][j][k]][0];
							tp[1] = s * ambient[space[i][j][k]][1];
							tp[2] = s * ambient[space[i][j][k]][2];
							tp[3] = s * ambient[space[i][j][k]][3];
							glMaterialfv(GL_FRONT, GL_DIFFUSE, tp);
							glMaterialfv(GL_BACK, GL_DIFFUSE, tp);
							//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,tp);
							//glLightfv(GL_LIGHT0,GL_AMBIENT,tp);
					}
					else {
						glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient[space[i][j][k]]);
						glMaterialfv(GL_BACK, GL_DIFFUSE, ambient[space[i][j][k]]);
						//glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambient[space[i][j][k]]);
						//glLightfv(GL_LIGHT0,GL_AMBIENT,ambient[space[i][j][k]]);
					}
					glPushMatrix();
					glTranslatef((i - c), (j - c), (k - c));
					glutSolidCube(0.9);
					glPopMatrix();
				}
}
   }
  }
  if (!gameover) {
	  for (int i = 1; i
		  glPushMatrix();
		  glTranslatef((L[i].xc - c + 1.5), (L[i].yc - c + 1.5), (L[i].zc - c + 1.5));
		  L[i].draw();
		  glPopMatrix();
  }
 }
 glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT);
	 int len = strlen(buf);
	 for (i = 0; i < len; i++)
		 glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
	 glPopAttrib();
 glutSwapBuffers();
}
void reshape(int x, int y)
{
	glViewport(0, 0, x, y);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}
int save(const char * fn) {
	FILE *F = fopen(fn, "w");
	fwrite((void*)space[0][0], sizeof(space[0][0][0]), N1*N1*N1, F);
	fclose(F);
	return 0;
}
int load(const char * fn) {
	FILE *F = fopen(fn, "r");
	if (F == NULL) return -1;
	fread((void*)space[0][0], sizeof(space[0][0][0]), N1*N1*N1, F);
	fclose(F);
	return 0;
}
void keyboard(unsigned char c, int x, int y)
{
	int i;
	switch (c) {
	case 'a': case 'A':
		L[1].rot(-90, 0, 1, 0);
		break;
	case 'w': case 'W':
		L[1].rot(-90, 1, 0, 0);
		break;
	case 'd': case 'D':
		L[1].rot(90, 0, 1, 0);
		break;
	case 's': case 'S':
		L[1].rot(90, 1, 0, 0);
		break;
	case 'q':
		L[1].rot(90, 0, 0, 1);
		break;
	case 'e': case 'E':
		L[1].rot(-90, 0, 0, 1);
		break;
	case 0x1b:
		if (Magc>EscMagc) Magc -= EscMagc * L[0].escape(space);
		break;
	case '-':
		Z -= 1;
		break;
	case '=':
		Z += 1;
		break;
	case ' ':
		for (i = 0; L[1].move(0, -1, 0); ++i) {
			t0 = clock();
		}
		if (i == 0) {
			stop = true;
			gameover = !L[1].push(space);
		}
		break;
	case 13://'\n'
		while (!stop) {
			stop = !L[1].move(0, -1, 0);
			if (stop) gameover = !L[1].push(space);
			t0 = clock();
		}
		break;
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':
	case '8':case '9':
		nk = c;
		break;
	case 127://Del
		if (Magc>MirMagc) Magc -= MirMagc * L[1].mirror(1, 0, 0);
		break;
	default:
		L[1].move(0, -1, 0);
		t0 = clock();
		break;
	}
	display();
}
void special(int c, int x, int y)
{
	switch (c) {
	case GLUT_KEY_LEFT:
		L[1].move(-1, 0, 0);
		break;
	case GLUT_KEY_RIGHT:
		L[1].move(1, 0, 0);
		break;

	case GLUT_KEY_UP:
		L[1].move(0, 0, -1);
		break;
	case GLUT_KEY_DOWN:
		L[1].move(0, 0, 1);
		break;
	case 4:
		save("save4.dat");
		exit(0);
		break;
	case 8:
		load("save4.dat");
		break;
	case 104:
		if (Magc>MirMagc) Magc -= UpMagc * L[1].move(0, 1, 0);
		break;
	case 105:
		if (Magc>MirMagc) Magc -= MirMagc * L[1].mirror(0, 1, 0);
		break;
	case 107:
		if (Magc>MirMagc) Magc -= MirMagc * L[1].mirror(0, 0, 1);
		break;
	default:
		break;
	}
	display();
}
int main(int argc, char* argv[])
{
	printf("delay = ? ms [100,9000]");
	if (delay<100) delay = 100;
	else if (delay>9000) delay = 9000;
	printf("delay=%ld", delay);
	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("GLUT-based OpenGL Cubes");
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);


	v
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(idle);
	glutSpecialFunc(special);
	init();
	glEnable(GL_FOG);
	glFogi(GL_FOG_MODE, GL_EXP2);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.25);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	t00 = clock();
	glutMainLoop();
	return 0;
}