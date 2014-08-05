#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include <iostream>
using std::cout;
using std::endl;
using std::ios;
using std::string;
using namespace std;

#include "Grid.h"
#include "functions.h"

// Editado por Yuri
///////////////////////////////
// #include "PerfTimer.h"
#define hertz 500
///////////////////////////////

float   amplificationFactor = 1.0,
        xTranslationFactor  = 0.0,
        yTranslationFactor  = 0.0,
        zTranslationFactor  = 0.0;

int currentX = 1;
int mouseButton;
int iteration = 0;

bool    levelNotSet = true,
        programHasAlreadyRun = 0,
        continueRefining = true;

int     refinementLevel;
char    refinementLevelNumber[5];

float   mousePreviousX,
        mousePreviousY;

double  quadCenterX = 300,
        quadCenterY = 200,
        elapsedTime = 0;

Grid    *grid,
        *grid2;

Cell    *gridCell,
        *aux;

void myInit(void)
{
    //glClearColor (0.0, 0.0, 0.0, 1.0); // set black background color
    glClearColor (1.0, 1.0, 1.0, 1.0);   // set white background color
    glClear(GL_COLOR_BUFFER_BIT);        // clear a tela
    glMatrixMode(GL_PROJECTION);         // set coordinate system
    glLoadIdentity();
    gluOrtho2D(0.0, 600.0, 0.0, 600.0);
}

void myDisplay(void)
{
/* Editado por Yuri */
///////////////////////////////////////////////
   	double totalTime = 0;
    // CPerfTimer totalTime;
///////////////////////////////////////////////

    cout << "Start of Program." << "\n\n";

    refinementLevel =  fabs(currentX);
    if(levelNotSet)
    {
        elapsedTime = 0;
        glClear(GL_COLOR_BUFFER_BIT);      //clear a tela

        setColor(0);
        drawCenterString("Selecione o nivel de refinamento desejado:",GLUT_BITMAP_HELVETICA_18,500);

        drawPoint(100,200);
        drawPoint(150,200);
        drawPoint(200,200);
        drawPoint(250,200);
        drawPoint(300,200);
        drawPoint(350,200);
        drawPoint(400,200);
        drawPoint(450,200);
        drawPoint(500,200);

        drawLineScale(100,200,500,200);
        //drawQuad(95,95,105,105);
        drawQuad(quadCenterX-5,quadCenterY-5,quadCenterX+5,quadCenterY+5);

        setColor(5);
        drawQuad(270,285,320,320);
        setColor(7);
        drawCenterString("Rodar",GLUT_BITMAP_HELVETICA_12,300);

        setColor(0);
        string refinementString( "Nivel: " );
        sprintf(refinementLevelNumber, "%i", refinementLevel);
        refinementString.append( refinementLevelNumber );
        drawCenterString(refinementString.data(),GLUT_BITMAP_HELVETICA_18, 100);
        glFlush();
    }
    else
    {

        if(iteration == 0 & !programHasAlreadyRun )// && !programHasAlreadyRun)
        {
            glClear(GL_COLOR_BUFFER_BIT);

            grid2 = new Grid;
            grid2->initializeGrid( true );

            //grid2->createAuxiliarCells();  /*IMPORTANTEEEE! NÃO REMOVER*/
            grid2->refineGridInitially(refinementLevel, true);

            grid2->solveFullDarcy(true, refinementLevel);

            //grid = new Grid(*grid2);

            grid = new Grid();
            grid->initializeGrid( true );
            grid->refineGridInitially(fmin(refinementLevel, 8), true);
            coarseData(grid, grid2);

            grid->drawPeanoCurve(1);
            grid->drawPressure();
            programHasAlreadyRun = true;
            glFlush();
            iteration++;

        }
        if(iteration == 1)
        {
            /* Editado por Yuri */
            ////////////////////////////////////////////
            totalTime = glutGet(GLUT_ELAPSED_TIME) / hertz;
            // totalTime.Start();
            ////////////////////////////////////////////

            grid->setInitialCondition(3);
            grid->drawPeanoCurve(1);
            glFlush();
            iteration++;
        }

        if(iteration > 1)
        {

            if(grid->initialMass > 1e-5)//iteration > 1 & iteration < 3000)
            {
                //if(iteration > 2)
                //{
                //cout << "iteration " << iteration << endl;
                continueRefining = true;
                while(continueRefining)
                {
                    setGridFlux(grid);
                    continueRefining = grid->refineGrid(refinementLevel, 1e-3, true);
                }

                coarseData(grid, grid2);
                //}
                setGridFlux(grid);
                //cout << "derrefinando" << endl;
                derefineGrid(grid, 1e-3, false);
                //cout << "derrefinado" << endl;

                //setGridFlux(grid);
                //glFlush();
                //system("pause");

                glClear(GL_COLOR_BUFFER_BIT);
                grid->calculateDeltaT();
                grid->calculateVolumeFluxes(0.95*grid->currentDeltaT);
                setGridFlux(grid);
                grid->updateConcentrations(0.5*grid->currentDeltaT);
                //cout << "total mass = " << grid->initialMass << endl;
                grid->drawPeanoCurve(1);
                //grid->drawPressure();
                //Sleep(100);
                iteration++;
                glFlush();
                //cout << grid->initialMass << endl;
            }
            /* Editado por Yuri */
            /////////////////////////////////////

            totalTime = glutGet(GLUT_ELAPSED_TIME) / hertz - totalTime;
            cout << "Total time = " << totalTime << endl;

            // totalTime.Stop();
            // elapsedTime += totalTime.Elapsedms()/1000;
            /////////////////////////////////////
        }
    }
    //grid->print();

    cout << "End of Program." << "\n\n";
    //glFlush();                      //send all output to display
}


