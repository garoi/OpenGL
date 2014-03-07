#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
using namespace std;

int height, width;
int minim;
int xAnterior, yAnterior;
char mode;
float negre = 0.0;
double angleX = 0.0;
double angleY = 0.0;
double angleZ = 0.0;
vector<double> xyz(3, 0.0);
vector<double> xeyeze(3, 0.5);
vector<double> xxyyzz(3, 0.0);

void initGL (int argc, const char *argv[]) {
    height = 600;
    width = 600;
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(height, width);
    glClearColor(negre, negre, negre, 0.0);
    glutCreateWindow("IDI: Practiques OpenGL");
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
}

void rotar() {
    //angle, x, y, z
    glRotated(angleX, 1, 0, 0);
    glRotated(angleY, 0, 1, 0);
    glRotated(angleZ, 0, 0, 1);
}

void traslladar() {
    glTranslated(xxyyzz[0], xxyyzz[1], xxyyzz[2]);
}

void escalar(){
    glScaled(xeyeze[0], xeyeze[1], xeyeze[2]);
}

void tetera() {
    glPushMatrix();
        glRotated(angleX, 1, 0, 0);
        glRotated(angleY, 0, 1, 0);
        glRotated(angleZ, 0, 0, 1);

        glColor3f(1,1,1);
        glTranslated(0.5, 0, 0);
        glutWireTeapot(0.50);
    glPopMatrix();
        glRotated(angleX*-1, 1, 0, 0);
        glRotated(angleY*-1, 0, 1, 0);
        glRotated(angleZ*-1, 0, 0, 1);
        glTranslated(-0.5, 0, 0);
        glutWireTeapot(0.20);
}

void eixos() {
    glPushMatrix();
        //X
        glBegin(GL_LINES);
            glColor3f(1,0,0);
            glVertex3d(0,0,0);
            glVertex3d(1,0,0);
        glEnd();
        //Y
        glBegin(GL_LINES);
            glColor3d(0,1,0);
            glVertex3d(0,0,0);
            glVertex3d(0,1,0);
        glEnd();
        //Z
        glBegin(GL_LINES);
            glColor3f(0,0,1);
            glVertex3f(0,0,0);
            glVertex3f(-1,-1,0);
        glEnd();
    glPopMatrix();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    eixos();
    tetera();

    glutSwapBuffers();
}

void resize(int w, int h) {
    if (w > h){
        glViewport(w / 2 - h / 2, 0, h, h);
        minim = h;
    }
    else {
        glViewport(0, h / 2 - w / 2, w, w);
        minim = w;
    }
    height = h;
    width = w;
}


void teclat(unsigned char caracter, int x, int y) {
    xyz = vector<double>(3, 0.0);
    if (caracter == 'h'){
        cout << "AJUDA:" << endl;
        cout << " - prement ’h’ apareix l'ajuda" << endl;
        cout << " - prement ’ESC’ es tanca l'aplicació" << endl;
        cout << " - prement ’t’ traslladem la tetera" << endl;
        cout << " - prement ’r’ rotem la tetera" << endl;
        cout << " - prement ’s’ escalem la tetera" << endl;
    }
    else if  (caracter == 27) {
        glutLeaveMainLoop();
    }
    else if (caracter == 'r') mode = 'r';
    else if (caracter == 't') mode = 't';
    else if (caracter == 's') mode = 's';

}

void move(int x, int y) {
    if (mode == 'r') {
        xyz = vector<double>(3, 0.0);
        if (y > yAnterior) {
            //baixant per la y
            angleX = (int)(angleX-1 + 360) % (int)360;
            xyz[0] = 1;
            glutPostRedisplay();
        }
        else if (y < yAnterior) {
            //pujant per la y
            angleX = (int)(angleX+1 + 360) % (int)360;
            xyz[0] = 1;
            glutPostRedisplay();
        }
        else if (x > xAnterior) {
            //cap a la dreta
            angleY = (int)(angleY-1 + 360) % (int)360;
            xyz[1] = 1;
            glutPostRedisplay();
        }
        else if (x < xAnterior) {
            angleY = (int)(angleY+1 + 360) % (int)360;
            xyz[1] = 1;
            glutPostRedisplay();
        }
        xAnterior = x;
        yAnterior = y;
        glutPostRedisplay();
    }
    else if (mode == 's') {
        if (x > xAnterior and y < yAnterior) {
            xeyeze[0] += 0.1;
            xeyeze[1] += 0.1;
            xeyeze[2] += 0.1;
            glutPostRedisplay();
        }
        else if (x < xAnterior and y > yAnterior){
            xeyeze[0] -= 0.1;
            xeyeze[1] -= 0.1;
            xeyeze[2] -= 0.1;
            glutPostRedisplay();
        }
        xAnterior = x;
        yAnterior = y;
        glutPostRedisplay();
    }
}
//
void punter(int idBoto, int presONo, int x, int y) {
    if (mode == 't' and idBoto == 0 and presONo == 1) {
        double xPunter = (double)x;
        double yPunter = ((double)height - y);
        xxyyzz[0] = 2.0*((xPunter-((width-minim)/2.0)) / minim) -1.0;
        xxyyzz[1] = 2.0*((yPunter-((height-minim)/2.0)) / minim) -1.0;
        xxyyzz[2] = 0.0;
        glutPostRedisplay();
    }
}

int main(int argc,const char * argv[]) {
    initGL(argc, argv);

    glutDisplayFunc(renderScene);

    glutMotionFunc(move);
    glutMouseFunc(punter);
    glutKeyboardFunc(teclat);
    glutReshapeFunc(resize);

    glutMainLoop();
}