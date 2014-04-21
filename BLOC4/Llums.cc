#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
#include <cmath>
#include "model.h"

using namespace std;

int height, width, xAnterior, yAnterior, AngleY, AngleX, AngleZ;
char mode;
bool ortoPers = false;       //true = ortogonal
bool parets = true;         //true = parets visibles
bool primeraPersona = false;
bool tafaner = false;
bool esNormal = false;
float negre = 0.0;
double factorZoom = 0.0;

struct Mov {
    double Rotacio;
    double X;
    double Z;
    double VelRotacio;
    double VelXZ;
    double RotacioTaf;
};

struct Contenidor {
    double xMax;
    double xMin;
    double yMax;
    double yMin;
    double zMax;
    double zMin;
    vector<double> centre;
    double factorEscalat;
};

struct EsferaMinima {
    double xMax;
    double xMin;
    double yMax;
    double yMin;
    double zMax;
    double zMin;
    float radiEsferaMinima;
    float diametreEsferaMinima;
};

struct Camera
{
    vector<double> up;
    vector<double> Obs;
};

struct Mat {
    float Ambient[4];
    float Diffuse[4];
    float Specular[4];
};

Model m;
Contenidor cont;
EsferaMinima ce;
Camera ca;
Mov Mov;
float PE0[4] = {5.0, 2.0, 5.0, 1.0};
float PE1[4] = {5.0, 2.0, -5.0, 1.0};
float PE2[4] = {-5.0, 2.0, -5.0, 1.0};
float PE3[4] = {-5.0, 2.0, 5.0, 1.0};
float PE4[4] = {2.5, 2.0, -2.5, 1.0};
float llum0[4] = {1.0, 1.0, 0.0, 1.0};
float llum1[4] = {1.0, 1.0, 1.0, 1.0};
int PosEscena = 0;

void calcularContenidorMinim() {
    const std::vector<Vertex> v = m.vertices();
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
}

void calcularEsferaMinima() {
    ce.xMin = ce.zMin = -5.0;
    ce.xMax = ce.zMax = 5.0;
    ce.yMin = 0.0;
    ce.yMax = 1.5;
    ce.diametreEsferaMinima = sqrt(pow((ce.xMax-ce.xMin), 2.0) + pow((ce.yMax-ce.yMin), 2.0) + pow((ce.zMax-ce.zMin), 2.0));
    ce.radiEsferaMinima = ce.diametreEsferaMinima / 2.0;
}

void initPosCamera() {
    glLoadIdentity();
    ca.Obs = vector<double>(3, 0.0);
    ca.Obs[2] = ce.diametreEsferaMinima;
    ca.up = vector<double>(3, 0.0);
    if (primeraPersona) {
        ca.up[1] = 1;
        gluLookAt(Mov.X, 1, Mov.Z, Mov.X + sin(Mov.Rotacio*M_PI/180.0), 1, Mov.Z + cos(Mov.Rotacio*M_PI/180.0), ca.up[0], ca.up[1], ca.up[2]);
    }
    else {
        glTranslated(ca.Obs[0], ca.Obs[1], -ca.Obs[2]);
        glRotated(-AngleZ, 0,0,1);
        glRotated(AngleX, 1,0,0);
        glRotated(-AngleY,0,1,0);
        glTranslated(ca.up[0], ca.up[1], ca.up[2]);
    }
}

void initCamaraOrto() {
    double relAspecte = (double) width / (double) height;
    double esqDret = ce.radiEsferaMinima;
    double daltBaix = ce.radiEsferaMinima;
    if (relAspecte >= 1) {
        esqDret *= relAspecte;
    }
    else {
        daltBaix /= relAspecte;
    }
    glOrtho(-esqDret-factorZoom, esqDret+factorZoom, -daltBaix-factorZoom , daltBaix+factorZoom, ce.diametreEsferaMinima-ce.radiEsferaMinima, ce.diametreEsferaMinima+ce.radiEsferaMinima);
}

void initCamaraPers() {
    double relAspecte = (double) width / (double) height;
    double angle = asin(ce.radiEsferaMinima / ce.diametreEsferaMinima);

    if(relAspecte < 1.0) {
        angle = atan(tan(angle) / relAspecte);
    }

    angle = (angle*180)/M_PI; // pas a radiants

    if (not primeraPersona) {
       gluPerspective(2*angle+factorZoom, relAspecte, ce.diametreEsferaMinima - ce.radiEsferaMinima , ce.radiEsferaMinima + ce.diametreEsferaMinima);

    }
    else {
        gluPerspective(60, relAspecte, 0.1, 3*ce.radiEsferaMinima);
    }
}

