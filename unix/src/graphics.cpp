//graphics.cpp

// #include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>

//#include <stdlib.h>
//#include <stdio.h>
#include <string.h>
// #include <gl/glu.h>

//Desenha uma reta dados os pontos inicial e final em coordenadas double.
void drawLine ( double x0, double y0, double x1, double y1 )
{
    glBegin(GL_LINES);
        glVertex2d(600*x0,600*y0);
        glVertex2d(600*x1,600*y1);
    glEnd();
}

void drawLineScale ( double x0, double y0, double x1, double y1 )
{
    glBegin(GL_LINES);
        glVertex2d(x0,y0);
        glVertex2d(x1,y1);
    glEnd();
}

void drawQuad(double x1,double y1, double x2, double y2)
{
	glBegin(GL_QUADS); // Start drawing a quad primitive
		glVertex2d(x1, y1); // The bottom left corner
		glVertex2d(x1, y2); // The top left corner
		glVertex2d(x2, y2); // The top right corner
		glVertex2d(x2,y1); // The bottom right corner
	glEnd();
}

//Draws a point.
void drawPoint( double x0, double y0 )
{
    glPointSize(4.0);
    glBegin(GL_POINTS);
        glVertex2d(x0,y0);
    glEnd();
}

//Define o array tableOfColors que é uma tabela das cores principais no sistema
//RGB a serem usadas pela função setColor.
static float tableOfColors[8][3] = { {0.0, 0.0, 0.0},  // 0 = preto
                              {1.0, 0.0, 0.0},  // 1 = vermelho
                              {1.0, 1.0, 0.0},  // 2 = amarelo
                              {0.0, 1.0, 0.0},  // 3 = verde
                              {0.0, 1.0, 1.0},  // 4 = azul claro
                              {0.0, 0.0, 1.0},  // 5 = azul
                              {1.0, 0.0, 1.0},  // 6 = violeta
                              {1.0, 1.0, 1.0}}; // 7 = branco


//Draws a string on screen, given its font and initial position.
void drawString(const char* string, void* font, double x0, double y0)
{
    glRasterPos2f(x0,y0); //Sets initial position.
    //Prints each character of string.
    char* charactere = (char*) string;
    while( *charactere != '\0')
        glutBitmapCharacter(font, *charactere++);

}

void drawCenterString(const char* string, void* font, double y0)
{

	int x0 = 300-((strlen(string) * 9) / 2);
	glRasterPos2f(x0,y0); //Sets initial position.
    //Prints each character of string.
    char* charactere = (char*) string;
    while( *charactere != '\0')
        glutBitmapCharacter(font, *charactere++);

}

//Define a cor que será usada para desenhar.
void setColor (int color)
{
    glColor3f(tableOfColors[color][0], tableOfColors[color][1], tableOfColors[color][2]);
}

void setBlueColor(double intensity)
{
	double aux1, aux2, aux3;

	if(intensity < 0.5)
	{
		aux1 = 0;
		aux2 = 2*intensity;
		aux3 = 1-2*intensity;
	}
	else
	{
		aux1 = 2*intensity-1;
		aux2 = 2-2*intensity;
		aux3 = 0;
	}
	glColor3f(aux1, aux2, aux3);
}
