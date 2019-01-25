/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */
#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<windows.h>
#include<bits/stdc++.h>

#include "RGBpixmap.cpp"
#ifdef __APPLE__
#include<openGL/openGL.h>
#include<GLUT/glut.h>
#else
#include<GL/glut.h>
#endif
#define GAME_SCREEN 0			//Constant to identify background color
#define MENU_SCREEN 4
#define GAME_OVER_SCREEN 6


using namespace std;

float ballX = 0.0f;
float ballY = 0.5f;
float ballZ = 0.0f;
float score = 0.0f;
bool gameOver = false;
float testX = 2;
float testZ = -2;
float testZPos = 2;
static float mouseX=-100 ,mouseY=-100 ;

bool mButtonPressed= false,startGame=false;

bool startScreen = true ,nextScreen=false,previousScreen=false;
bool gameQuit = false,instructionsGame = false, optionsGame = false;
/*
float ballX2 = 0.3f;
float ballY2 = 0.0f;
float ballZ2 = -1.0f;
*/
bool dirX = true;

static int flag=1;

float angle=0.0;
float lx=0.0f,lz=-1.0f,ll=0.0f;
float ballPos = 0;
static int slices = 16;
static int stacks = 16;
static int dx = 0;
static int dy = 0;
static int dz = 8;
float xx=0,zz=0;
float x=1.0f,z=5.0f;
float tableStandWidth = 0.2f;
float translate_x,translate_y,translate_z;
float scale_x,scale_y,scale_z;
extern unsigned int sleep (unsigned int __seconds);

int cubeCnt = 0;
RGBpixmap pix[6];
float xxx[100],zzzPos[100],zzz[100];


/* GLUT callback Handlers */

void displayRasterText(float x ,float y ,float z ,char *stringToDisplay)
{
    int length;
    glRasterPos3f(x, y, z);
    length = strlen(stringToDisplay);

    for(int i=0 ; i<length ; i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24 ,stringToDisplay[i]);
    }
}

void SetDisplayMode(int modeToDisplay)
{
    switch(modeToDisplay)
    {
    case GAME_SCREEN:
        glClearColor(0, 0, 1, 1);
        break;
    case MENU_SCREEN :
        glClearColor(0, 1 , 0, 1);
        break;
    case GAME_OVER_SCREEN :
        glClearColor(1, 1 , 0, 1);
        break;
    }
}

GLdouble ox=0.0,oy=0.0,oz=0.0;
void Mouse(int button,int state,int x,int y)
{
    GLint viewport[4];
    GLdouble modelview[16],projection[16];
    GLfloat wx=x,wy,wz;

    if(state!=GLUT_DOWN)
        return;
    if(button==GLUT_RIGHT_BUTTON)
        exit(0);
    glGetIntegerv(GL_VIEWPORT,viewport);
    y=viewport[3]-y;
    wy=y;
    glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
    glGetDoublev(GL_PROJECTION_MATRIX,projection);
    glReadPixels(x,y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&wz);
    gluUnProject(wx,wy,wz,modelview,projection,viewport,&ox,&oy,&oz);

    mouseX = ox;
    mouseY = oy;
    //printf("%f\n",mouseX);
    mButtonPressed = true;
    glutPostRedisplay();
}



void Init()
{
    glEnable(GL_TEXTURE_2D);


    pix[0].makeCheckImage();
    pix[0].setTexture(1);

    pix[1].readBMPFile("D:\\4th year 2nd term\\GraphicsPractice\\ProjectDemo3\\zigzag2.bmp");
    pix[1].setTexture(2);

    pix[2].readBMPFile("D:\\4th year 2nd term\\GraphicsPractice\\ProjectDemo3\\zigzag2.bmp");
    pix[2].setTexture(3);

    pix[3].readBMPFile("D:\\4th year 2nd term\\GraphicsPractice\\ProjectDemo4\\football.bmp");
    pix[3].setTexture(4);

    pix[4].readBMPFile("D:\\4th year 2nd term\\GraphicsPractice\\ProjectDemo3\\ball1.bmp");
    pix[4].setTexture(5);



}