void initCamara() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (ortoPers) {
        initCamaraOrto();
    }
    else {
        initCamaraPers();
    }
    glMatrixMode(GL_MODELVIEW);

    initPosCamera();
    glutPostRedisplay();
}

void initGL (int argc, const char *argv[]) {
    height = 600;
    width = 600;
    glutInit(&argc, (char **)argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(height, width);
    glClearColor(negre, negre, negre, negre);
    glutCreateWindow("IDI: Practiques OpenGL BLOC 3");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void calcularContenidor(const std::vector<Vertex> &v) {
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
}

double calcularEscalat(double costatGran) {
    //Busco quin es el costat mes gran
    double costatX = cont.xMax - (double) cont.xMin;
    double costatY = cont.yMax - (double) cont.yMin;
    double costatZ = cont.zMax - (double) cont.zMin;
    if (costatX >= costatY and costatX >= costatZ) return costatGran / (double) costatX;
    else if (costatY >= costatZ) return costatGran / (double) costatY;
    else return costatGran / (double) costatZ;
}

void calcularCentre() {
    cont.centre = vector<double>(3, 0.0);
    cont.centre[0] = (-1.0)*((cont.xMin + cont.xMax) / 2.0);
    cont.centre[1] = (-1.0)*(cont.yMin);
    cont.centre[2] = (-1.0)*((cont.zMin + cont.zMax) / 2.0);
}

void lightModel() {
    glPushMatrix();
        float vectorModel [4] = {1.0, 1.0, 1.0, 1.0};
        float positionModel [4] = {Mov.X, 1.0, Mov.Z, 1.0};
        glLightfv(GL_LIGHT2, GL_DIFFUSE, vectorModel);
        glLightfv(GL_LIGHT2, GL_SPECULAR, vectorModel);
        glLightfv(GL_LIGHT2, GL_POSITION, positionModel);
    glPopMatrix();
}

/*S'ha de fer per vertex*/
void modelOBJ(double costatGran) {
    const std::vector<Face> f = m.faces();
    const std::vector<Vertex> v = m.vertices();
    calcularContenidor(v);
    cont.factorEscalat = calcularEscalat(costatGran);
    glScaled(cont.factorEscalat, cont.factorEscalat, cont.factorEscalat);
    calcularCentre();
    glTranslated(cont.centre[0], cont.centre[1], cont.centre[2]);
    for (int i = 0; i < f.size(); ++i) {
        glColor4fv(Materials[f[i].mat].diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, Materials[f[i].mat].ambient);
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, Materials[f[i].mat].diffuse);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, Materials[f[i].mat].specular);
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, Materials[f[i].mat].shininess);

        if(esNormal) glNormal3dv(&m.normals()[f[i].n[0]]);
        glBegin(GL_TRIANGLES);
            if(not esNormal) glNormal3dv(&m.normals()[f[i].n[0]]);
            glVertex3dv(&m.vertices()[f[i].v[0]]);
            if(not esNormal) glNormal3dv(&m.normals()[f[i].n[1]]);
            glVertex3dv(&m.vertices()[f[i].v[1]]);
            if(not esNormal) glNormal3dv(&m.normals()[f[i].n[2]]);
            glVertex3dv(&m.vertices()[f[i].v[2]]);
        glEnd();
    }
}

void model(double costatGran) {
    glPushMatrix();
        lightModel();
        modelOBJ(costatGran);
    glPopMatrix();
}

void ninot() {
    Mat mate;
    mate.Ambient[0] = mate.Ambient[1] = mate.Ambient[2] = mate.Ambient[3] = 1.0;
    mate.Diffuse[0] = mate.Diffuse[1] = mate.Diffuse[2] = mate.Diffuse[3] = 1.0;
    mate.Specular[0] = mate.Specular[1] = mate.Specular[2] = mate.Specular[3] = 1.0;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mate.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mate.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mate.Specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127.0);
    glPushMatrix();
        glTranslated(0.0, 0.0, 0.0);
        glColor3f(1,1,1);
        glutSolidSphere(0.4, 20, 20);
    glPopMatrix();

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mate.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mate.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mate.Specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127.0);
    glPushMatrix();
        glTranslated(0, 0.6, 0);
        glColor3f(1,1,1);
        glutSolidSphere(0.2, 20, 20);
    glPopMatrix();

    Mat mate2;
    mate2.Ambient[0] = 1.0;
    mate2.Ambient[1] = 0.5;
    mate2.Ambient[3] = 1.0;
    mate2.Diffuse[0] = 1.0;
    mate2.Diffuse[1] = 0.5;
    mate2.Diffuse[3] = 1.0;
    mate2.Specular[0] = 1.0;
    mate2.Specular[1] = 0.5;
    mate2.Specular[3] = 1.0;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mate2.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mate2.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mate2.Specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127.0);
    glPushMatrix();
        glTranslated(0.1, 0.6, 0);
        glRotated(90, 0, 1, 0);
        glColor3d(1, 0, 0);
        glutSolidCone(0.1, 0.2, 20, 20);
    glPopMatrix();
}

