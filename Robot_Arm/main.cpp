
/*
 * robot.c
 * This program shows how to composite modeling transformations
 * to draw translated and rotated hierarchical models.
 * Interaction:  pressing the s and e keys (shoulder and elbow)
 * alters the rotation of the robot arm.
 */
#include <GL/glut.h>
#include <stdlib.h>

static int shoulder = 0, elbow = 0; 
static int fingerBase1 = 0, fingerUp1 = 0;
static int fingerBase2 = 0, fingerUp2 = 0;
static int fingerBase3 = 0, fingerUp3 = 0;
static int fingerBase4 = 0, fingerUp4 = 0;

int moving, startx, starty;

GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);
}

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glRotatef(angle2, 1.0, 0.0, 0.0);
   glRotatef(angle, 0.0, 1.0, 0.0);
   // Whole arm rotation
   // All of arm is rotating around x = -1
   glTranslatef (-3.0, 0.0, 0.0);
   glRotatef ((GLfloat) shoulder, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   //Draw the shoulder centered at origin has x=2, y=0.6, z=1
   //where x => width , y => hight , z => depth 
   glPushMatrix();
   glScalef (2.0, 0.6, 1.0); /* modeling transformation */
   glutWireCube (1.0);  /*  draw the cube */
   glPopMatrix();

   // Elbow and below rotation
   // When the elbow first drawn, it is rotated around x = -1
   // But then shifted in X by 1
   glTranslatef (1.0, 0.0, 0.0);
   glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
   glTranslatef (1.0, 0.0, 0.0);
   //Drawing the elbow
   glPushMatrix();
   glScalef (2.0, 0.6, 1.0);
   glutWireCube (1.0);
   glPopMatrix();

   
   
   
   
   //Finger 1
   glPushMatrix();
      // Rotation of finger base 1
      glTranslatef(1.0, 0.2, 0.0); 
      glRotatef((GLfloat)fingerBase1, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      // Draw finger base 1
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.2); // base1 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
      //Rotation of finger flang 1
      glTranslatef(0.15, 0.0, 0.0);
      glRotatef((GLfloat)fingerUp1, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      //Draw finger flang 1 
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.2); // up1 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
   glPopMatrix();

   //Finger 2
   glPushMatrix();
      // Rotation of finger base 2
      glTranslatef(1.0, 0.2, 0.0); 
      glRotatef((GLfloat)fingerBase2, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      // Draw finger base 2
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.0); // base2 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
      //Rotation of finger flang 2
      glTranslatef(0.15, 0.0, 0.0);
      glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      //Draw finger flang 2 
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.0); // up2 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
   glPopMatrix();

   //Finger 3
   glPushMatrix();
      // Rotation of finger base 3
      glTranslatef(1.0, 0.2, 0.0); 
      glRotatef((GLfloat)fingerBase3, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      // Draw finger base 3
      glPushMatrix();
      glTranslatef(0.0, 0.0, -0.2); // base3 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
      //Rotation of finger flang 3
      glTranslatef(0.15, 0.0, 0.0);
      glRotatef((GLfloat)fingerUp3, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      //Draw finger flang 3
      glPushMatrix();
      glTranslatef(0.0, 0.0, -0.2); // up3 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
   glPopMatrix();

   //Finger 4
   glPushMatrix();
      // Rotation of finger base 4
      glTranslatef(1.0, -0.2, 0.0); 
      glRotatef((GLfloat)fingerBase4, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      // Draw finger base 4
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.0); // base4 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
      //Rotation of finger flang 4
      glTranslatef(0.15, 0.0, 0.0);
      glRotatef((GLfloat)fingerUp4, 0.0, 0.0, 1.0);
      glTranslatef(0.15, 0.0, 0.0);
      //Draw finger flang 4
      glPushMatrix();
      glTranslatef(0.0, 0.0, 0.0); // up4 coordinates
      glScalef(0.3, 0.1, 0.1);
      glutWireCube(1);
      glPopMatrix();
   glPopMatrix();

   glPopMatrix();
   glutSwapBuffers();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h); /* define the viewport */
   glMatrixMode(GL_PROJECTION); /* prepare for and then */ 
   glLoadIdentity(); /* define the projection */
   gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0); /* transformation */
   glMatrixMode(GL_MODELVIEW); /* back to modelview matrix */
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
   case 'S':
      shoulder = (shoulder + 5) % 360;
      glutPostRedisplay();
      break;
   case 's':
      shoulder = (shoulder - 5) % 360;
      glutPostRedisplay();
      break;
   case 'e':
      if (elbow < 150)
            {
                elbow = (elbow + 5);
                glutPostRedisplay();
            }
            break;
   case 'E':
      if (elbow > 0)
            {
                elbow = (elbow - 5);
                glutPostRedisplay();
            }
            break;
   // FINGER 1
   //Base 1
   case 'F':
      if (fingerBase1 < 60) {
            fingerBase1 = (fingerBase1 + 5);
            glutPostRedisplay();
      }
      break;
   case 'f':
      if (fingerBase1 > -90) {
            fingerBase1 = (fingerBase1 - 5);
            glutPostRedisplay();
      }
      break;
   //up 1   
   case 'R':
      if (fingerUp1 < 0) {
            fingerUp1 = (fingerUp1 + 5);
            glutPostRedisplay();
      }
      break;
   case 'r':
      if (fingerUp1 > -90) {
            fingerUp1 = (fingerUp1 - 5);
            glutPostRedisplay();
      }
      break;

   // FINGER 2
   //base 2
   case 'G':
      if (fingerBase2 < 60) {
            fingerBase2 = (fingerBase2 + 5);
            glutPostRedisplay();
      }
      break;
   case 'g':
      if (fingerBase2 > -90) {
            fingerBase2 = (fingerBase2 - 5);
            glutPostRedisplay();
      }
      break;
   //up 2   
   case 'T':
      if (fingerUp2 < 0) {
            fingerUp2 = (fingerUp2 + 5);
            glutPostRedisplay();
      }
      break;
   case 't':
      if (fingerUp2 > -90) {
            fingerUp2 = (fingerUp2 - 5);
            glutPostRedisplay();
      }
      break;

   // FINGER 3
   //base 3
   case 'H':
      if (fingerBase3 < 60) {
            fingerBase3 = (fingerBase3 + 5);
            glutPostRedisplay();
      }
      break;
   case 'h':
      if (fingerBase3 > -90) {
            fingerBase3 = (fingerBase3 - 5);
            glutPostRedisplay();
      }
      break;
   //up 3
   case 'Y':
      if (fingerUp3 < 0) {
            fingerUp3 = (fingerUp3 + 5);
            glutPostRedisplay();
      }
      break;
   case 'y':
      if (fingerUp3 > -90) {
            fingerUp3 = (fingerUp3 - 5);
            glutPostRedisplay();
      }
      break;
   // FINGER 4
   //base 4
   case 'j':
      if (fingerBase4 < 90) {
            fingerBase4 = (fingerBase4 + 5);
            glutPostRedisplay();
      }
      break;
   case 'J':
      if (fingerBase4 > -60) {
            fingerBase4 = (fingerBase4 - 5);
            glutPostRedisplay();
      }
      break;
   // up 4   
   case 'u':
      if (fingerUp4 < 90) {
            fingerUp4 = (fingerUp4 + 5);
            glutPostRedisplay();
      }
      break;
   case 'U':
      if (fingerUp4 > 0) {
            fingerUp4 = (fingerUp4 - 5);
            glutPostRedisplay();
      }
      break;   

   case 27:
      exit(0);
      break;
   default:
      break;
   }
}

static void mouse(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON) {
    if (state == GLUT_DOWN) {
      moving = 1;
      startx = x;
      starty = y;
    }
    if (state == GLUT_UP) {
      moving = 0;
    }
  }
}


static void motion(int x, int y)
{
  if (moving) {
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}



int main(int argc, char **argv)
{
   /* Basics in Building */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize(900, 800);
   glutInitWindowPosition(100, 100);
   /* glutCreateWindow(argv[0]); */
   glutCreateWindow("Robotic Arm"); /* To Give the window title */
   
   init();
   glutMouseFunc(mouse);
   glutMotionFunc(motion);
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}