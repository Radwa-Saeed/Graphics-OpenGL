#include <GL/glut.h>
#include <math.h>
#include <stdlib.h>

#include "imageloader.h"
#include "glm.h"
#include <iostream>
using namespace std;
#define PI 3.14
//camera
double eye[] = { 0, 0, 30 };
double center[] = { 0, 0, 0 };
double up[] = {0.0, 1.0, 0.0};

const char *modelname = "data/clicClac.obj";
GLMmodel* pmodel1;
const char *modelname2 = "data/taburet1_update.obj";
GLMmodel* pmodel2;
const char *modelname3 = "data/apple-ibook-2001.obj";
GLMmodel* pmodel3;


static int shoulderz = 0,shoulderx = 0, elbow = 0 , body =90; 
static int fingerBase1 = 0, fingerUp1 = 0; 
static int fingerBase2 = 0, fingerUp2 = 0; 
static int fingerBase3 = 0, fingerUp3 = 0; 
static int fingerBase4 = 0, fingerUp4 = 0; 
static int fingerBase5 = 0, fingerUp5 = 0; 
static int LeftLegz = 0, LeftLegx=-90 , LeftKneez =0,LeftKneex=90 ,LeftFoot=0;
static int shoulderz2 = 0,shoulderx2 = 0, elbow2 = 0 ; 
static int fingerBase21 = 0, fingerUp21 = 0; 
static int fingerBase22 = 0, fingerUp22 = 0; 
static int fingerBase23 = 0, fingerUp23 = 0; 
static int fingerBase24 = 0, fingerUp24 = 0; 
static int fingerBase25 = 0, fingerUp25 = 0; 
static int RightLegz = 0, RightLegx=-90 , RightKneez =0 ,RightKneex=90, RightFoot=0;
int moving, startx, starty;
GLfloat angle = 0.0;   /* in degrees */
GLfloat angle2 = 0.0;   /* in degrees */
GLuint _textureId; //The id of the texture

double direction[3];
double speed = 0.1;

// RGBA
GLfloat light_ambient1[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 0.5, 0.5, 0.5,1.0 };
GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0 };
// x , y, z, w
GLfloat light_position[] = {0.5, 5.0, 0.0, 0.0};
GLfloat lightPos1[] = {-0.5, -5.0, -2.0, 0.0};
//
// Material Properties
GLfloat mat_amb_diff[] = {0.643, 0.753, 0.934, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = {100.0 };  

// Body Positions
static float xBody = -8.6;
static float yBody = -1.5;
static float zBody = -1.5;
//sofa position
static float xsofa = 5.0;
static float ysofa = -3.5;
static float zsofa = -10.0;
//table position
static float xtable = 2.0;
static float ytable = -4.0;
static float ztable = 12.0;

//table position
static float xlab = 10.0;
static float ylab = 0;
static float zlab = -2.0;

void drawmodel(char *modelname ,GLMmodel* pmodel)
{
	GLMmodel *model = glmReadOBJ(modelname);
	glmUnitize(model);
	glmFacetNormals(model);
	glmVertexNormals(model, 90.0);
	glmScale(model, .15);
	glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);			 //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
				 0,							  //0 for now
				 GL_RGB,					  //Format OpenGL uses for image
				 image->width, image->height, //Width and height
				 0,							  //The border of the image
				 GL_RGB,					  //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE,			  //GL_UNSIGNED_BYTE, because pixels are stored
											  //as unsigned numbers
				 image->pixels);			  //The actual pixel data
	return textureId;						  //Returns the id of the texture
}