void pintarTerra() {
    Mat mate;
    mate.Ambient[2] = 0.5;
    mate.Ambient[3] = 1.0;
    mate.Diffuse[2] = 0.5;
    mate.Diffuse[3] = 1.0;
    mate.Specular[2] = 0.5;
    mate.Specular[3] = 1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mate.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mate.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mate.Specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 127.0);
    glNormal3f(0.0, 1.0, 0.0);

    glPushMatrix();
        glBegin(GL_QUADS);
            glColor3f(0, 0.5, 1.0);
            //L'ordre es: inferior dreta, superior dreta, inferior dreta, superior dreta
            glVertex3f(5.0, 0, 5.0);
            glVertex3f(5.0, 0, -5.0);
            glVertex3f(-5.0, 0, -5.0);
            glVertex3f(-5.0, 0, 5.0);
        glEnd();
    glPopMatrix();
}

void pintarParets() {
    Mat mate;
    mate.Ambient[1] = 1.0;
    mate.Ambient[3] = 1.0;
    mate.Diffuse[1] = 1.0;
    mate.Diffuse[3] = 1.0;
    mate.Specular[1] = 1.0;
    mate.Specular[3] = 1.0;
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, mate.Ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mate.Diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mate.Specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 1.0);

    glPushMatrix();
        glTranslated(2.5, 1.5/2.0, -1.5);
        glScaled(4.0, 1.5, 0.2);
        glTranslated(0.0, 0.0, 0.0);
        glColor3f(0, 1.0, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();

    glPushMatrix();
        glColor3d(0.3, 0.8, 0.1);
        glTranslated(-4.9, 1.5/2.0, 0);
        glScaled(0.2, 1.5, 10);
        glTranslated(0.0, 0.0, 0.0);
        glColor3f(0, 1.0, 0.0);
        glutSolidCube(1.0);
    glPopMatrix();

}

void llumDeCamara() {
    glPushMatrix();
    glLoadIdentity();
        glLightfv(GL_LIGHT1, GL_DIFFUSE, llum1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, llum1);
    glPopMatrix();
}

void llumEscena() {
  glPushMatrix();
      glLightfv(GL_LIGHT0, GL_DIFFUSE, llum0);
      glLightfv(GL_LIGHT0, GL_SPECULAR, llum0);
      if(PosEscena == 0) glLightfv(GL_LIGHT0, GL_POSITION, PE0);
      else if(PosEscena == 1) glLightfv(GL_LIGHT0, GL_POSITION, PE1);
      else if(PosEscena == 2) glLightfv(GL_LIGHT0, GL_POSITION, PE2);
      else if(PosEscena == 3) glLightfv(GL_LIGHT0, GL_POSITION, PE3);
      else if(PosEscena == 4) glLightfv(GL_LIGHT0, GL_POSITION, PE4);
  glPopMatrix();
}

void renderScene(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    llumDeCamara();
    llumEscena();

    glPushMatrix();
        pintarTerra();

        //NINOT
        glPushMatrix();
            glTranslated(2.5, 0.4, 2.5);
            ninot();
        glPopMatrix();

        glPushMatrix();
            glTranslated(-2.5, 0.4, 2.5);
            ninot();
        glPopMatrix();

        glPushMatrix();
            glTranslated(-2.5, 0.4, -2.5);
            ninot();
        glPopMatrix();


        //MODEL
        glPushMatrix();
            glTranslated(Mov.X, 0, Mov.Z);
            glRotated(Mov.Rotacio, 0, 1, 0);
            model(1.0);
        glPopMatrix();

        glPushMatrix();
            glTranslated(2.5, 0, -2.5);
            glRotated(90, 0, 1, 0);
            glTranslated(0.0, 0.0, 0.0);
            model(1.5);
        glPopMatrix();

        glPushMatrix();
            if (parets)pintarParets();
        glPopMatrix();

        //Esfera
        glPushMatrix();
            glutWireSphere(ce.radiEsferaMinima, 20, 20);
        glPopMatrix();

    glPopMatrix();

    glutSwapBuffers();
}


