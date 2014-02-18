#include <GL/gl.h>
#include <GL/freeglut.h>

void renderScene(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
    glVertex3f(-0.5, -0.5, 0.0);
    glVertex3f(0.5, -0.5, 0.0);
    glVertex3f(0.0, 0.5, 0.0);
  glEnd();
  glutSwapBuffers();
}

void resize(int w, int h) {
  if (
  glViewport(0, 0, ample, alt);
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