#include <GL/gl.h>
#include <GL/freeglut.h>

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  //No es veu res perque estas sortint de la escena
    glVertex3f(-0.5, -0.5, 1.1);
    glVertex3f(0.5, -0.5, 1.1);
    glVertex3f(0.0, 0.5, 1.1);
  glEnd();
  glutSwapBuffers();
}

void resize(int w, int h) {
  if (w > h){
      glViewport(w/2-h/2, 0, h, h);
  }
  else {
    glViewport(0, h/2-w/2, w, w);
  }
}

int main(int argc, char**argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize(600, 600);
  glutCreateWindow("IDI: Practiques OpenGL");
  glutDisplayFunc(renderScene);

  glutReshapeFunc(resize);

  glutMainLoop();
}