void resize(int w, int h) {
    height = h;
    width = w;

    initCamara();

    glViewport(0, 0, w, h);
    glutPostRedisplay();
}


void move(int x, int y) {
    if (x > xAnterior) {
        //cap a la dreta
        ++AngleY;
        initCamara();
        glutPostRedisplay();
    }
    else if (x < xAnterior) {
        //cap a l'esquerra
        --AngleY;
        initCamara();
        glutPostRedisplay();
    }
    else if (y > yAnterior) {
        //baixant per la y
        ++AngleX;
        initCamara();
        glutPostRedisplay();
    }
    else if (y < yAnterior) {
        //pujant per la y
        --AngleX;
        initCamara();
        glutPostRedisplay();
    }
    xAnterior = x;
    yAnterior = y;
    glutPostRedisplay();
}

void zoom(int x, int y) {
    if (y > yAnterior) {
        //baixant per la y
        if (ortoPers) {
            factorZoom += 0.1;
            initCamara();
        }
        else {
            factorZoom += 0.5;
            initCamara();
        }
    }
    else if (y < yAnterior) {
        //pujant per la y
        if (ortoPers) {
            factorZoom -= 0.1;
            initCamara();
        }
        else {
            factorZoom -= 0.5;
            initCamara();
        }
    }
    xAnterior = x;
    yAnterior = y;
}

void teclat(unsigned char caracter, int x, int y) {
    if (caracter == 'h'){
        cout << "####################################################################" << endl;
        cout << "# AJUDA:                                                           #" << endl;
        cout << "# - prement ’h’ apareix l'ajuda                                    #" << endl;
        cout << "# - prement ’ESC’ es tanca l'aplicació                             #" << endl;
        cout << "# - prement ’v’ visible / invisible parets                         #" << endl;
        cout << "# - prement ’p’ canvi entre ortogonal i prespectiva                #" << endl;
        cout << "# - prement ’r’ reset escena                                       #" << endl;
        cout << "# - Scroll Zoom o bé clic dret y arrastrar el ratoli               #" << endl;
        cout << "# - prement 'a' gires a l'esquerra                                 #" << endl;
        cout << "# - prement 'b' gires a la dreta                                   #" << endl;
        cout << "# - prement 'w' avances                                            #" << endl;
        cout << "# - prement 's' cap endarerra                                      #" << endl;
        cout << "# - prement 'c' primera persona, si estas en ortho es canvia       #" << endl;
        cout << "# - prement 't' entres en mode tafaner                             #" << endl;
        cout << "# - prement 'i' activar il·luminació.                              #" << endl;
        cout << "# - prement '2' activar/desactivar una llum blanca.                #" << endl;
        cout << "# - prement 'm' posició focus d'escena canvia.                     #" << endl;
        cout << "# - prement '0' encendre i apagar un llum d’escena.                #" << endl;
        cout << "# - prement '1' pugui activar/desactivar el llum de càmera.        #" << endl;
        cout << "# - prement 'n' communar entre normal per cara i vèrtex.           #" << endl;
        cout << "####################################################################" << endl;
    }
    else if  (caracter == 27) {
        glutLeaveMainLoop();
    }
    else if (caracter == 'v') {
        mode = 'v';
        if (parets) parets = false;
        else parets = true;
        glutPostRedisplay();
    }
    else if (caracter == 'p') {
        mode = 'p';
        if (ortoPers) ortoPers = false;
        else ortoPers = true;
        initCamara();
    }
    else if (caracter == 'r') {
        mode = 'r';
        factorZoom = 0.0;
        PosEscena = 0;
        ortoPers = false;
        esNormal = true;
        AngleX = AngleY = AngleZ = 0;
        Mov.X = Mov.Z = Mov.Rotacio = Mov.RotacioTaf = 0.0;
        Mov.VelRotacio = 2.5;
        Mov.VelXZ = 0.2;
        primeraPersona = false;
        ce.radiEsferaMinima = ce.diametreEsferaMinima / 2.0;
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        initCamara();
    }
    else if (caracter == 'c') {
        mode = 'c';
        if (!primeraPersona) {
            ortoPers = false;
            primeraPersona = true;
        }
        else {
            primeraPersona = false;
            if (tafaner) {
                Mov.Rotacio = Mov.RotacioTaf;
                tafaner = false;
            }
        }
        initCamara();
    }
    else if (caracter == 'a') {
        Mov.Rotacio += Mov.VelRotacio;
        initCamara();
        glutPostRedisplay();
    }
    else if (caracter == 'd') {
        Mov.Rotacio -= Mov.VelRotacio;
        initCamara();
        glutPostRedisplay();
    }
    else if (caracter == 'w') {
        Mov.X += sin(Mov.Rotacio*M_PI/180) * Mov.VelXZ;
        Mov.Z += cos(Mov.Rotacio*M_PI/180) * Mov.VelXZ;
        initCamara();
        glutPostRedisplay();
    }
    else if (caracter == 's') {
        Mov.X -= sin(Mov.Rotacio*M_PI/180) * Mov.VelXZ;
        Mov.Z -= cos(Mov.Rotacio*M_PI/180) * Mov.VelXZ;
        initCamara();
        glutPostRedisplay();
    }
    else if (caracter == 'z') {
        Mov.VelXZ += 0.1;
    }
    else if (caracter == 'x') {
        if (Mov.VelXZ > 0.2) {
            Mov.VelXZ -= 0.1;
        }
        else {
            cout << "no pots reduïr més la velocitat; estas en velocitat mínima" << endl;
        }
    }
    else if (caracter == 't' and primeraPersona) {
        if (not tafaner) {
            tafaner = true;
            Mov.RotacioTaf = Mov.Rotacio;
            cout << "Mode tafaner" << endl;
        }
        else {
            tafaner = false;
            Mov.Rotacio = Mov.RotacioTaf;
            cout << "Surts del mode tafaner" << endl;
        }
    }
    //LLUMS
    else if (caracter == 'i') {
        if (glIsEnabled(GL_LIGHTING)) {
            glDisable(GL_LIGHTING);
            glDisable(GL_LIGHT0);
            cout << "Apagar llums" << endl;
            glutPostRedisplay();
        }
        else {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
            cout << "Encenem llums" << endl;
            glutPostRedisplay();
        }
    }
    else if (caracter == 'n') {
        if (esNormal) esNormal = false;
        else esNormal = true;
        glutPostRedisplay();
    }
    else if (caracter == '1') {
        if (glIsEnabled(GL_LIGHT1)) {
            glDisable(GL_LIGHT1);
        }
        else {
            cout << "Encenem la llum 1" << endl;
            glEnable(GL_LIGHT1);
        }
        glutPostRedisplay();
    }
    if (caracter == '0') {
        if (glIsEnabled(GL_LIGHT0)) glDisable(GL_LIGHT0);
        else glEnable(GL_LIGHT0);
        glutPostRedisplay();
    }
    if (caracter == '2') {
        if (glIsEnabled(GL_LIGHT2)) glDisable(GL_LIGHT2);
        else glEnable(GL_LIGHT2);
        glutPostRedisplay();
    }
    else if (caracter == 'm') {
        ++PosEscena;
        PosEscena %= 5;
        glutPostRedisplay();
    }
}

