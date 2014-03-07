#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include "model.h"

using namespace std;

int height, width;
int minim;
int i = 0;
int xAnterior, yAnterior;
char mode;
float negre = 0.0;
double angleX = 0.0;
double angleY = 0.0;
bool modeLinea = false;
bool enable = true;
vector<double> xyz(3, 0.0);
vector<double> xeyeze(3, 1);
vector<double> xxyyzz(3, 0.0);
vector<int> anglesCamera(12, 0);
Model m;

struct Contenidor {
    double xMax;
    double xMin;
    double yMax;
    double yMin;
    double zMax;
    double zMin;
};

void initAngles() {
    anglesCamera[0] = 90;
    anglesCamera[1] = 90;

    anglesCamera[2] = 180;
    anglesCamera[3] = 180;

    anglesCamera[4] = 270;
    anglesCamera[5] = 270;

    anglesCamera[6] = 0;
    anglesCamera[7] = 90;

    anglesCamera[8] = 0;
    anglesCamera[9] = 180;

    anglesCamera[10] = 0;
    anglesCamera[11] = 270;

}

void initGL (int argc, const char *argv[]) {
    height = 600;
    width = 600;
    initAngles();
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(height, width);
    glClearColor(negre, negre, negre, 0.0);
    glutCreateWindow("IDI: Practiques OpenGL");
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    m.load("HomerProves.obj");
}

void rotar() {
    //angle, x, y, z
    glRotated(angleX, 1, 0, 0);
    glRotated(angleY, 0, 1, 0);
}

void traslladar() {
    glTranslated(xxyyzz[0], xxyyzz[1], xxyyzz[2]);
}

void escalar(){
    glScaled(xeyeze[0], xeyeze[1], xeyeze[2]);
}

Contenidor calcularContenidor(const std::vector<Vertex> &v) {
    Contenidor cont;
    cont.xMax = v[0];
    cont.yMax = v[1];
    cont.zMax = v[2];
    cont.xMin = v[0];
    cont.yMin = v[1];
    cont.zMin = v[2];
    for (int i = 3; i < v.size(); ++i) {
        if (i % 3 == 0) {
            //X
            if (cont.xMax < v[i]) cont.xMax = v[i];
            if (cont.xMin > v[i]) cont.xMin = v[i];
        }
        else if (i % 3 == 1) {
            //Y
            if (cont.yMax < v[i]) cont.yMax = v[i];
            if (cont.yMin > v[i]) cont.yMin = v[i];
        }
        else if (i % 3 == 2) {
            //Z
            if (cont.zMax < v[i]) cont.zMax = v[i];
            if (cont.zMin > v[i]) cont.zMin = v[i];
        }
    }
    return cont;
}

double calcularEscalat(Contenidor& cont) {
    //Busco quin es el costat mes gran
    double costatX = cont.xMax - (double) cont.xMin;
    double costatY = cont.yMax - (double) cont.yMin;
    double costatZ = cont.zMax - (double) cont.zMin;
    if (costatX >= costatY and costatX >= costatZ) return 1.0 / (double) costatX;
    else if (costatY >= costatZ) return 1.0 / (double) costatY;
    else return 1.0 / (double) costatZ;
}

vector<double> calcularCentre(Contenidor& cont) {
    vector<double> centre(3, 0.0);
    centre[0] = (-1.0)*((cont.xMin + cont.xMax) / 2.0);
    centre[1] = (-1.0)*(cont.yMin);
    centre[2] = (-1.0)*((cont.zMin + cont.zMax) / 2.0);
    return centre;
}

void modelOBJ() {
    const std::vector<Face> f = m.faces();
    const std::vector<Vertex> v = m.vertices();
    Contenidor cont = calcularContenidor(v);
    double factorEscalat = calcularEscalat(cont);
    glScaled(factorEscalat, factorEscalat, factorEscalat);
    vector<double> centreContenidor = calcularCentre(cont);
    glTranslated(centreContenidor[0], centreContenidor[1], centreContenidor[2]);
    for (int i = 0; i < f.size(); ++i) {
        glBegin(GL_TRIANGLES);
            glColor3f(Materials[f[i].mat].diffuse[0], Materials[f[i].mat].diffuse[1], Materials[f[i].mat].diffuse[2]);
            glVertex3dv(&m.vertices()[f[i].v[0]]);
            glVertex3dv(&m.vertices()[f[i].v[1]]);
            glVertex3dv(&m.vertices()[f[i].v[2]]);
        glEnd();
    }
}

void ninot() {
    glLoadIdentity();
    glPushMatrix();
        glTranslated(0.0, 0.0, 0.0);
        rotar();
        escalar();
        glColor3f(1,1,1);
        glutWireSphere(0.4, 20, 20);
    glPopMatrix();

    glLoadIdentity();

    glPushMatrix();
        rotar();
        escalar();
        glTranslated(0, 0.6, 0);
        glutWireSphere(0.2, 20, 20);
    glPopMatrix();

    glLoadIdentity();

    glPushMatrix();
        rotar();
        escalar();
        glTranslated(0.1, 0.6, 0);
        glRotated(90, 0, 1, 0);
        glColor3d(1, 0, 0);
        glutWireCone(0.1, 0.2, 20, 20);
    glPopMatrix();
}

void model() {
    glPushMatrix();
        rotar();
        modelOBJ();
    glPopMatrix();
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
    eixos();
    ninot();
    model();

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
        cout << " - prement ’t’ traslladem el ninot de neu" << endl;
        cout << " - prement ’r’ rotem el ninot de neu" << endl;
        cout << " - prement ’s’ escalem el ninot de neu" << endl;
        cout << " - prement ’l’ canviem entre GL_FILL i GL_LINE" << endl;
        cout << " - prement ’p’ canviem la profunditat" << endl;
    }
    else if  (caracter == 27) {
        glutLeaveMainLoop();
    }
    else if (caracter == 'r') mode = 'r';
    else if (caracter == 't') mode = 't';
    else if (caracter == 's') mode = 's';
    else if (caracter == 'v') {
        mode = 'v';
        angleX = anglesCamera[i];
        ++i;
        angleY = anglesCamera[i];
        ++i;
        if (i >= 0 and i <= 11) {
            xyz[0] = 1;
        }
        else if (i >= 12 and i <= 24) {
            xyz[1] = 1;
            xyz[0] = 0;
        }
        glutPostRedisplay();
    }
    else if (caracter == 'l') {
        mode = 'l';
        if (not modeLinea) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            modeLinea = true;
        }
        else {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            modeLinea = false;
        }
        glutPostRedisplay();
    }
    else if (caracter == 'p') {
        mode = 'p';
        if (enable) {
            glDisable(GL_DEPTH_TEST);
            enable = false;
        }
        else {
            glEnable(GL_DEPTH_TEST);
            enable = true;
        }
        glutPostRedisplay();
    }
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