void GameOverScreen()
{
    SetDisplayMode(GAME_OVER_SCREEN);
    glColor3f(0,0,0);
    glLineWidth(50);
    glBegin(GL_LINE_LOOP);               //Border
    glVertex3f(-650 ,-500 ,0.5);
    glVertex3f(-650 ,520 ,0.5);
    glVertex3f(650 ,520 ,0.5);
    glVertex3f(650 ,-500, 0.5);
    glEnd();

    glLineWidth(1);
//	stoneTranslationSpeed=5;
    glColor3f(0, 1, 0);
    glBegin(GL_POLYGON);				//GAME OVER
    glVertex3f(-10 ,10,0.5);
    glVertex3f(-10 ,10 ,0.5);
    glVertex3f(10 ,10 ,0.5);
    glVertex3f(10 ,10, 0.5);
    glEnd();

    glColor3f(1, 1, 0);
    glBegin(GL_POLYGON);				//RESTART POLYGON
    glVertex3f(-200, 50 ,0.5);
    glVertex3f(-200 ,150 ,0.5);
    glVertex3f(200 ,150 ,0.5);
    glVertex3f(200 ,50, 0.5);
    glEnd();

    glBegin(GL_POLYGON);				//QUIT POLYGON
    glVertex3f(-200 ,-200 ,0.5);
    glVertex3f(-200 ,-100 ,0.5);
    glVertex3f(200, -100 ,0.5);
    glVertex3f(200, -200 ,0.5);
    glEnd();


    displayRasterText(-2 ,5 ,0.4 ,"G A M E    O V E R ! ! !");
    glColor3f(0 , 0, 0);

    //displayRasterText(-2 ,2 ,0.4 ,"SCORE: ");
    glColor3f(0 , 0, 0);
    char temp[40];

    int Score = ceil(score);
    sprintf(temp,"SCORE = %d",Score);

    displayRasterText(0 ,2 ,0.4 ,temp);




}



static void resize(int w, int h)
{
    /*const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;*/
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective

    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
    gluPerspective(45.0,                  //The camera angle
                   (double)w / (double)h, //The width-to-height ratio
                   1.0,                   //The near z clipping coordinate
                   200.0);                //The far z clipping coordinate

}

void drawWalls()
{


    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;
    translate_x = 3;
    translate_y = 0;
    translate_z = -2;

    scale_x = 2;
    scale_y = 0.4;
    scale_z = 3;


    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(0,0.5,0.5);
    glPushMatrix();
    glTranslated(0,-1,-1);
    glRotated(30,1,0,0);

    //Zigzag
    glPushMatrix();
    glTranslated(0,0,0);
    glScaled(4,0.4,4);
    glutSolidCube(1);
    glPopMatrix();

    xxx[0]=2;
    zzzPos[0]=2;
    zzz[0]=-2;



    glPushMatrix();
    glTranslated(3,0,-2);
    glScaled(2,0.4,3);
    glutSolidCube(1);
    glPopMatrix();

    xxx[1]=4;
    zzzPos[1]= -0.5;
    zzz[1] = -3.5;

    testX = 4;
    testZ = -3.5;
    testZPos = 0.5;



    glColor3d(0,0.5,0.5);

    for(int i = 0; i<15; i++)
    {
        translate_x = translate_x + scale_x;
        translate_z = translate_z - 2;

        glPushMatrix();
        glTranslated(translate_x,translate_y,translate_z);
        glScaled(scale_x,scale_y,scale_z);
        glutSolidCube(1);
        glPopMatrix();


        testX = translate_x + 1;
        testZ = translate_z - 1.5;
        testZPos = translate_z + 1.5;

        xxx[i+2] = testX;
        zzz[i+2] = testZ;
        zzzPos[i+2] = testZPos;


        //cout<<translate_z<<endl;

    }

    glPopMatrix();

}

void drawBall(void)
{


    glPushMatrix();
    glTranslated(0,-1,-1);
    glRotated(30,1,0,0);

    glPushMatrix();
    glColor3d(1,0,0);
    glTranslated(ballX,ballY,ballZ);
    glRotated(60,1,0,0);
    //glRotated(a,1,1,1);
    glScaled(1.5,1.5,1.5);
    glutSolidSphere(0.1,10,10);
    glPopMatrix();
    glPopMatrix();

}

void gameScreenDisplay()
{
    SetDisplayMode(GAME_SCREEN);


    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D,2);
    glEnable(GL_TEXTURE_2D);
    glTranslated(-2,0,6);
    drawWalls();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();



    glPushMatrix();
    glTranslated(-2,-0.3,6);
    drawBall();
    glPopMatrix();
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    /*if(startScreen)
    {
        startScreenDisplay();
    }*/

    if(gameOver)
    {
        GameOverScreen();
    }


    else
    {
        gameScreenDisplay();
    }




    //glutSwapBuffers();
}