void teclesEspecial(int caracter, int x, int y) {
    if (tafaner and caracter == GLUT_KEY_LEFT) {
        Mov.Rotacio += Mov.VelRotacio;
        initCamara();
        glutPostRedisplay();
    }
    else if (tafaner and caracter == GLUT_KEY_RIGHT) {
        Mov.Rotacio -= Mov.VelRotacio;
        initCamara();
        glutPostRedisplay();
    }
}


void ratoli(int idBoto, int presONo, int x, int y) {
    if (idBoto == GLUT_LEFT_BUTTON) {
        glutMotionFunc(move);
    }
    else if (idBoto == GLUT_RIGHT_BUTTON) {
        glutMotionFunc(zoom);
    }
    else {
        //Scroll -> zoom
        if (idBoto == 3 and presONo == GLUT_UP) {
            if (ortoPers) {
                factorZoom -= 0.1;
                initCamara();
            }
            else {
                factorZoom -= 0.5;
                initCamara();
            }
        }
        else if (idBoto == 4 and presONo == GLUT_UP){
            if (ortoPers) {
                factorZoom += 0.1;
                initCamara();
            }
            else {
                factorZoom += 0.5;
                initCamara();
            }
        }
    }
}

int main(int argc,const char * argv[]) {
    AngleX = AngleY = AngleZ = 0;
    Mov.X = Mov.Z = Mov.Rotacio = Mov.RotacioTaf = 0.0;
    Mov.VelRotacio = 2.5;
    Mov.VelXZ = 0.2;
    initGL(argc, argv);
    m.load("Patricio.obj");
    // m.load(argv[1]);
    calcularContenidorMinim();
    calcularEsferaMinima();
    initCamara();

    glutDisplayFunc(renderScene);

    glutKeyboardFunc(teclat);
    glutMouseFunc(ratoli);
    glutSpecialFunc(teclesEspecial);

    glutReshapeFunc(resize);

    glutMainLoop();
}