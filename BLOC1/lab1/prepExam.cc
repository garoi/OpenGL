#include <GL/gl.h>
#include <GL/freeglut.h>
#include <iostream>
#include <vector>
using namespace std;

int xAnterior, yAnterior;
float blanc = 1.0;
char mode;
int contClick = 0;
int height, width;
vector<float> vertex(6);

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  float j = 0;
    for (int i = 0; i < 6; ++i){
      j += 0.5;
      glColor3f(0.0 + j, 0.0+j, 0.0-j);
      j += 0.1;
      glVertex3f(vertex[i], vertex[i+1], 0.0);
      ++i;
    }
  glEnd();
  glutSwapBuffers();
}

void resize(int w, int h) {
  //rediemnsio viewport
  if (w > h){
      glViewport(w / 2 - h / 2, 0, h, h);
  }
  else {
    glViewport(0, h / 2 - w / 2, w, w);
  }
  height = h;
  width = w;
}

void pinta(int x, int y) {
  if (mode == 'f') {
    if (y > yAnterior){
      //aumenta foscor
      if (blanc > 0.0) {
        blanc -= 0.01;
        glClearColor(blanc, blanc, blanc, 0.0);
      }
    }
    else if (y < yAnterior){
      //disminuex foscor
      if (blanc < 1.0){
        blanc += 0.01;
        glClearColor(blanc, blanc, blanc, 0.0);
      }
    }
    xAnterior = x;
    yAnterior = y;
    glutPostRedisplay();
  }
}

void estat(unsigned char caracter, int x, int y) {
  if (caracter == 'h'){
    cout << "AJUDA:" << endl;
    cout << " - prement ’h’ apareix l'ajuda" << endl;
    cout << " - prement ’ESC’ es tanca l'aplicació" << endl;
    cout << " - prement ’f’ i arreosegant el ratoli canviem el color de fons" << endl;
    cout << " - prement ’t’ tal que després de premer-la, els seguents tres clics del ratoĺı defineixin les coordenades dels tres vertexs del triangle a pintar" << endl;
  }
  else if  (caracter == 27) {
    glutLeaveMainLoop();
  }
  else if (caracter == 'f'){
    mode = 'f';
  }
  else if (caracter == 't'){
    mode = 't';
  }
}

void pintaTriangle(int idBoto, int presONo, int x, int y) {
  if (mode == 't' and contClick < 6 and idBoto == 0 and presONo == 1) {
    vertex[contClick] = x;
    vertex[contClick+1] = (height - y);


    //Converteix coordenades
    if (vertex[contClick] < (float) width / 2) {
      vertex[contClick] = 1 - ((float) vertex[contClick] / ((float) width / 2));
      vertex[contClick] *= -1;
    }
    else if (vertex[contClick] > (float) width / 2) {
      vertex[contClick] = ((float) (vertex[contClick] - ((float) width / 2)) / ((float) width / 2));
    }
    else {
      vertex[contClick] = 0.0;
    }

    if (vertex[contClick+1] < (float) width / 2) {
      vertex[contClick+1] = 1 - ((float) vertex[contClick+1] / ((float) width / 2));
      vertex[contClick+1] *= -1;
    }
    else if (vertex[contClick+1] > (float) width / 2) {
      vertex[contClick+1] = ((float) (vertex[contClick+1] - ((float) width / 2)) / ((float) width / 2));
    }
    else {
      vertex[contClick+1] = 0.0;
    }

    //fins aquí

    contClick += 2;
    if (contClick >= 5) {
      contClick = 0;
      glutPostRedisplay();
    }
  }
}

int main(int argc, char**argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(600, 600);

  glutCreateWindow("IDI: Examen OpenGL");

  //Triar color inicial
  glClearColor(blanc, blanc, blanc, 0.0);

  //Pinta la escena
  glutDisplayFunc(renderScene);

  //detecta click de ratoli, @params int idBoto, int presONo, int x, int y
  glutMouseFunc(pintaTriangle);

  //detecta click+arrastrar @params int x, int y
  glutMotionFunc(pinta);

  //detecta tecles de teclat @params unsigned char caracter, int x, int y
  glutKeyboardFunc(estat);

  //detecta redimensio finestra
  glutReshapeFunc(resize);

  glutMainLoop();
}