void changeSize(int w, int h)
{
    if (h == 0)
        h = 1;
    float ratio =  w * 1.0 / h;

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(100.0f, ratio, 0.5f, 100.0f);

    glMatrixMode(GL_MODELVIEW);
}

bool rot = true;
void renderScene()
{

    const double t = glutGet(GLUT_ELAPSED_TIME) / 1900.0;
    double a;

    if(rot)
    {
        a = t*90.0;
    }
    else
    {
        a = t;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(	x + 5*sin(ll), 2.0f, z + 5*cos(ll),
                x+lx, 1.0f,  z+lz,
                0.0f, 1.0f,  0.0f);


    glPushMatrix();
    glRotated(0,0.5,1,0);
    //glRotated(a,0,1,0);
    display();
    glPopMatrix();
    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27 :
    case 'q':
        exit(0);
        break;

    case 'z':
        dz++;
        stacks++;
        break;

    case 'x':
        dx++;
        break;

    case 'X':
        dx--;
        break;

    case 'Y':
        dy++;
        break;

    case 'y':
        dy--;
        break;

    case 'a' :
        xx += .01 * .1;
        //zz += .1 * .5;
        gluLookAt(	0.0f, 1.0f, 5.0f,
                    xx+0.0f, 1.0f,  2.0f,
                    0.0f, 1.0f,  0.0f);
        break;
    case 'b' :
        xx -= .01 * .1;
        //zz -= .01 * .5;
        gluLookAt(	0.0f, 1.0f, 5.0f,
                    xx+1.0, 1.0f, 2.0,
                    0.0f, 1.0f,  0.0f);
        break;
    case 'Z':
        if (slices>3 && stacks>3)
        {
            dz--;
            stacks--;
        }
        break;
    }

    glutPostRedisplay();
}

void keyPress(int key, int x, int y)
{
    if(key==GLUT_KEY_RIGHT)
    {
        ballX += 0.05f;
        dirX = true;
        startGame = true;
    }
    if(key==GLUT_KEY_UP)
    {
        ballZ  -= 0.05f;
        dirX = false;
        startGame = true;
    }

    glutPostRedisplay();
}


static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };

/* Program entry point */

void processSpecialKeys(int key, int x, int y)
{

    float fraction = 0.3f;

    switch (key)
    {

    case GLUT_KEY_LEFT :
        angle -= 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_RIGHT :
        angle += 0.05f;
        lx = sin(angle);
        lz = -cos(angle);
        break;
    case GLUT_KEY_UP :
        x += lx * fraction;
        z += lz * fraction;
        break;
    case GLUT_KEY_DOWN :
        x -= lx * fraction;
        z -= lz * fraction;
        break;
    case GLUT_KEY_HOME :
        ll += .05;
        break;
    }

    glutPostRedisplay();

}

void update(int value)
{
    int flag = 0;
    for(int i=0; i<7; i++)
    {
        if(ballZ>=zzz[i]&&ballZ<=zzzPos[i])
        {
            if(ballX<=xxx[i])
            {
                flag = 1;
                //break;
            }
            //break;
        }
    }




    if(flag==0)
    {
        gameOver = true;
        //GameOverScreen();
    }

    if(gameOver)
    {
        ballY -= 0.05f;
    }

    flag = 0;

    for(int i=0; i<7; i++)
    {

        if(ballX<=xxx[i])
        {
            if(ballZ>=zzz[i]&&ballZ<=zzzPos[i])
            {
                flag = 1;
            }
            break;
        }


    }

    if(flag==0)
    {
        gameOver = true;

    }

    if(gameOver)
    {
        ballY -= 0.05f;
        //glColor3d(1,0,0);
        //displayRasterText(0 ,8 ,0 ,"Game OVER");
    }



    if(dirX&&!gameOver&&startGame)
    {
        flag = 0;
        ballX += 0.1f;
        score += .1;
    }

    if(!dirX&&!gameOver&&startGame)
    {
        flag = 0;
        ballZ -= 0.1f;
        score +=.1;
    }

    glutPostRedisplay(); //Tell GLUT that the display has changed

    //Tell GLUT to call update again in 25 milliseconds
    glutTimerFunc(25, update, 0);

}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(1300,720);
    //glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    //glutIdleFunc(renderScene);
    glutSpecialFunc(keyPress);
    //glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutMouseFunc(Mouse);


    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);

    glutTimerFunc(25, update, 0);
    Init();


    glutMainLoop();

    return EXIT_SUCCESS;
}