//Initializes 3D rendering
//char modelname1[] = "images/wood.bmp";
void initRendering(const char* bmbImg,GLuint & textureID)
{
	Image *image = loadBMP(bmbImg);
	textureID = loadTexture(image);
	delete image;
	// Turn on the power
	glEnable(GL_LIGHTING);
	// Flip light switch
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	// assign light parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    //glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties
	GLfloat lightColor1[] = {1.0f, 1.0f, 1.0f, 1.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	glEnable(GL_NORMALIZE);
	//Enable smooth shading
	//glShadeModel(GL_SMOOTH);
	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void init(void)
{
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glShadeModel(GL_FLAT);

   glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0, 1.0, 1.0, 20.0);
	//glMatrixMode(GL_MODELVIEW);
}

void reset()
{
	double e[] = {0.0, 0.0, 2};
	double c[] = {0.0, 0.0, 1.0};
	double u[] = {0.0, 1.0, 0.0};
	for (int i = 0; i < 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}

//int BallPoses[8][3]={{45,0,0},{90,0,0},{45,0,0},{0,0,0},{0,-20,-40},{0,-40,-80},{0,-20,-40},{0,0}};
int BallPoses[8][6]={{-30,0,-2,0,-90,0},{-45,0,-2,0,-90,0},{-50,20,-5,0,-100,0},{-20,30,-10,-1,-130,0},{-10,20,-20,-2,-145,20},{0,0,-20,-3,-160,45},{0,0,-30,-4,-170,100},{0,0,-40,-5,-170,135}};
static int ballAngle=0;
GLfloat speed1=0,verTheta =0, horTheta=0;
int frame =0 ;

int walkposes[8][6]={{-45,45,30,30,30,-20},{0,30,-45,45,-20,30},{-45,45,30,30,30,-20},{0,30,-45,45,-20,30},{-45,45,30,30,30,-20},{0,30,-45,45,-20,30},{-45,45,30,30,30,-20},{0,30,-45,45,-20,30}};
static int walking=0;
int frame1 =0 ;

static int labx=0;
static int laby=-90;
void setPoses(int frame){
    if(zlab !=-40 && xBody >=7){
    shoulderx=BallPoses[frame][0];
    shoulderz=BallPoses[frame][1];
    zlab=BallPoses[frame][2];
    ylab=BallPoses[frame][3];    
    laby=BallPoses[frame][4];
    labx=BallPoses[frame][5];
    } 
    else{
        body=0;
        //shoulderz=45;
        //elbow=90;
    }   

}
void timer(int value)
{
    frame = frame % 10;
    setPoses(frame);
    frame++;
    glutPostRedisplay();
    glutTimerFunc(300, timer, 0);

   }
void setPoses1(int frame){
    if(xBody <= -8.6){
        RightLegx=0;
        RightKneex=0;
        LeftLegx=0;
        LeftKneex=0;
        xBody=-6.5;
        yBody=-0.2;
        shoulderx=0;
        shoulderx2=0;
        frame=0;
    }
    else if(xBody >=-8 && xBody <=7){
         RightLegx=walkposes[frame1][0];
         RightKneex=walkposes[frame1][1];
         LeftLegx=walkposes[frame1][2];
         LeftKneex=walkposes[frame1][3];
         shoulderx=walkposes[frame1][4];
         shoulderx2=walkposes[frame1][5];
         xBody+=2;
        }
    else{
        xBody==6;
        RightLegx=0;
        RightKneex=0;
        LeftLegx=0;
        LeftKneex=0;
        shoulderx=0;
        shoulderx2=0;
    }    

}
void timer2(int value)
{
    frame1 = frame1 % 10;
    setPoses1(frame1);
    frame1++;
    glutPostRedisplay();
    glutTimerFunc(600, timer2, 0);

   }
void display(void)
    {
    glClearColor(0.952, 0.745, 0.607, 1.0);
    // Clear Depth and Color buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);

    //********** texturing **********
    glPushMatrix();
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position); 
    glRotatef(angle2, 1.0, 0.0, 0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glPushMatrix();
    glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBegin(GL_QUADS);
            glNormal3f(0.0, 1.0, 0.0);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-20, -6,  30);
            glTexCoord2f(3.0f, 0.0f); glVertex3f( 20, -6,  30);
            glTexCoord2f(3.0f, 3.0f); glVertex3f( 20, -6, -70);
            glTexCoord2f(0.0f, 3.0f); glVertex3f(-20, -6, -70);
        glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    //************* placing objects **************
    glPushMatrix();
        glRotatef (90, 0.0, 1.0, 0.0);
        glTranslatef(xsofa,ysofa,zsofa);
        glScalef(40.0,40.0,40.0);
        glColor3b(5,5,5);
        drawmodel((char *)modelname,pmodel1);
	glPopMatrix();
    
    glPushMatrix();
        glRotatef (90, 0.0, 1.0, 0.0);
        glTranslatef(xtable,ytable,ztable);
        glScalef(40.0,40.0,40.0);
        glColor3b(5,5,5);
        drawmodel((char *)modelname2,pmodel2);
	glPopMatrix();

    glPushMatrix();
        glTranslatef(xlab,ylab,zlab);
        glRotatef ((GLfloat) laby, 0.0, 1.0, 0.0);
        glRotatef ((GLfloat) labx, 1.0, 0.0, 0.0);
        glTranslatef(0,0,0);
        glScalef(8.0,8.0,8.0);
        glColor3b(5,5,5);
        drawmodel((char *)modelname3,pmodel3);
	glPopMatrix();

    //glPushMatrix();
    glTranslatef (xBody , yBody, zBody); // location of the robot 
    glRotatef ((GLfloat) body, 0.0, 1.0, 0.0);
    glTranslatef (0.0, 0.0, 0.0);
        glPushMatrix();
        glScalef (1.5, 4.0, 0.5);
        glutWireCube (1.0);    
        glPopMatrix();
        //   Head
        glPushMatrix();
        glTranslatef (0.0, 2.6 , 0.0);
        glutWireSphere(0.42 , 10 , 10);
        glPopMatrix();
        //    Right Arm
        glPushMatrix();
        glTranslatef (-0.9, 1.85, 0.0);
        glRotatef ((GLfloat) shoulderz2, 0.0, 0.0, 1.0);
        glRotatef ((GLfloat) shoulderx2, 1.0, 0.0, 0.0);
        glTranslatef (0.0, -0.55, 0.0);
        glPushMatrix();
        glScalef (0.3, 1.25, 0.3);
        glutWireCube (1.0);
        glPopMatrix();

        glTranslatef (0.0, -0.6, 0.0);
        glRotatef ((GLfloat) elbow2, 0.0, 0.0, 1.0);
        glTranslatef (0.0, -0.65, 0.0);
        glPushMatrix();
        glScalef (0.3, 1.25, 0.3);
        glutWireCube (1.0);
        glPopMatrix();

        //**********1
        glPushMatrix();
        glTranslatef(-0.1, -0.35, -0.1); 
        glRotatef((GLfloat)fingerBase1, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp1, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        //*************2
        glPushMatrix();
        glTranslatef(-0.1, -0.35, 0.0); 
        glRotatef((GLfloat)fingerBase2, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        //*************3
        glPushMatrix();
        glTranslatef(-0.1, -0.35, 0.1); 
        glRotatef((GLfloat)fingerBase3, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp3, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        //*************4
        glPushMatrix();
        glTranslatef(0.1, -0.35, 0.0); 
        glRotatef((GLfloat)fingerBase4, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp4, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        glPopMatrix();

    //   Left Arm
        glPushMatrix();
        //glRotatef(angle2, 1.0, 0.0, 0.0);
        //glRotatef(angle, 0.0, 1.0, 0.0);
        glTranslatef (0.9, 1.85, 0.0);
        glRotatef ((GLfloat) shoulderz, 0.0, 0.0, 1.0);
        glRotatef ((GLfloat) shoulderx, 1.0, 0.0, 0.0);
        glTranslatef (0.0, -0.55 , 0.0);
        glPushMatrix();
        glScalef (0.3, 1.25, 0.3);
        glutWireCube (1.0);
        glPopMatrix();

        glTranslatef (0.0, -0.6, 0.0);
        glRotatef ((GLfloat) elbow, 0.0, 0.0, 1.0);
        glTranslatef (0.0, -0.65, 0.0);
        glPushMatrix();
        glScalef (0.3, 1.25, 0.3);
        glutWireCube (1.0);
        glPopMatrix();

        //**********1
        glPushMatrix();
        glTranslatef(0.1, -0.35, -0.1); 
        glRotatef((GLfloat)fingerBase1, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp1, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        //*************2
        glPushMatrix();
        glTranslatef(0.1, -0.35, 0.0); 
        glRotatef((GLfloat)fingerBase2, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp2, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        //*************3
        glPushMatrix();
        glTranslatef(0.1, -0.35, 0.1); 
        glRotatef((GLfloat)fingerBase3, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp3, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();
        //*************4
        glPushMatrix();
        glTranslatef(-0.1, -0.35, 0.0); 
        glRotatef((GLfloat)fingerBase4, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.4, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glTranslatef(0.0, -0.1, 0.0);
        glRotatef((GLfloat)fingerUp4, 0.0, 0.0, 1.0);
        glTranslatef(0.0, -0.15, 0.0);
        glPushMatrix();
        glScalef(0.1, 0.3, 0.1);
        glutWireCube(1);
        glPopMatrix();
        glPopMatrix();

        glPopMatrix();


        //   Right Leg
        glPushMatrix();
        glTranslatef (-0.55, -2.0, 0.0);
        glRotatef ((GLfloat) RightLegz, 0.0, 0.0, 1.0);
        glRotatef((GLfloat)RightLegx,1.0,0.0,0.0);
        glTranslatef (0.05, -0.85, 0.0);

        glPushMatrix();
        glScalef (0.5, 1.7, 0.5);
        glutWireCube (1.0);
        glPopMatrix();

        glTranslatef (0.0, -0.8, 0.0);
        glRotatef ((GLfloat) RightKneez, 0.0, 0.0, 1.0);
        glRotatef((GLfloat)RightKneex,1.0,0.0,0.0);
        glTranslatef (0.0, -0.9, 0.0);

        glPushMatrix();
        glScalef (0.5, 1.7, 0.5);
        glutWireCube (1.0);
        glPopMatrix();

        glTranslatef (0.0, -1.0, 0.0);
        glRotatef ((GLfloat) RightFoot, 0.0, 0.0, 1.0);
        glTranslatef (0.0, 0.0, 0.25);
        glPushMatrix();
        glScalef (0.55, 0.2, 1.0);
        glutWireCube (1.0);
        glPopMatrix();

        glPopMatrix();

        //  Left Leg
        glPushMatrix();
        glRotatef(angle2, 1.0, 0.0, 0.0);
        glRotatef(angle, 0.0, 1.0, 0.0);
        glTranslatef (0.55, -2.0, 0.0);
        glRotatef ((GLfloat) LeftLegz, 0.0, 0.0, 1.0);
        glRotatef ((GLfloat) LeftLegx, 1.0, 0.0, 0.0);
        glTranslatef (-0.05, -0.85, 0.0);   
        glPushMatrix();
        glScalef (0.5, 1.7, 0.5);
        glutWireCube (1.0);
        glPopMatrix();

        glTranslatef (0.0, -0.8, 0.0);
        glRotatef ((GLfloat) LeftKneez, 0.0, 0.0, 1.0);
        glRotatef ((GLfloat) LeftKneex, 1.0, 0.0, 0.0);
        glTranslatef (0.0, -0.9, 0.0);
        glPushMatrix();
        glScalef (0.5, 1.7, 0.5);
        glutWireCube (1.0);
        glPopMatrix();

        glTranslatef (0.0, 0.0, 0.0);
        glRotatef ((GLfloat) LeftFoot, 0.0, 0.0, 1.0);
        glTranslatef (0.0, -1.0, 0.25);
        glPushMatrix();
        glScalef (0.55, 0.2, 1.0);
        glutWireCube (1.0);
        glPopMatrix();

        glPopMatrix(); 
        
    glPopMatrix();
    glutSwapBuffers();

}

// Rotation about vertical direction
void lookRight()
{
	rotatePoint(up, PI / 20, eye);
}

void lookLeft()
{
	rotatePoint(up, -PI / 20, eye);
}

// Rotation about horizontal direction

void lookUp()
{
	double horizontal[3];
	double look[] = {center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]};
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, PI / 20, eye);
	rotatePoint(horizontal, PI / 20, up);
}

void lookDown()
{
	double horizontal[3];
	double look[] = {center[0] - eye[0], center[1] - eye[1], center[2] - eye[2]};
	crossProduct(up, look, horizontal);
	normalize(horizontal);
	rotatePoint(horizontal, -PI / 20, eye);
	rotatePoint(horizontal, -PI / 20, up);
}

// Forward and Backward
void moveForward()
{
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	double speed = 0.1;
	eye[0] += direction[0] * speed;
	eye[1] += direction[1] * speed;
	eye[2] += direction[2] * speed;

	center[0] += direction[0] * speed;
	center[1] += direction[1] * speed;
	center[2] += direction[2] * speed;
}

void moveBackword()
{
	double direction[3];
	direction[0] = center[0] - eye[0];
	direction[1] = center[1] - eye[1];
	direction[2] = center[2] - eye[2];
	double speed = 0.1;
	eye[0] -= direction[0] * speed;
	eye[1] -= direction[1] * speed;
	eye[2] -= direction[2] * speed;

	center[0] -= direction[0] * speed;
	center[1] -= direction[1] * speed;
	center[2] -= direction[2] * speed;

}



/**yBody=0;
    //xBody=-6;
    //LeftLegx=0;
    //LeftKneex=0;
    //RightLegx=0;
    //RightKneex=0;
    if (xBody==-7.5){
        xBody=-6;
        LeftLegx=0;
        LeftKneex=0;
        RightLegx=0;
        RightKneex=0;**/
static bool isTable = false;
static int leg_state = 0;
void animation1(int value)
{
    /*
     * Description:
     * This function is responsible for moving the body forward in one direction.
     *
     * First we check if the body reaches the end of the floor or no, then
     * the body should stop and return to normal state.
     *
     * Cases:
     * Case 1: Move the body in the forward direction
     *         Move rhe right leg and the right knee forward.
     *         Move the left leg and the left knee backward.
     *         Move the right arm forward.
     *         Move the left arm backward.
     *
     * Case 2: Continue moving the body in the forward direction
     *         Move rhe right leg and the right knee backward.
     *         Move the left leg and the left knee forward.
     *         Move the right arm backward.
     *         Move the left arm forward.
     *
     * Case 3: The body reached the end of the floor
     *
     * Case 4: The walkposesing process has finished
     */

    // Check if the body reached the table
    if (xBody >19 && xBody <25) {
          isTable = true;
            
    }

    // Check if the body exceeded the table
    if (xBody <=20) { 
        if (xBody==-7.5){
            xBody=-6.5;
            LeftLegx=0;
            LeftKneex=0;
            RightLegx=0;
            RightKneex=0;        
            yBody=0;
            glutTimerFunc(30, animation1, 0);
        }
        else{
          isTable = false;  
        }
        //isTable = false;        
    }
    // Reached start of the table
    if (xBody > -9 && isTable) {
        leg_state = 3; //********************************
        // Return Body to normal state
        RightLegx = 0;
        RightKneex = 0;
        LeftLegx = 0;
        LeftKneex = 0;
        shoulderx = 0;
        shoulderx2 = 0;
        shoulderz = 0;
        shoulderz2 = 0;
    }

    // Reached End of the floor
    if (xBody > 11.5) {
        leg_state = 4; //**********************************
        // Return Body to normal state
        RightLegx = 0;
        RightKneex = 0;
        LeftLegx = 0;
        LeftKneex = 0;

        //shoulderx = -90;
        //shoulderx2 = 90;
        shoulderx = 0;
        shoulderx2 = 0;
    }

    switch (leg_state)
    {
        case 1:
            if (RightLegx > -45 && LeftLegx <45 ) {
                // Move Body
                // Check on movement's direction
                if (body == 0) {
                    zBody += 0.03;
                } else if (body == -90 || body == 270) {
                    xBody += 0.03;
                } else if (body == -180 || body == 180) {
                    zBody -= 0.03;
                } else if (body == -270 || body == 90) {
                    xBody += 0.03;
                }
                // Move Right Leg Forward
                RightLegx -= 1;
                RightKneex += 1;
                LeftLegx += 1;
                LeftKneex += 1;
                // Move Left Leg Backward
                
                // Move Arms
                if (shoulderx < 45 && shoulderx2 >-45)
                {
                    shoulderx += 1;
                    shoulderx2 -= 1;
                }
            } else {
                leg_state = 2;
            }
            glutTimerFunc(10, animation1, 0);
            break;

        case 2:
            if (RightLegx < 0 && LeftLegx > 0) {
                // Move Body
                // Check on movement's direction
                if (body == 0) {
                    xBody += 0.03;
                } else if (body == -90 || body == 270) {
                    zBody += 0.03;
                } else if (body == -180 || body == 180) {
                    xBody -= 0.03;
                } /**else if (body == -270 || body == 90) {
                    zBody -= 0.03;
                }**/

                // Move Right Leg
                RightLegx += 1;
                RightKneex -= 1;
                LeftLegx -= 1;
                LeftKneex -= 1;
                // Return Left Leg
                
                // Return Arms
                if (shoulderx > 0 && shoulderx2 < 0)
                {
                    shoulderx -= 1;
                    shoulderx2 += 1;
                }
            }
            else {
                leg_state = 0;
            }
            glutTimerFunc(10, animation1, 0);
            break;

        case 3:
            cout<<"Your Reached the start of the table"<<xBody<<endl;
            leg_state = 1;
            break;

        case 4:
            cout<<"Your Reached the end of the floor"<<xBody<<endl;
            leg_state = 1;
            break;

        case 0:
            leg_state = 1;
            break;

        default:
            break;

    }
    glutPostRedisplay();
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef(0.0, 0.0, -5.0);
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{  
        case GLUT_KEY_LEFT:
            lookLeft();
            break;
        case GLUT_KEY_RIGHT:
            lookRight();
            break;
        case GLUT_KEY_UP:
            lookUp();
            break;
        case GLUT_KEY_DOWN:
            lookDown();
            break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {    
        case 'm':
            timer2(0);
            glutPostRedisplay();
            break; 
        case 'n':
            timer(0);
            glutPostRedisplay();
            break;            
        //camera's keys
        case '+':
            moveForward();
            glutPostRedisplay();
            break;
        case '-':
            moveBackword();
            glutPostRedisplay();
            break;
        case '0':
            reset();
            glutPostRedisplay();
            break; 
        //   Left Arm
        // Shoulderzshoulderz 
        case 's':
            if (shoulderz < 180){
            shoulderz = (shoulderz + 5) % 360;
            glutPostRedisplay();
            }
            break;
        case 'S':
            if (shoulderz > -90){
            shoulderz = (shoulderz - 5) % 360;
            glutPostRedisplay();
            }
            break;
        case 'Y':
            if (shoulderx < 90){
            shoulderx = (shoulderx + 5) % 360;
            glutPostRedisplay();
            }
            break;
        case 'y':
            if (shoulderx > -180){
            shoulderx = (shoulderx - 5) % 360;
            glutPostRedisplay();
            }
            break;    
        // Elbow
        case 'e':
            if (elbow <90){
            elbow = (elbow + 5) % 180;
            glutPostRedisplay();
            }
            break;
        case 'E':
        if (elbow >-90){
            elbow = (elbow - 5) % 180;
            glutPostRedisplay();
            }
            break;

        // Right Arm
        // Shoulderzshoulderz
        case 'A':
            if (shoulderz2 <90){
            shoulderz2 = (shoulderz2 + 5) % 360;
            glutPostRedisplay();}
            break;
        case 'a':
            if (shoulderz2 >-180){
            shoulderz2 = (shoulderz2 - 5) % 360;
            glutPostRedisplay();}
            break;
        case 'G':
            if (shoulderx2 < 90){
            shoulderx2 = (shoulderx2 + 5) % 360;
            glutPostRedisplay();
            }
            break;
        case 'g':
            if (shoulderx2 > -180){
            shoulderx2 = (shoulderx2 - 5) % 360;
            glutPostRedisplay();
            }
            break;    
        // Elbow
        case 'W':
            if (elbow2 <90){
            elbow2 = (elbow2 + 5) % 180;
            glutPostRedisplay();}
            break;
        case 'w':
            if(elbow2 > -90){
            elbow2 = (elbow2 - 5) % 180;
            glutPostRedisplay();}
            break;

        // Right Leg
        case 'x':
            if(RightLegx>-90){
                RightLegx = (RightLegx -5)%360;
                glutPostRedisplay();
            }
            break;
        case 'X':
            if (RightLegx < 90) {
                RightLegx = (RightLegx +5)%360;
                glutPostRedisplay();
            }
            break;
        case 'z':
            if(RightKneex>-20){
                RightKneex = (RightKneex -5)%360;
                glutPostRedisplay();
            }
            break;
        case 'Z':
            if (RightKneex < 90) {
                RightKneex = (RightKneex +5)%360;
                glutPostRedisplay();
            }
            break;  
        case 'D':
            if (RightLegz <90){
            RightLegz = (RightLegz + 5) % 360;
            glutPostRedisplay();}
            break;
        case 'd':
            if (RightLegz >-90){
            RightLegz = (RightLegz - 5) % 360;
            glutPostRedisplay();}
            break;
    
        case 'R':
            if (RightKneez <30){
            RightKneez = (RightKneez + 5) % 180;
            glutPostRedisplay();}
            break;
        case 'r':
            if (RightKneez >-30){
            RightKneez = (RightKneez - 5) % 180;
            glutPostRedisplay();}
            break;

        // Left Leg
        case 'f':
            if (LeftLegz < 90){
            LeftLegz = (LeftLegz + 5) % 360;
            glutPostRedisplay();}
            break;
        case 'F':
            if (LeftLegz >-90){
            LeftLegz = (LeftLegz - 5) % 360;
            glutPostRedisplay();}
            break;
    
        case 't':
            if (LeftKneez<50){
            LeftKneez = (LeftKneez + 5) % 180;
            glutPostRedisplay();}
            break;
        case 'T':
            if (LeftKneez >-50){
            LeftKneez = (LeftKneez - 5) % 180;
            glutPostRedisplay();}
            break;
        case 'c':
            if (LeftLegx >-90){
            LeftLegx = (LeftLegx - 5) % 360;
            glutPostRedisplay();}
            break;
        case 'C':
            if (LeftLegx <90){
            LeftLegx = (LeftLegx + 5) % 360;
            glutPostRedisplay();}
            break;
    
        case 'v':
            if (LeftKneex>-20){
            LeftKneex = (LeftKneex - 5) % 180;
            glutPostRedisplay();}
            break;
        case 'V':
            if (LeftKneex <90){
            LeftKneex = (LeftKneex + 5) % 180;
            glutPostRedisplay();}
            break;                
   case 27:
      exit(0);
      break;
   default:
      break;
   }
}

/*static void mouse(int button, int state, int x, int y)
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
*/

/* static void motion(int x, int y)
{
  if (moving) {
    angle = angle + (x - startx);
    angle2 = angle2 + (y - starty);
    startx = x;
    starty = y;
    glutPostRedisplay();
  }
}
 */

void Choose_texture(int id)
{
    /*
     * Description:
     *     This function is responsible for changing the texture of the floor
     *     using a drop menu by the right click on the mouse.
     */
    switch(id)
    {
        case 1:
            initRendering("images/f1.bmp",_textureId);
            break;
        case 2:
            initRendering("images/f2.bmp",_textureId);
            break;
        case 3:
            initRendering("images/f3.bmp",_textureId);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}
int main(int argc, char **argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(900, 900);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Task3-Part1");
   //init();
   initRendering("images/wood.bmp",_textureId);
    glutCreateMenu(Choose_texture);
        glutAddMenuEntry("Floor 1",1);
        glutAddMenuEntry("Floor 2",2);
        glutAddMenuEntry("Floor 3",3);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

   glMatrixMode(GL_PROJECTION);
   gluPerspective(60, 1.0, 0.1, 150);
   //glutMouseFunc(mouse);
   glutDisplayFunc(display);
   //glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutSpecialFunc(specialKeys);
   /* glutMotionFunc(motion);  */ 
   glutMainLoop();
   return 0;
}