/*______________________________________________________________________________

                          Control through keyboard
______________________________________________________________________________*/
void myKeyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    //______________________________________________________________________
    //Amplification
    case 'a'://enlarge
    case 'A':
        if(amplificationFactor < 1)
            amplificationFactor = 1.0;
        xTranslationFactor = 0.0;
        yTranslationFactor = 0.0;
        amplificationFactor *= 1.05;
        break;
    case 'l'://shrink
    case 'L':
        if(amplificationFactor > 1)
            amplificationFactor = 1.0;
        xTranslationFactor = 0.0;
        yTranslationFactor = 0.0;
        amplificationFactor *= 0.95;
        break;
    //______________________________________________________________________
    //Translation
    case 'q'://translates graph x-direction negative
    case 'Q':
        amplificationFactor = 1.0;
        xTranslationFactor = -0.01;
        yTranslationFactor = 0.0;
        break;
    case 'w'://translates graph x-direction positive
    case 'W':
        amplificationFactor = 1.0;
        xTranslationFactor = 0.01;
        yTranslationFactor = 0.0;
        break;
    case 'o'://translates graph y-direction negative
    case 'O':
        amplificationFactor = 1.0;
        xTranslationFactor = 0.0;
        yTranslationFactor = -0.01;
        break;
    case 'p'://translates graph y-direction positive
    case 'P':
        /* Editado por Yuri */
        // system("pause");
        break;
    //______________________________________________________________________
    //Full screen
    case '1':

        break;
    case '2':

        break;
    //______________________________________________________________________
    //Increases iteration number.
    case 'b':
    case 'B':

        iteration++;

        break;
    //______________________________________________________________________
    //Resets the simulation
    case 'r':
    case 'R':

        iteration = 0;
        levelNotSet = true;
        programHasAlreadyRun = false;

        break;
    //______________________________________________________________________
    default:
        amplificationFactor = 1.0;
        xTranslationFactor = 0.0;
        yTranslationFactor = 0.0;
        break;
    }
    glutPostRedisplay();
}


/*______________________________________________________________________________

                          Control through mouse
______________________________________________________________________________*/
//Through mouse motion.
void myMovingMouse(int x, int y)
{
    if(levelNotSet)
    {
        if( mouseButton == GLUT_LEFT_BUTTON )
        {
            if(fabs(600-y-quadCenterY) <= 20 && fabs(x-quadCenterX) <= 20)
                ((x<100)? (currentX = 1) : ((x>500)?(currentX = 10):(currentX = floor(x/50))));
            ((x<100)? (quadCenterX = 100) : ((x>500)?(quadCenterX = 500):(quadCenterX = x)));
        }

        glutPostRedisplay();
    }
}
//Through mouse button click.
void myMouse(int button, int state, int x, int y)
{
    mouseButton = button;

    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        break;

    case GLUT_MIDDLE_BUTTON:
        break;

    case GLUT_RIGHT_BUTTON:
        break;
    }

    if(levelNotSet)
        glutPostRedisplay();

    if(fabs(x - 300) <=20 && fabs(y - 300) <= 20)
        levelNotSet = false;
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);                                              //initialize the toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);                        //set display mode
    glutInitWindowSize(600, 600);                                       //set window size
    glutInitWindowPosition (100, 0);                                    //set window position on screen
    glutCreateWindow("Malha Adaptativa - Escoamento em meios porosos"); //open the screen window

    myInit();
    glutKeyboardFunc(myKeyboard);
    glutMouseFunc(myMouse);
    glutMotionFunc(myMovingMouse);

    glutDisplayFunc(myDisplay);

    glutMainLoop();

    return(0);
}
