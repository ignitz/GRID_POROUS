/* Grid.cpp
Implementation of class Grid. */

#include "Grid.h"
#include "functions.h"

// Editado por Yuri
// #include "PerfTimer.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>

#include <fstream> //For File Input/Output

#include <iostream>
    using std::cout;
    using std::cin;
    using std::endl;
    using std::ios;
    using namespace std;


/* Editado por Yuri */
///////////////////////////////
#include <GL/glut.h>

#define hertz 500

#ifdef _WIN64
        // Windows (64-bit)
        #define MATLAB_READ system("ResolveMatrixExternal.exe"); readMatlab();
        // Windows (32-bit)
        // Ainda nao foi testado
#endif
#ifdef __GNUC__
        // gnu linux
        // #define MATLAB_READ system("bicstab.exe"); readMatlab();
        #define MATLAB_READ system("./run_ResolveMatrixExternal.sh /usr/local/MATLAB/R2012b/"); readMatlab();
        /*
        // doesn't work because conflict names in MATLAB
        #define MATLAB_READ system("./run_Bicstab.sh /usr/local/MATLAB/R2012b/"); readMatlab();
        */
#elif __unix // sobram os unix
        // Unix
#endif

///////////////////////////////

#include <new>

#include <fstream>
    using std::ofstream;

//Constructor
Grid::Grid():
    firstGridCell(0),
    sideLength(1.0),
    numberOfCells(1),
    currentDeltaT(1),
    elapsedDeltaT(0.0),
    vMax(1.0),
    phi(0.27),
    initialMass(0.0),
    leftBoundaryType('p'),
    rightBoundaryType('p'),
    upperBoundaryType('v'),
    lowerBoundaryType('v')
{}

//Destructor
Grid::~Grid()
{}

/*______________________________________________________________________________
________________________________________________________________________________

                           GRID INITIALIZATION

________________________________________________________________________________
______________________________________________________________________________*/
//Initializes grid with four cells.
void Grid::initializeGrid( bool drawGrid )
{

    //Subcells.
    Cell *northeastCell,
    	 *northwestCell,
  		 *southwestCell,
     	 *southeastCell;

    northeastCell = new Cell;
    northwestCell = new Cell;
    southeastCell = new Cell;
    southwestCell = new Cell;

    //Ground transition nodes.
    Cell *northConnector,
   		 *southConnector,
      	 *eastConnector,
         *westConnector;

    northConnector = new Cell;
    southConnector = new Cell;
    eastConnector  = new Cell;
    westConnector  = new Cell;

    //Grid geometry.
    double halfSideLength    = sideLength/2.0,
  		   quarterSideLength = sideLength/4.0;


    //__________________________________________________________________________
    //					Initialization of ground transition nodes.
    //__________________________________________________________________________

  	eastConnector->type = 'w';
   	eastConnector->level = 1;
   	eastConnector->active = true;
    eastConnector->singleConnector = 0;
    eastConnector->doubleConnector1 = northeastCell;
    eastConnector->doubleConnector2 = southeastCell;

  	northConnector->type = 'w';
 	northConnector->level = 1;
 	northConnector->active = true;
    northConnector->singleConnector = 0;
   	northConnector->doubleConnector1 = northeastCell;
    northConnector->doubleConnector2 = northwestCell;

   	westConnector->type = 'w';
   	westConnector->level = 1;
   	westConnector->active = true;
    westConnector->singleConnector = 0;
    westConnector->doubleConnector1 = northwestCell;
    westConnector->doubleConnector2 = southwestCell;

   	southConnector->type = 'w';
  	southConnector->level = 1;
  	southConnector->active = true;
    southConnector->singleConnector = 0;
    southConnector->doubleConnector1 = southeastCell;
    southConnector->doubleConnector2 = southwestCell;

    //__________________________________________________________________________
    //					  Initialization of cell nodes.
    //__________________________________________________________________________

    //Northeast subcell initialization.
 	northeastCell->type = 'b';
  	northeastCell->active = true;
  	northeastCell->level = 1;
  	northeastCell->bunchNumber = 1;
  	northeastCell->faceLength = halfSideLength;
  	northeastCell->halfFaceLength = quarterSideLength;

  	northeastCell->east = eastConnector;
  	northeastCell->north = northConnector;
  	northeastCell->west = northwestCell;
   	northeastCell->south = southeastCell;
   	northeastCell->previous = 0;
   	northeastCell->next = northwestCell;

   	northeastCell->peanoNumber = 0;
    northeastCell->peanoCoordinate = 0;

   	northeastCell->centerX = halfSideLength + quarterSideLength;
   	northeastCell->centerY = halfSideLength + quarterSideLength;

   	northeastCell->rightBoundary = true;
   	northeastCell->leftBoundary = false;
   	northeastCell->lowerBoundary = false;
   	northeastCell->upperBoundary = true;

    //Northwest subcell initialization.
  	northwestCell->type = 'b';
   	northwestCell->active = true;
   	northwestCell->level = 1;
   	northwestCell->bunchNumber = 2;
  	northwestCell->faceLength = halfSideLength;
  	northwestCell->halfFaceLength = quarterSideLength;

  	northwestCell->east = northeastCell;
  	northwestCell->north = northConnector;
  	northwestCell->west = westConnector;
   	northwestCell->south = southwestCell;
   	northwestCell->previous = northeastCell;
   	northwestCell->next = southwestCell;

	northwestCell->peanoNumber = 1;
	northwestCell->peanoCoordinate = 1;

   	northwestCell->centerX = halfSideLength - quarterSideLength;
   	northwestCell->centerY = halfSideLength + quarterSideLength;

   	northwestCell->rightBoundary = false;
   	northwestCell->leftBoundary = true;
   	northwestCell->lowerBoundary = false;
   	northwestCell->upperBoundary = true;

    //Southwest subcell initialization.
  	southwestCell->type = 'b';
   	southwestCell->active = true;
   	southwestCell->level = 1;
   	southwestCell->bunchNumber = 3;
  	southwestCell->faceLength = halfSideLength;
  	southwestCell->halfFaceLength = quarterSideLength;

  	southwestCell->east = southeastCell;
  	southwestCell->north = northwestCell;
  	southwestCell->west = westConnector;
   	southwestCell->south = southConnector;
   	southwestCell->previous = northwestCell;
   	southwestCell->next = southeastCell;

   	southwestCell->peanoNumber = 2;
	southwestCell->peanoCoordinate = 2;

   	southwestCell->centerX = halfSideLength - quarterSideLength;
   	southwestCell->centerY = halfSideLength - quarterSideLength;

   	southwestCell->rightBoundary = false;
   	southwestCell->leftBoundary = true;
   	southwestCell->lowerBoundary = true;
   	southwestCell->upperBoundary = false;

    //Southeast subcell initialization.
  	southeastCell->type = 'b';
   	southeastCell->active = true;
   	southeastCell->level = 1;
   	southeastCell->bunchNumber = 4;
  	southeastCell->faceLength = halfSideLength;
  	southeastCell->halfFaceLength = quarterSideLength;

  	southeastCell->east = eastConnector;
  	southeastCell->north = northeastCell;
  	southeastCell->west = southwestCell;
   	southeastCell->south = southConnector;
   	southeastCell->previous = southwestCell;
   	southeastCell->next = 0;

   	southeastCell->peanoNumber = 3;
	southeastCell->peanoCoordinate = 3;

   	southeastCell->centerX = halfSideLength + quarterSideLength;
   	southeastCell->centerY = halfSideLength - quarterSideLength;

   	southeastCell->rightBoundary = true;
   	southeastCell->leftBoundary = false;
   	southeastCell->lowerBoundary = true;
   	southeastCell->upperBoundary = false;

    //Grid initialization
    firstGridCell = northeastCell;
    numberOfCells = 4;



    //__________________________________________________________________________
    //Grid drawing.
    if( drawGrid )
    {
        setColor(5);		        //Sets drawing color to blue

        drawLine(0.0,1.0,0.0,0.0);	//Draws square's west border.
        drawLine(1.0,1.0,0.0,1.0);	//Draws square's north border.
        drawLine(0.0,0.0,1.0,0.0);	//Draws square's south border.
        drawLine(1.0,0.0,1.0,1.0);	//Draws square's east border.
        drawLine(0.0,0.5,1.0,0.5);	//Draws horizontal line inside square.
        drawLine(0.5,0.0,0.5,1.0);	//Draws vertical line inside square.

        drawPeanoCurve(1);			//Draws peano curve in red color.
    }

}//end initializeGrid() function.

/*______________________________________________________________________________

                           PEANO'S CURVE DRAWING

    Draws peano curve of whole grid.
______________________________________________________________________________*/
void Grid::drawPeanoCurve( int colorValue )
{
    Cell *gridCell;
    gridCell = firstGridCell;
    setColor( colorValue );
    while( gridCell->next != 0 )
    {
        drawLine( gridCell->centerX, gridCell->centerY,
                  gridCell->next->centerX, gridCell->next->centerY );
        gridCell = gridCell->next;
    }
}//end drawPeanoCurve() function.

/*______________________________________________________________________________

                   ORDERING OF GRID CELLS THROUGH PEANO'S CURVE

    Enumerates cells of grid according to the ordering given by Peano's curve.
    This function is called by refineCell() after the cell is refined.
______________________________________________________________________________*/
void Grid::orderGridCells()
{
    Cell *gridCell;
    gridCell = firstGridCell;

    int counter = 0;
    while( gridCell != 0 )
   	{
        gridCell->peanoNumber = counter;
        gridCell = gridCell->next;
        counter++;
    }

}//end orderGridCells() function.
void Grid::createAuxiliarCells()
{
	auxiliarGrid = new Grid;
	Cell *gridCell = new Cell;
	Cell *northCell = new Cell;
	Cell *southCell = new Cell;
	Cell *eastCell = new Cell;
	Cell *westCell = new Cell;

	auxiliarGrid->initializeGrid(false);

	auxiliarCell = gridCell;

	auxiliarCell->north = northCell;
	auxiliarCell->south = southCell;
	auxiliarCell->east = eastCell;
	auxiliarCell->west = westCell;

	northCell->south = auxiliarCell;
	southCell->north = auxiliarCell;
	westCell->east = auxiliarCell;
	eastCell->west = auxiliarCell;


}

/*______________________________________________________________________________
________________________________________________________________________________

                               PRINT FUNCTIONS

________________________________________________________________________________
______________________________________________________________________________*/
//Prints grid cells information.
int Grid::print()
{
    ofstream outputFile( "output.txt", ios::app ); //trunc

    Cell *gridCell;
    gridCell = firstGridCell;
    int numberOfCellsGreaterThan1 = 0,
        numberOfCellsGreaterThan10Minus1 = 0,
        numberOfCellsGreaterThan10Minus2 = 0,
        numberOfCellsGreaterThan10Minus3 = 0,
        numberOfCellsGreaterThan10Minus4 = 0,
        numberOfCellsGreaterThan10Minus5 = 0,
        numberOfCellsGreaterThan10Minus6 = 0,
        numberOfCellsGreaterThan10Minus7 = 0,
        numberOfCellsGreaterThan10Minus8 = 0,
        numberOfCellsGreaterThan10Minus9 = 0;
    double exactValue, relativeError;
    while( gridCell != 0 )
   	{
        exactValue = boundaryCondition( gridCell->centerX, gridCell->centerY );
        relativeError =  fabs( ( fabs(exactValue) - fabs(gridCell->u) ) );
        if( relativeError > 1.0 )
                ++numberOfCellsGreaterThan1;
        if( relativeError > 0.1 )
                ++numberOfCellsGreaterThan10Minus1;
        if( relativeError > 0.01 )
                ++numberOfCellsGreaterThan10Minus2;
        if( relativeError > 0.001 )
                ++numberOfCellsGreaterThan10Minus3;
        if( relativeError > 0.0001 )
                ++numberOfCellsGreaterThan10Minus4;
        if( relativeError > 0.00001 )
                ++numberOfCellsGreaterThan10Minus5;
        if( relativeError > 0.000001 )
                ++numberOfCellsGreaterThan10Minus6;
        if( relativeError > 0.0000001 )
                ++numberOfCellsGreaterThan10Minus7;
        if( relativeError > 0.00000001 )
                ++numberOfCellsGreaterThan10Minus8;
        if( relativeError > 0.000000001 )
                ++numberOfCellsGreaterThan10Minus9;

        /*
        outputFile << "Cell " << gridCell->peanoNumber << "\n\n";
        outputFile << "Valor Calculado = " << gridCell->u << "\n";
        outputFile << "Valor Exato = " << exactValue << "\n";
        //outputFile << "Erro Relativo = " << relativeError << "\n";
        //outputFile << "Total Flux = " << gridCell->totalFlux << "\n";
        outputFile <<
        "________________________________________________________________________\n"
        << endl;
        */


        gridCell = gridCell->next;
    }
    outputFile << "Number of cells = " << numberOfCells << endl;
    outputFile << "Number of cells with error greater than 1 = "
                << numberOfCellsGreaterThan1 << endl;
    outputFile << "Number of cells with error greater than 0.1 = "
                << numberOfCellsGreaterThan10Minus1 << endl;
    outputFile << "Number of cells with error greater than 0.01 = "
                << numberOfCellsGreaterThan10Minus2 << endl;
    outputFile << "Number of cells with error greater than 0.001 = "
                << numberOfCellsGreaterThan10Minus3 << endl;
    outputFile << "Number of cells with error greater than 0.0001 = "
                << numberOfCellsGreaterThan10Minus4 << endl;
    outputFile << "Number of cells with error greater than 0.00001 = "
                << numberOfCellsGreaterThan10Minus5 << endl;
    outputFile << "Number of cells with error greater than 0.000001 = "
                << numberOfCellsGreaterThan10Minus6 << endl;
    outputFile << "Number of cells with error greater than 0.0000001 = "
                << numberOfCellsGreaterThan10Minus7 << endl;
    outputFile << "Number of cells with error greater than 0.00000001 = "
                << numberOfCellsGreaterThan10Minus8 << endl;
    outputFile << "Number of cells with error greater than 0.000000001 = "
                << numberOfCellsGreaterThan10Minus9 << endl;
    outputFile <<
            "________________________________________________________________________\n";
    return 0;

}//end print() function.

//Prints grid discretization matrix.
int Grid::printGridMatrix()
{
	int n = numberOfCells;

	int aux = 0;

	n = (int) sqrt(n);
	n = (int) 2*n*n-2*n;

	std::vector<double> Matrix (n*n,0);


	ofstream outputFile( "outputMatrix.txt", ios::trunc ); //app

    Cell *gridCell;
    gridCell = firstGridCell;
    Element *element;

    int counter = -1;

    while( gridCell != 0 )
   	{
   	    if( !gridCell->rightBoundary)
		{
			element = gridCell->firstElement;

            while ( element != 0 )
            {
                aux = (2*gridCell->peanoNumber-counter-1)*n + (element->column-1);
				Matrix[aux] = element->value;
                element = element->next;
            }
        }
        else
        counter++;

		if( !gridCell->lowerBoundary)
   	    {
            element = gridCell->secondElement;

            while ( element != 0 )
            {
                aux = (2*gridCell->peanoNumber -counter)*n + (element->column-1);
                Matrix[aux] = element->value;
				element = element->next;
            }
		}
		else
		counter++;

		gridCell = gridCell->next;
    }
    //outputFile << "________________________________________________________________________\n";

    outputFile << "M = " << endl;

	int aux2;
    aux2 = (int)0;

    for (int i = 0; i < n; i++  )
    {
		for (int j = 0; j < n ; j++  )
		{
			outputFile.precision(4);
    		outputFile.width(7);

			aux2 = j + i*n;
			if(Matrix[aux2] != 0 )
			outputFile << Matrix[aux2] << " (" << i+1 << "," << j+1 << ")";
		}
		outputFile << endl;
	}
    return 0;

}//end printGridMatrix() function.


//Prints full grid discretization matrix.
int Grid::printFullGridMatrix()
{
	//int n = 5*numberOfCells;
	//int aux = 0;

	Cell *gridCell;
    Element *element;

	//std::vector<double> Matrix (n*n,0);
	//std::vector<double> Matrix (10,0);

	ofstream outputFile( "outputFullMatrix.txt", ios::trunc ); //app



	gridCell = firstGridCell;
	while( gridCell != 0 )
   	{

		element = gridCell->firstElement;
        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber)*n + (element->column);
            //cout << element->column << endl;
            outputFile << 5*gridCell->peanoNumber + 1 << " " << element->column+1 << " " << element->value << endl;
			//outputFile << 5*gridCell->peanoNumber + 1 << "\n" << element->column+1 << "\n" << element->value << endl;
		    element = element->next;
        }

        element = gridCell->secondElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+1)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 5*gridCell->peanoNumber + 2 << " " << element->column+1 << " " << element->value << endl;
            //outputFile << 5*gridCell->peanoNumber + 2 << "\n" << element->column+1 << "\n" << element->value << endl;
			element = element->next;
        }

		element = gridCell->thirdElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+2)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 5*gridCell->peanoNumber + 3 << " " << element->column+1 << " " << element->value << endl;
			//outputFile << 5*gridCell->peanoNumber + 3 << "\n" << element->column+1 << "\n" << element->value << endl;
			element = element->next;
        }

		element = gridCell->fourthElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+3)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 5*gridCell->peanoNumber + 4 << " " << element->column+1 << " " << element->value << endl;
			//outputFile << 5*gridCell->peanoNumber + 4 << "\n" << element->column+1 << "\n" << element->value << endl;
			element = element->next;
        }

        element = gridCell->fifthElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+4)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 5*gridCell->peanoNumber + 5 << " " << element->column+1 << " " << element->value << endl;
			//outputFile << 5*gridCell->peanoNumber + 5 << "\n" << element->column+1 << "\n" << element->value << endl;
			element = element->next;
        }


		gridCell = gridCell->next;
    }

    /*
    aux = (int)0;

    for (int i = 0; i < n; i++  )
    {
		for (int j = 0; j < n ; j++  )
		{
			//outputFile.precision(4);
    		//outputFile.width(7);

			aux = j + i*n;
			if(Matrix[aux] != 0 )

			outputFile << i+1 << " " << j+1 << " " << Matrix[aux] << endl;
			//outputFile << Matrix[aux] ;//<< " (" << i+1 << "," << j+1 << ")";
		}
		//outputFile << endl << endl;
	}
	*/

	return 0;
}

void Grid::printB()
{
	Cell *gridCell;
    Element *element;


	ofstream outputFile( "outputB.txt", ios::trunc ); //app

	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		outputFile << gridCell-> b << endl;
		outputFile << gridCell-> b_ << endl;
		outputFile << gridCell-> bThird << endl;
		outputFile << gridCell-> bFourth << endl;
		outputFile << gridCell-> bFifth << endl;

		if((gridCell->leftBoundary && leftBoundaryType == 'n') || (gridCell->rightBoundary && rightBoundaryType == 'n'))
			outputFile << gridCell-> bSixth << endl;

		if((gridCell->upperBoundary && upperBoundaryType == 'n') || (gridCell->lowerBoundary && lowerBoundaryType == 'n'))
			outputFile << gridCell-> bSeventh << endl;

		gridCell = gridCell->next;
	}
}


//Prints full grid discretization matrix.
int Grid::printFullGridMatrixRefining()
{
	//int n = 5*numberOfCells;
	//int aux = 0;

	Cell *gridCell;
    Element *element;

    int counter = 0;
	//std::vector<double> Matrix (n*n,0);
	//std::vector<double> Matrix (10,0);
	ofstream outputFile( "outputFullMatrixRefining.txt", ios::trunc ); //app


	gridCell = firstGridCell;
	while( gridCell != 0 )
   	{

		element = gridCell->firstElement;
        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber)*n + (element->column);
            //cout << element->column << endl;
            outputFile << 7*gridCell->peanoNumber + 1 - counter<< " " << element->column+1 << " " << element->value << endl;
		    element = element->next;
        }

        element = gridCell->secondElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+1)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 7*gridCell->peanoNumber + 2 - counter<< " " << element->column+1 << " " << element->value << endl;
			element = element->next;
        }

		element = gridCell->thirdElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+2)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 7*gridCell->peanoNumber + 3 - counter<< " " << element->column+1 << " " << element->value << endl;
			element = element->next;
        }

		element = gridCell->fourthElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+3)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 7*gridCell->peanoNumber + 4 - counter<< " " << element->column+1 << " " << element->value << endl;
			element = element->next;
        }

        element = gridCell->fifthElement;

        while ( element != 0 )
        {
            //aux = (5*gridCell->peanoNumber+4)*n + (element->column);
            //Matrix[aux] = element->value;
            outputFile << 7*gridCell->peanoNumber + 5 - counter<< " " << element->column+1 << " " << element->value << endl;
			element = element->next;
        }

       	if((gridCell->leftBoundary && leftBoundaryType == 'n') || (gridCell->rightBoundary && rightBoundaryType == 'n'))
	    {
			element = gridCell->sixthElement;

	        while ( element != 0 )
	        {
	            //aux = (5*gridCell->peanoNumber+4)*n + (element->column);
	            //Matrix[aux] = element->value;
	            outputFile << 7*gridCell->peanoNumber + 6 - counter<< " " << element->column+1 << " " << element->value << endl;
				element = element->next;
	        }
		}
		else
			counter++;

        if((gridCell->upperBoundary && upperBoundaryType == 'n') || (gridCell->lowerBoundary && lowerBoundaryType == 'n'))
	    {
	        element = gridCell->seventhElement;

	        while ( element != 0 )
	        {
	            //aux = (5*gridCell->peanoNumber+4)*n + (element->column);
	            //Matrix[aux] = element->value;
	            outputFile << 7*gridCell->peanoNumber + 7 - counter<< " " << element->column+1 << " " << element->value << endl;
				element = element->next;
	        }
		}
		else
			counter++;

		gridCell = gridCell->next;
    }

    /*
    aux = (int)0;

    for (int i = 0; i < n; i++  )
    {
		for (int j = 0; j < n ; j++  )
		{
			//outputFile.precision(4);
    		//outputFile.width(7);

			aux = j + i*n;
			if(Matrix[aux] != 0 )

			outputFile << i+1 << " " << j+1 << " " << Matrix[aux] << endl;
			//outputFile << Matrix[aux] ;//<< " (" << i+1 << "," << j+1 << ")";
		}
		//outputFile << endl << endl;
	}
	*/

	return 0;
}





/*______________________________________________________________________________
________________________________________________________________________________

                           SOLUTION OF DARCY'S EQUATION

________________________________________________________________________________
______________________________________________________________________________*/

/*______________________________________________________________________________

                        FUNCTION setM()

   Sets the arrays M and m in order to solve Darcy's equation
______________________________________________________________________________*/
void Grid::setM()
{
    Cell *gridCell;
    gridCell = firstGridCell;

    double aux1, aux2;



    while( gridCell != 0 )
    {

		//4 Quadrantes
		/*
		if(gridCell->centerX <= 0.5 && gridCell->centerY <= 0.5)
			gridCell->S = 1;
		else if(gridCell->centerX <= 0.5 && gridCell->centerY > 0.5)
			gridCell->S = 2;
		else if(gridCell->centerX > 0.5 && gridCell->centerY <= 0.5)
			gridCell->S = 3;
		else if(gridCell->centerX > 0.5 && gridCell->centerY > 0.5)
			gridCell->S = 4;
		*/

		//if(gridCell->centerY >= 0.5)
	//		gridCell->S = gridCell->centerY;
		//else
	//		gridCell->S = 1-gridCell->centerY;

		//Círculos
        if((gridCell->centerY-0.5)*(gridCell->centerY-0.5) + (gridCell->centerX-0.5)*(gridCell->centerX-0.5) <= 0.01)
		gridCell->S = 1e5;

		//Elipsóide
        //if(((gridCell->centerY)*(gridCell->centerY) + (gridCell->centerX-0.5)*(gridCell->centerX-0.5) <= 0.4) &&
		//	((gridCell->centerY-1)*(gridCell->centerY-1) + (gridCell->centerX-0.5)*(gridCell->centerX-0.5) <= 0.4) )
		//gridCell->S = 1e5;

		//if((gridCell->centerY-0.3)*(gridCell->centerY-0.3) + (gridCell->centerX-0.5)*(gridCell->centerX-0.5) <= 0.01)
		//gridCell->S = 1e5;

    	//Retângulos
 		/*
		if(gridCell->centerY <= 0.6 && gridCell->centerY >= 0.4 && gridCell->centerX <= 0.6 && gridCell->centerX >= 0.2)
        gridCell->S = 1e5;

        if(gridCell->centerY <= 0.6 && gridCell->centerY >= 0.4 && gridCell->centerX <= 0.6 && gridCell->centerX >= 0.2)
        gridCell->S = 1e5;

        if(gridCell->centerY <= 0.3 && gridCell->centerY >= 0.1 && gridCell->centerX <= 0.6 && gridCell->centerX >= 0.2)
        gridCell->S = 1e5;
		*/


		gridCell->K = 1/gridCell->S;

		//while(gridCell->K <= 0)
        //{gridCell->K = rand()%5;}
        //cout << " K = " << gridCell->K << endl;

        aux1 = (double) 1/(gridCell->w * gridCell->K * gridCell->K - 24 * gridCell->K);
    	aux2 = (double) 6/(gridCell->w * gridCell->K - 24);

		gridCell->M[0] = (4*gridCell->w*gridCell->K-60)*aux1;
		gridCell->M[1] = (12 - 2*gridCell->w*gridCell->K)*aux1;
        gridCell->M[2] = (4*gridCell->w*gridCell->K-60)*aux1;
        gridCell->M[3] = 36*aux1;
        gridCell->M[4] = -36*aux1;
        gridCell->M[5] = (4*gridCell->w*gridCell->K-60)*aux1;
        gridCell->M[6] = -36*aux1;
        gridCell->M[7] = 36*aux1;
        gridCell->M[8] = (12 - 2*gridCell->w*gridCell->K)*aux1;
        gridCell->M[9] = (4*gridCell->w*gridCell->K-60)*aux1;

        gridCell->m[0] = -aux2;
		gridCell->m[1] = aux2;
        gridCell->m[2] = -aux2;
        gridCell->m[3] = aux2;


        /*cout << "M = " << endl;
		for (int i=0; i<=9; i++)
			{
			cout << gridCell->M[i] << "  ";
			}
		cout << endl;

		cout << "m = " << endl;

		for (int i=0; i<=3; i++)
			{
			cout << gridCell->m[i] << "  ";
			}

        cout << endl;*/
        gridCell = gridCell->next;
    }

}


/*______________________________________________________________________________
/*______________________________________________________________________________
/*______________________________________________________________________________
/*______________________________________________________________________________

                            SOLVE REDUCED DARCY

    Solves Darcy's problem if all the boundaries are pressure-specified

//____________________________________________________________________________*/
//____________________________________________________________________________*/
//____________________________________________________________________________*/
/*______________________________________________________________________________

                        FUNCTION initializeDiagonalElements()

  Initializes the diagonal elements of the grid discretization matrix.
  Each diagonal element corresponds to the first element in the linked list which
  represents the row associated to the particular cell.

  Moreover, this function also resets to zero the vector b in Ax = b.

______________________________________________________________________________*/
void Grid::initializeDiagonalElements()
{
    Cell *gridCell;
    gridCell = firstGridCell;

    Element *element;
    Element *element2;

    int counter = -1;

    while( gridCell != 0 )
   	{

			gridCell->b = 0.0;
			gridCell->b_ = 0.0;

			if (!gridCell->rightBoundary)
			{
	            element = new Element;
				element->column = 2*gridCell->peanoNumber-counter;
	       	    element->cell = gridCell;
	       	    gridCell->firstElement = element;
			}
			else
			counter++;

			if (!gridCell->lowerBoundary)
			{
				element2 = new Element;
				element2->column = 2*gridCell->peanoNumber+1-counter;
				element2->right = false;
	       	    element2->cell = gridCell;
	       	    gridCell->secondElement = element2;
			}
			else
			counter++;

   	    gridCell = gridCell->next;
    }
}

/*______________________________________________________________________________

                      FUNCTION fillDiscretizationMatrixElements()

______________________________________________________________________________*/
void Grid::fillDiscretizationMatrixElements( )
{
    Cell *gridCell;
    Element *element;
    Element *element2;
    Element *auxiliarElement;

	gridCell = firstGridCell;

	int counter = -1;

    while ( gridCell != 0 )
    {
		 /***************Primeira linha associada a esta célula****************/
	     /*********************************************************************/



		if (!gridCell->rightBoundary)
		{
			element = new Element;
			element = gridCell->firstElement;
		 	//p i+1/2, j
			element->value = gridCell->M[2] + gridCell->east->M[0];


			while( element->next != 0 )
	        {
				element = element->next;
	        }

			//p i-1/2, j
			if (!gridCell->leftBoundary)
			{
	            auxiliarElement = new Element;
	            auxiliarElement->value = (-1) * gridCell->M[1];
	            auxiliarElement->cell = gridCell->west;
	            auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	            element->next = auxiliarElement;
			}
			else
			{
			    gridCell->b += gridCell->M[1]*westBoundaryCondition(gridCell->centerY);
			}
			while( element->next != 0 )
	        {
				element = element->next;
	        }


			//p i+3/2, j
		    if (!gridCell->east->rightBoundary)
			{
				auxiliarElement = new Element;
	            auxiliarElement->value = (-1) * gridCell->east->M[1];
	            auxiliarElement->cell = gridCell->east;
	            auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	            element->next = auxiliarElement;
			}
			else
			{
			    gridCell->b += gridCell->east->M[1]*eastBoundaryCondition(gridCell->centerY);
			}
			while( element->next != 0 )
	        {
				element = element->next;
	        }


			//p i, j-1/2 e p i+1, j-1/2
			if (!gridCell->upperBoundary)
			{
				auxiliarElement = new Element;
	            auxiliarElement->value = (-1) * gridCell->M[4];
	            auxiliarElement->right = false;
	            auxiliarElement->cell = gridCell->north;
	            auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	            element->next = auxiliarElement;

	            element = element->next;

	            auxiliarElement = new Element;
	            auxiliarElement->value = gridCell->east->M[3];
	            auxiliarElement->right = false;
	            auxiliarElement->cell = gridCell->east->north;
	            auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	            element->next = auxiliarElement;
			}
			else
			{
				gridCell->b += gridCell->M[4]*northBoundaryCondition(gridCell->centerX)
								- gridCell->east->M[3]*northBoundaryCondition(gridCell->east->centerX);
			}
			while( element->next != 0 )
	        {
				element = element->next;
	        }

			//p i, j+1/2 e p i+1, j+1/2
			if (!gridCell->lowerBoundary)
			{
				auxiliarElement = new Element;
	            auxiliarElement->value = gridCell->M[7];
	            auxiliarElement->right = false;
	            auxiliarElement->cell = gridCell;
	            auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	            element->next = auxiliarElement;

				element = element->next;

				auxiliarElement = new Element;
	            auxiliarElement->value = (-1)*gridCell->east->M[6];
	            auxiliarElement->right = false;
	            auxiliarElement->cell = gridCell->east;
	            auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	            element->next = auxiliarElement;
			}
			else
			{
				gridCell->b += gridCell->east->M[6]*southBoundaryCondition(gridCell->east->centerX)
								-gridCell->M[7]*southBoundaryCondition(gridCell->centerX);
			}
	     } // end if Primeira Linha
	     else
	     counter++;




	     /****************Segunda linha associada a esta célula****************/
	     /*********************************************************************/


	     if (!gridCell->lowerBoundary)
		 {
		 	 element2 = new Element;
			 element2 = gridCell->secondElement;
			 //p i, j+ 1/2
			element2->value = gridCell->M[9] + gridCell->south->M[5];

			while( element2->next != 0 )
	        {
				element2 = element2->next;
	        }

			//p i, j-1/2
			if (!gridCell->upperBoundary)
			{
	            auxiliarElement = new Element;
	            auxiliarElement->value = (-1) * gridCell->M[8];
	            auxiliarElement->right = false;
	            auxiliarElement->cell = gridCell->north;
	            auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	            element2->next = auxiliarElement;
			}
			else
			{
			    gridCell->b_ += gridCell->M[8]*northBoundaryCondition(gridCell->centerX);
			}
			while( element2->next != 0 )
	        {
				element2 = element2->next;
	        }


			//p i, j+3/2
		    if (!gridCell->south->lowerBoundary)
			{
				auxiliarElement = new Element;
	            auxiliarElement->value = (-1) * gridCell->south->M[8];
	            auxiliarElement->right = false;
	            auxiliarElement->cell = gridCell->south;
	            auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	            element2->next = auxiliarElement;
			}
			else
			{
			    gridCell->b_ += gridCell->south->M[8]*southBoundaryCondition(gridCell->centerX);
			}
			while( element2->next != 0 )
	        {
				element2 = element2->next;
	        }


			//p i- 1/2, j e p i- 1/2, j+1
			if (!gridCell->leftBoundary)
			{
				auxiliarElement = new Element;
	            auxiliarElement->value = (-1) * gridCell->M[6];
	            auxiliarElement->cell = gridCell->west;
	            auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	            element2->next = auxiliarElement;

	            element2 = element2->next;

	            auxiliarElement = new Element;
	            auxiliarElement->value = gridCell->south->M[3];
	            auxiliarElement->cell = gridCell->west->south;
	            auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	            element2->next = auxiliarElement;
			}
			else
			{
				gridCell->b_ += gridCell->M[6]*westBoundaryCondition(gridCell->centerY)
								-gridCell->south->M[3]*westBoundaryCondition(gridCell->south->centerY);
			}
			while( element2->next != 0 )
	        {
				element2 = element2->next;
	        }



			//p i+ 1/2, j e p i+1/2, j+1
			if (!gridCell->rightBoundary)
			{
				auxiliarElement = new Element;
	            auxiliarElement->value = gridCell->M[7];
	            auxiliarElement->cell = gridCell;
	            auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	            element2->next = auxiliarElement;

				element2 = element2->next;

				auxiliarElement = new Element;
	            auxiliarElement->value = (-1)*gridCell->south->M[4];
	            auxiliarElement->cell = gridCell->south;
	            auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	            element2->next = auxiliarElement;
			}
			else
			{
				gridCell->b_ += gridCell->south->M[4]*eastBoundaryCondition(gridCell->centerY)
							    -gridCell->M[7]*eastBoundaryCondition(gridCell->south->centerY);
			}
	     }  //end if Segunda linha
	     else
	     counter++;

	     gridCell = gridCell->next;


	  }


}
/*______________________________________________________________________________
________________________________________________________________________________

                           FUNCTION refineGridInitially()

    Refines grid before starting computations.

________________________________________________________________________________
______________________________________________________________________________*/

void Grid::refineGridInitially( int finalLevel, bool drawGrid )
{
 	 Cell *gridCell;
     for( int i = 0; i < finalLevel; i++ )
     {
		 gridCell = firstGridCell;
         while( gridCell != 0 )
       	 {
       	     if( gridCell->level < finalLevel )
       	     {
				 refineCell(gridCell,drawGrid);
                 numberOfCells += 3;
             }
             gridCell = gridCell->next;
         }
         //cout << "refined" << endl;
     }
     orderGridCells();
}//end refineGridInitially() function


/*______________________________________________________________________________

                        FUNCTION refineGrid()

    Decides if the grid should be refined by traversing the whole grid.
    If refining is no longer necessary, according to the parameters
    refinementLevel and refinementBound, returns false.
    A cell will not be refined either if its level of refinement is equal to
    refinementLevel or the totalFlux into it is less than refinementBound.

______________________________________________________________________________*/
bool Grid::refineGrid( int refinementLevel, double refinementBound, bool drawGrid )
{
    Cell *gridCell,
    	 *auxiliarGridCell,
    	 *neighbourCell;
    bool continueRefining = false;
    double maximumFlux = 0;


    gridCell = firstGridCell;
    while( gridCell != 0 )
    {
		if( ( gridCell->level < refinementLevel ) &&
            (gridCell->totalFlux >= refinementBound ) )
        {
			auxiliarGridCell = gridCell->next;
            refineCell( gridCell, drawGrid);
            numberOfCells += 3;
            gridCell = auxiliarGridCell;
            continueRefining = true;
        }
        else
            gridCell = gridCell->next;
    }
    orderGridCells();
    return continueRefining;
}

/*______________________________________________________________________________

                        FUNCTION conjugateGradient()

    Conjugate gradient method for symmetric positive definite matrix.
    The grid discretization matrix and all variables used in the conjugate
    gradient method are stored on the grid's cells, which improves performance.
    There is no independent linear algebra package.

______________________________________________________________________________*/
void Grid::conjugateGradient()
{
    double error = 0.0,	     //Norm of resídue b - Ax after each iteration.
           pError = 1.0,
           rTr,
           r1Tr1,
           pTAp,
           alpha,
           beta,
           normR = 0.0,
           normPP1 = 0.0,
           precision = 1e-15; //Maximum precision which error must not exceed.

    int maximumNumberOfIterations = 4*numberOfCells,
        numberOfIterations = 0,
        column;

    Cell *gridCell;
    Element *element;

    //__________________________________________________________________________
    //Computes vector A*x, residue r = b - Ax, scalar rTr = r^T * r and
    //sets initial search direction p.   (Uses Ap to store Ax)
    gridCell = firstGridCell;
    rTr = 0.0;


    while( gridCell != 0 )
    {

        gridCell->Ap = 0.0;

        if(!gridCell->rightBoundary)
        {
			element = gridCell->firstElement;
            while( element != 0 )
            {

				if(element->right)
				{
					gridCell->Ap += element->value * element->cell->pR;
                	element = element->next;
				}
				else
				{
					gridCell->Ap += element->value * element->cell->pD;
                	element = element->next;
				}
            }
		}



        gridCell->Ap_ = 0.0;

		if(!gridCell->lowerBoundary)
        {
		    element = gridCell->secondElement;
			while( element != 0 )
            {

			    if(element->right)
				{
					gridCell->Ap_ += element->value * element->cell->pR;
					element = element->next;
				}
				else
				{
					gridCell->Ap_ += element->value * element->cell->pD;
					element = element->next;
				}
			}
		}

        gridCell->r = gridCell->b - gridCell->Ap;
        gridCell->r_ = gridCell->b_ - gridCell->Ap_;

        rTr += gridCell->r * gridCell->r;
        rTr += gridCell->r_ * gridCell->r_;

		gridCell->p = gridCell->r;
        gridCell->p_ = gridCell->r_;

        if( fabs(gridCell->r) > error )
            error = fabs(gridCell->r);

		if( fabs(gridCell->r_) > error )
            error = fabs(gridCell->r_);

        gridCell = gridCell->next;
    }
    //__________________________________________________________________________
    //Conjugate gradient iterations.
    if( error >= precision )
    {

		while( (pError >= precision)  &&
               (numberOfIterations < maximumNumberOfIterations) )
        {
            //cout << "iteration = " << numberOfIterations << endl;
			pError = 0.0;
            normPP1 = 0.0;
            //__________________________________________________________________
            // Computes Ap and pTAp. Uses Ax to store Ap.
            gridCell = firstGridCell;
            pTAp = 0.0;

            while( gridCell != 0 )
            {
                if( !gridCell->rightBoundary )
                {
                    gridCell->Ap = 0.0;
		            element = gridCell->firstElement;
		            while( element != 0 )
		            {
		                if(element->right)
						{
							gridCell->Ap += element->value * element->cell->p;
		                	element = element->next;
						}
						else
						{
							gridCell->Ap += element->value * element->cell->p_;
		                	element = element->next;
						}
		            }
                    pTAp += gridCell->p * gridCell->Ap;
				}

				if( !gridCell->lowerBoundary )
                {
					gridCell->Ap_ = 0.0;
					element = gridCell->secondElement;
                    while( element != 0 )
		            {
		                if(element->right)
						{
							gridCell->Ap_ += element->value * element->cell->p;
		                	element = element->next;
						}
						else
						{
							gridCell->Ap_ += element->value * element->cell->p_;
		                	element = element->next;
						}
		            }
                    pTAp += gridCell->p_ * gridCell->Ap_;

                }
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            // Computes alpha.
            alpha = rTr/pTAp;
            //cout << "pTAp = " << pTAp<<endl;
            //cout << "alpha = " << alpha << endl;
            //__________________________________________________________________
            // Computes new value of solution: u = u + alpha*p.
            gridCell = firstGridCell;
            while( gridCell != 0 )
            {
                if( !gridCell->rightBoundary )
                    gridCell->pR += alpha * gridCell->p;
                if( !gridCell->lowerBoundary )
                    gridCell->pD += alpha * gridCell->p_;
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            //Upgrades residue r1 = r - alpha*Ap and computes r1Tr1 = r1^T * r1.
            gridCell = firstGridCell;
            r1Tr1 = 0.0;
            while( gridCell != 0 )
            {
                if( !gridCell->rightBoundary )
                {
                    gridCell->r -= alpha * gridCell->Ap;
                    r1Tr1 += gridCell->r * gridCell->r;
				}
				if( !gridCell->lowerBoundary )
                {
                    gridCell->r_ -= alpha * gridCell->Ap_;
                    r1Tr1 += gridCell->r_ * gridCell->r_;
                }
                gridCell = gridCell->next;
            }
            //__________________________________________________________________
            //Computes beta.
            beta = r1Tr1/rTr;

            //__________________________________________________________________
            //Computes vector p1 = r1 + beta*p and uses it to upgrade p.
            gridCell = firstGridCell;
            while( gridCell != 0 )
            {
                if( !gridCell->rightBoundary )
                {
                    if ( fabs(gridCell->r) > normR )
                         normR = fabs(gridCell->r);
                    gridCell->p1 = gridCell->r + beta * gridCell->p;
                    if ( fabs(gridCell->p - gridCell->p1) > normPP1 )
                         normPP1 = fabs(gridCell->p - gridCell->p1);
                    gridCell->p = gridCell->p1;
				}
				if( !gridCell->lowerBoundary )
				{
					gridCell->p1_ = gridCell->r_ + beta * gridCell->p_;

                    if ( fabs(gridCell->r_) > normR )
                         normR = fabs(gridCell->r_);

                    if ( fabs(gridCell->p_ - gridCell->p1_) > normPP1 )
                         normPP1 = fabs(gridCell->p_ - gridCell->p1_);

                    gridCell->p_ = gridCell->p1_;
                }
                gridCell = gridCell->next;
            }

            if( normR < precision )
                pError = 0.0;
            else if( normPP1 > pError )
                pError = normPP1;
            //__________________________________________________________________
            //Upgrades rTr.
            rTr = r1Tr1;
			cout << "r1Tr1 " << rTr << endl << endl ;

            numberOfIterations++;

       }

     cout << "Numero de Celulas: " << numberOfCells
     << "\tNumero de iteracoes do gradiente conjugado: " << numberOfIterations << endl;

	 }//end of conjugate gradient iterations.
}//end conjugateGradient() function.

/*______________________________________________________________________________
                        FUNCTION freeMemory()

    Eliminates memory overload of grid discretization matrix at each refinement
    iteration.

______________________________________________________________________________*/
void Grid::freeMemory()
{
    Cell *gridCell;
    Element *element,
            *auxiliarElement;

    gridCell = firstGridCell;
    while( gridCell != 0 )
    {
        if( gridCell->active )
        {
            element = gridCell->firstElement;

            while( element != 0 )
            {
                auxiliarElement = element->next;
                delete element;
                element = auxiliarElement;
            }
            element = gridCell->secondElement;

            while( element != 0 )
            {
                auxiliarElement = element->next;
                delete element;
                element = auxiliarElement;
            }
        }
        gridCell = gridCell->next;
     }
}//end freeMemory() function.


/*______________________________________________________________________________

                            FUNCTION solveReducedDarcy()

     Solves Darcy's equation, given the pressures in ALL boundaries through the following steps:
     1) Initializes grid and sets initial shot for the solution pR and pD.
     2) Sets grid discretization matrix A and constant term b.
     3) Solves linear system Ax = b by conjugate gradient method.

     Its boolean argument will determine if the program will draw on the screen
     the grid and its associated Peano's curve (true) or not (false).

//____________________________________________________________________________*/
void Grid::solveReducedDarcy( bool drawGrid, int refinementLevel )
{
    //int refinementLevel = 2;
    double refinementBound = 1;
    int numberOfRefinements = 0;
    bool continueRefining = true;

    double conjugateGradientTotalTime = 0.0;

    Cell *gridCell;
    Cell *aux;

    //cout << "Digite o nivel de refinamento desejado: \nNivel = ";
    //cin >> refinementLevel;

    //Initialize grid and sets initial shot.
    //initializeGrid( drawGrid );
    //initialGuess();

	/*
	//Gambiarra temporária pra refinar todo o grid inicialmente.
	for (int i = 1; i <= refinementLevel-1; i++)
	{
		//cout << "level = " << i << endl;
		//system("pause");
		gridCell = firstGridCell;
	    while(gridCell != 0)
		{

			aux = gridCell->next;
			refineCell( gridCell, true);
			numberOfCells += 3;
		    gridCell = aux;

	    }

	}
	*/
	//Setting Problem
	setM();
	cout << "M set" << endl;
	initializeDiagonalElements();
	fillDiscretizationMatrixElements();
	cout << "problem set" << endl;
	//printGridMatrix();

    //Solving the system

/* Editado por Yuri */
////////////////////////////////////////////////////////////////////////////
	double conjugateGradientTime = glutGet(GLUT_ELAPSED_TIME) / hertz;
	conjugateGradient();
	freeMemory();
	conjugateGradientTotalTime = glutGet(GLUT_ELAPSED_TIME) / hertz - conjugateGradientTime;

/*
	CPerfTimer conjugateGradientTime;
    conjugateGradientTime.Start();
	conjugateGradient();
	freeMemory();
    conjugateGradientTime.Stop();
    conjugateGradientTotalTime += conjugateGradientTime.Elapsedms()/1000;
*/
////////////////////////////////////////////////////////////////////////////



    /*
    //Refinement iterations.
    while( continueRefining )
    {
        setDiscretizationMatrix();

        CPerfTimer conjugateGradientTime;
        conjugateGradientTime.Start();
        conjugateGradient();
        conjugateGradientTime.Stop();
        conjugateGradientTotalTime += conjugateGradientTime.Elapsedms()/1000;

        freeMemory();
        continueRefining = refineGrid( refinementLevel, refinementBound, drawGrid );
        numberOfRefinements++;
    }*/

    cout << "Tempo gasto no Gradiente Conjugado: " << conjugateGradientTotalTime << endl;
    calculatePressure();
    calculateVelocities();
    calculateVolumeFluxes(currentDeltaT);

}
/*______________________________________________________________________________

                            FUNCTION calculatePressure()

    Calculates the pressure inside each cell by "averaging" the pressures on the
	sides.

//____________________________________________________________________________*/
void Grid::calculatePressure()
{
	Cell *gridCell;

	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		if(!gridCell->leftBoundary)
		gridCell->pressure += gridCell->m[0]*gridCell->west->pR;
		else
		gridCell->pressure += gridCell->m[0]*westBoundaryCondition(gridCell->centerY);

		if(!gridCell->rightBoundary)
		gridCell->pressure -= gridCell->m[1]*gridCell->pR;
		else
		gridCell->pressure -= gridCell->m[1]*eastBoundaryCondition(gridCell->centerY);

		if(!gridCell->upperBoundary)
		gridCell->pressure += gridCell->m[2]*gridCell->north->pD;
		else
		gridCell->pressure += gridCell->m[2]*northBoundaryCondition(gridCell->centerX);

		if(!gridCell->lowerBoundary)
		gridCell->pressure -= gridCell->m[3]*gridCell->pD;
		else
		gridCell->pressure -= gridCell->m[3]*southBoundaryCondition(gridCell->centerX);


		//cout << " pressure = " << gridCell->pressure << "(" << (gridCell->centerX/0.125+1)/2 << "," << (gridCell->centerY/0.125+1)/2 << ")" << endl;
		gridCell = gridCell->next;
	}

}
/*______________________________________________________________________________

                            FUNCTION calculateVelocities()

    Calculates the velocities on each cell side using the pressures on the
	sides.

//____________________________________________________________________________*/

void Grid::calculateVelocities()
{
	Cell *gridCell;
	double A;

	currentDeltaT = 1.0;

	gridCell = firstGridCell;

	while(gridCell != 0)
	{

		// Calculating vL
		if(!gridCell->leftBoundary)
		gridCell->vL += gridCell->M[0]*gridCell->west->pR;
		else
		gridCell->vL += gridCell->M[0]*westBoundaryCondition(gridCell->centerY);

		if(!gridCell->rightBoundary)
		gridCell->vL -= gridCell->M[1]*gridCell->pR;
		else
		gridCell->vL -= gridCell->M[1]*eastBoundaryCondition(gridCell->centerY);

		if(!gridCell->upperBoundary)
		gridCell->vL += gridCell->M[3]*gridCell->north->pD;
		else
		gridCell->vL += gridCell->M[3]*northBoundaryCondition(gridCell->centerX);

		if(!gridCell->lowerBoundary)
		gridCell->vL -= gridCell->M[6]*gridCell->pD;
		else
		gridCell->vL -= gridCell->M[6]*southBoundaryCondition(gridCell->centerX);



		// Calculating vR

		if(!gridCell->leftBoundary)
		gridCell->vR += gridCell->M[1]*gridCell->west->pR;
		else
		gridCell->vR += gridCell->M[1]*westBoundaryCondition(gridCell->centerY);

		if(!gridCell->rightBoundary)
		gridCell->vR -= gridCell->M[2]*gridCell->pR;
		else
		gridCell->vR -= gridCell->M[2]*eastBoundaryCondition(gridCell->centerY);

		if(!gridCell->upperBoundary)
		gridCell->vR += gridCell->M[4]*gridCell->north->pD;
		else
		gridCell->vR += gridCell->M[4]*northBoundaryCondition(gridCell->centerX);

		if(!gridCell->lowerBoundary)
		gridCell->vR -= gridCell->M[7]*gridCell->pD;
		else
		gridCell->vR -= gridCell->M[7]*southBoundaryCondition(gridCell->centerX);




		// Calculating vU

		if(!gridCell->leftBoundary)
		gridCell->vU += gridCell->M[3]*gridCell->west->pR;
		else
		gridCell->vU += gridCell->M[3]*westBoundaryCondition(gridCell->centerY);

		if(!gridCell->rightBoundary)
		gridCell->vU -= gridCell->M[4]*gridCell->pR;
		else
		gridCell->vU -= gridCell->M[4]*eastBoundaryCondition(gridCell->centerY);

		if(!gridCell->upperBoundary)
		gridCell->vU += gridCell->M[5]*gridCell->north->pD;
		else
		gridCell->vU += gridCell->M[5]*northBoundaryCondition(gridCell->centerX);

		if(!gridCell->lowerBoundary)
		gridCell->vU -= gridCell->M[8]*gridCell->pD;
		else
		gridCell->vU -= gridCell->M[8]*southBoundaryCondition(gridCell->centerX);




		// Calculating vD

		if(!gridCell->leftBoundary)
		gridCell->vD += gridCell->M[6]*gridCell->west->pR;
		else
		gridCell->vD += gridCell->M[6]*westBoundaryCondition(gridCell->centerY);

		if(!gridCell->rightBoundary)
		gridCell->vD -= gridCell->M[7]*gridCell->pR;
		else
		gridCell->vD -= gridCell->M[7]*eastBoundaryCondition(gridCell->centerY);

		if(!gridCell->upperBoundary)
		gridCell->vD += gridCell->M[8]*gridCell->north->pD;
		else
		gridCell->vD += gridCell->M[8]*northBoundaryCondition(gridCell->centerX);

		if(!gridCell->lowerBoundary)
		gridCell->vD -= gridCell->M[9]*gridCell->pD;
		else
		gridCell->vD -= gridCell->M[9]*southBoundaryCondition(gridCell->centerX);



		gridCell = gridCell->next;
	}
	//cout << "Delta t maximo = " << currentDeltaT << endl;
}
/*______________________________________________________________________________

                            FUNCTION calculateDeltaT()

    Actually, calculates deltaT/faceLength. It will be 	corrected in
	calculateVolumeFluxes()

//____________________________________________________________________________*/

void Grid::calculateDeltaT()
{
	Cell *gridCell;
	double A;

	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		A = gridCell->faceLength*gridCell->faceLength*phi;

		if ( A/( fmax(gridCell->vR,0) - fmin(gridCell->vL,0) ) < currentDeltaT)
		currentDeltaT = A/( fmax(gridCell->vR,0) - fmin(gridCell->vL,0) );

		if ( A/( fmax(gridCell->vD,0) - fmin(gridCell->vU,0) ) < currentDeltaT)
		currentDeltaT = A/( fmax(gridCell->vD,0) - fmin(gridCell->vU,0) );

		gridCell = gridCell->next;
	}

	//cout << "current delta T = " << currentDeltaT << endl;
	//system("pause");

}


/*______________________________________________________________________________

                            FUNCTION calculateVolumeFluxes()

    Calculates the volume flux integrals on each edge

//____________________________________________________________________________*/

void Grid::calculateVolumeFluxes(double deltaT)
{
	Cell *gridCell;
	double realDeltaT;

	// For safety reasons, uses only 90% of the maximum velocity



	//cout << "deltaT = " << deltaT << endl;
	//system("pause");


	gridCell = firstGridCell;

	while(gridCell != 0)
	{
		realDeltaT = deltaT;
		//realDeltaT = 0.9*deltaT;  /* SEE calculateDeltaT() */

		//VL is the volume flux integral
		gridCell->VL = gridCell->vL*realDeltaT;

		//VR is the volume flux integral
		gridCell->VR = gridCell->vR*realDeltaT;

		//VU is the volume flux integral
		gridCell->VU = gridCell->vU*realDeltaT;
		//cout << "vU = " << gridCell->vU << endl;
		//cout << gridCell->VU << endl;

		//VD is the volume flux integral
		gridCell->VD = gridCell->vD*realDeltaT;
		//cout << "vD = " << gridCell->vD << endl;
		//cout <<gridCell->VD << endl;

		gridCell = gridCell->next;
	}
	//cout << endl << endl;
}

/*______________________________________________________________________________

                            SOLVE COMPLETE SYSTEM

    				   Sometimes, hard work is necessary.

//____________________________________________________________________________*/


/*______________________________________________________________________________

                    FUNCTION initializeDiagonalElementsFull()

  Initializes the diagonal elements of the grid discretization matrix.
  Each diagonal element corresponds to the first element in the linked list which
  represents the on of the rows associated to the particular cell.

  Moreover, this function also resets to zero the vector b in Ax = b.

______________________________________________________________________________*/
void Grid::initializeDiagonalElementsFull()
{
    Cell *gridCell;
    gridCell = firstGridCell;

    Element *element;


    while( gridCell != 0 )
   	{

			gridCell->b = 0.0;
			gridCell->b_ = 0.0;
			gridCell->bThird = 0.0;
   	        gridCell->bFourth = 0.0;
   	        gridCell->bFifth = 0.0;
   	        //gridCell->bSixth = 0.0;
   	        //gridCell->bSeventh = 0.0;


			if (!gridCell->leftBoundary)
			{
				//First Row: vL

				element = new Element;
				element->column = 5*gridCell->peanoNumber;
	       	    element->cell = gridCell;
	       	    element->right = false;
	       	    element->value = gridCell->S/3;
	       	    gridCell->firstElement = element;

			}
			else
			{
				//Dirichlet type: pressure specified, but not velocity
				if (leftBoundaryType == 'p')
				{
					//First Row: vL. The quantity pL is known

					element = new Element;
					element->column = 5*gridCell->peanoNumber;
		       	    element->cell = gridCell;
		       	    element->right = false;
		       	    element->value = gridCell->S/3;
		       	    gridCell->firstElement = element;
				}
				else //Neumann type: velocity specified, but not pressure
				{
					//Change first row to pL

					element = new Element;
					element->column = 5*gridCell->peanoNumber;
					element->cell = gridCell;
					element->right = false;
					element->vel = false;
					element->value = -1;
		       	    gridCell->firstElement = element;
				}
			}

			if (!gridCell->rightBoundary)
			{
				//Second Row: pR

				element = new Element;
				element->column = 5*gridCell->peanoNumber+ 1;
	    		element->right = true;
				element->vel = false;
				element->value = 1;
				element->cell = gridCell;
	       	    gridCell->secondElement = element;
			}
			else
			{
				//Dirichlet type: pressure specified, but not velocity
				if (rightBoundaryType == 'p')
				{
					//Change second row to vR

					element = new Element;
					element->column = 5*gridCell->peanoNumber+ 1;
		       	    element->cell = gridCell;
		       	    element->vel = true;
					element->right = true;
					element->value = gridCell->S/3;
		       	    gridCell->secondElement = element;
				}
				else //Neumann type: velocity specified, but not pressure
				{
					//Second Row: pR. The quantity vR is known

					element = new Element;
					element->column = 5*gridCell->peanoNumber+ 1;
		    		element->right = true;
					element->vel = false;
					element->cell = gridCell;
					element->value = 1;
		       	    gridCell->secondElement = element;
				}
			}

			if (!gridCell->upperBoundary)
			{

				//Third Row: vU

				element = new Element;
				element->column = 5*gridCell->peanoNumber+2;
				element->right = false;
				element->horizontal = false;
				element->vel = true;
	       	    element->cell = gridCell;
	       	    element->value = gridCell->S/3;
	       	    gridCell->thirdElement = element;

			}
			else
			{
				//Dirichlet type: pressure specified, but not velocity
				if (upperBoundaryType == 'p')
				{
					//Third Row: vU. The quantity pU is known
					element = new Element;
					element->column = 5*gridCell->peanoNumber+2;
					element->right = false;
					element->horizontal = false;
					element->vel = true;
		       	    element->cell = gridCell;
		       	    element->value = gridCell->S/3;
		       	    gridCell->thirdElement = element;

				}
				else //Neumann type: velocity specified, but not pressure
				{
					//Change third row to pU

					element = new Element;
					element->column = 5*gridCell->peanoNumber+2;
					element->right = false;
					element->horizontal = false;
					element->vel = false;
		       	    element->cell = gridCell;
		       	    element->value = -1;
		       	    gridCell->thirdElement = element;

				}
			}

			if (!gridCell->lowerBoundary)
			{

				//Fourth Row: pD

				element = new Element;
				element->column = 5*gridCell->peanoNumber+3;
				element->right = true;
				element->horizontal = false;
				element->vel = false;
	       	    element->cell = gridCell;
	       	    element->value = 1;
	       	    gridCell->fourthElement = element;

			}
			else
			{
				//Dirichlet type: pressure specified, but not velocity
				if (lowerBoundaryType == 'p')
				{
					//Change the fourth row to vD
					element = new Element;
					element->column = 5*gridCell->peanoNumber+3;
					element->right = true;
					element->horizontal = false;
					element->vel = true;
		       	    element->cell = gridCell;
		       	    element->value = gridCell->S/3;
		       	    gridCell->fourthElement = element;

				}
				else //Neumann type: velocity specified, but not pressure
				{
					//Fourth row: pD

					element = new Element;
					element->column = 5*gridCell->peanoNumber+3;
					element->right = true;
					element->horizontal = false;
					element->vel = false;
		       	    element->cell = gridCell;
		       	    element->value = 1;
		       	    gridCell->fourthElement = element;

				}
			}

			//Fifth and last row: pressure inside the cell

			element = new Element;
			element->column = 5*gridCell->peanoNumber+4;
			element->pressure = true;
       	    element->cell = gridCell;
       	    element->value = 2;
       	    gridCell->fifthElement = element;

   	    gridCell = gridCell->next;
    }
}

/*______________________________________________________________________________

                      FUNCTION fillDiscretizationMatrixElementsFull()

______________________________________________________________________________*/


void Grid::fillDiscretizationMatrixElementsFull( )
{
	Cell *gridCell;
	Element *element;
	Element *auxiliarElement;

	gridCell = firstGridCell;
	while(gridCell != 0)
	{

		/********* Primeira linha **********/

		element = gridCell->firstElement;

		//vL or pL is already set in initializeDiagonalElementsFull
		//1/3*S*vL - pL + 1/6*S*vR + pressure = 0

		if (!gridCell->leftBoundary)
		{
			//Means that vL is the main variable of this line

			//pL = pR_{ i-1, j}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->west;
			auxiliarElement->right = true;
			auxiliarElement->vel = false;
       	    auxiliarElement->value = -1;
       	    auxiliarElement->column = auxiliarElement->cell->secondElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			if (leftBoundaryType == 'p')
			{
				//vL is still the main variable of this line
				//pL is set by boundary condition
				gridCell->b += westBoundaryCondition(gridCell->centerY);
			}
			else
			{
				//pL is the main variable of this line
				//vL is set by boundary condition
				gridCell->b += -(gridCell->S/3)*velocityWestBoundaryCondition(gridCell->centerY);
			}
		}


		if (!gridCell->rightBoundary)
		{
			//vR is a variable
			//vR = vL_{ i+1, j}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->east;
			auxiliarElement->right = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/6;
       	    auxiliarElement->column = auxiliarElement->cell->firstElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{

			if(rightBoundaryType == 'p')
			{
				//vR is a variable
				//vR is the main variable of the second row
				auxiliarElement = new Element;
				auxiliarElement->cell = gridCell;
				auxiliarElement->right = true;
				auxiliarElement->vel = true;
	       	    auxiliarElement->value = gridCell->S/6;
	       	    auxiliarElement->column = auxiliarElement->cell->secondElement->column;
	       	    element->next = auxiliarElement;
	       	    element = element->next;
			}
			else
			{
				//gridCell->rightBoundary =  true && rightBoundaryType == 'v'
				//vR is set by boundary condition
				gridCell->b += -(gridCell->S/6)*velocityEastBoundaryCondition(gridCell->centerY);
			}
		}

		//pressure

		auxiliarElement = new Element;
		auxiliarElement->cell = gridCell;
		auxiliarElement->pressure = true;
   	    auxiliarElement->value = 1;
   	    auxiliarElement->column = auxiliarElement->cell->fifthElement->column;
   	    element->next = auxiliarElement;
   	    //element = element->next;       Not needed: last element of the row


		/********* Segunda linha **********/

		element = gridCell->secondElement;

		//pR or vR is already set in initializeDiagonalElementsFull
		//pR + 1/3*S*vR + 1/6*S*vL - pressure = 0

		if (!gridCell->rightBoundary)
		{
			//Means that pR is the main variable of this line (already set)

			//vR = vL_{ i+1, j}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->east;
			auxiliarElement->right = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/3;
       	    auxiliarElement->column = auxiliarElement->cell->firstElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			if (rightBoundaryType == 'p')
			{
				//vR is the main variable of this line
				//pR is set by boundary condition
				gridCell->b_ -= eastBoundaryCondition(gridCell->centerY);
			}
			else
			{
				//pR is the main variable of this line
				//vR is set by boundary condition
				gridCell->b_ += -(gridCell->S/3)*velocityEastBoundaryCondition(gridCell->centerY);
			}
		}

		if (!gridCell->leftBoundary || leftBoundaryType == 'p')
		{
			//vL is a variable
			//vL = vL_{ i, j}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell;
			auxiliarElement->right = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/6;
       	    auxiliarElement->column = auxiliarElement->cell->firstElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			//gridCell->leftBoundary =  true && leftBoundaryType == 'v'
			//vL is set by boundary condition
			gridCell->b_ += -(gridCell->S/6)*velocityWestBoundaryCondition(gridCell->centerY);
		}

		//pressure

		auxiliarElement = new Element;
		auxiliarElement->cell = gridCell;
		auxiliarElement->pressure = true;
   	    auxiliarElement->value = -1;
   	    auxiliarElement->column = auxiliarElement->cell->fifthElement->column;
   	    element->next = auxiliarElement;
   	    //element = element->next;       Not needed: last element of the row

		/********* Terceira linha **********/

		element = gridCell->thirdElement;

		//vU or pU is already set in initializeDiagonalElementsFull()
		//1/3*S*vU - pU + 1/6*S*vD + pressure = 0

		if (!gridCell->upperBoundary)
		{
			//Means that vU is the main variable of this line

			//pU = pD_{ i, j-1}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->north;
			auxiliarElement->right = true;
			auxiliarElement->horizontal = false;
			auxiliarElement->vel = false;
       	    auxiliarElement->value = -1;
       	    auxiliarElement->column = auxiliarElement->cell->fourthElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			if (upperBoundaryType == 'p')
			{
				//vU is still the main variable of this line
				//pU is set by boundary condition
				gridCell->bThird += northBoundaryCondition(gridCell->centerX);
			}
			else
			{
				//pU is the main variable of this line
				//vU is set by boundary condition
				gridCell->bThird += -(gridCell->S/3)*velocityNorthBoundaryCondition(gridCell->centerX);
			}
		}


		if (!gridCell->lowerBoundary)
		{
			//vD is a variable
			//vD = vU_{ i, j+1}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->south;
			auxiliarElement->right = false;
			auxiliarElement->horizontal = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/6;
       	    auxiliarElement->column = auxiliarElement->cell->thirdElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			if(lowerBoundaryType == 'p')
			{
				//vD is a variable
				//vD is the main variable of the fourth row
				auxiliarElement = new Element;
				auxiliarElement->cell = gridCell;
				auxiliarElement->right = true;
				auxiliarElement->horizontal = false;
				auxiliarElement->vel = true;
	       	    auxiliarElement->value = gridCell->S/6;
	       	    auxiliarElement->column = auxiliarElement->cell->fourthElement->column;
	       	    element->next = auxiliarElement;
	       	    element = element->next;
			}
			else
			{
				//gridCell->lowerBoundary =  true && lowerBoundaryType == 'v'
				//vD is set by boundary condition
				gridCell->bThird += -(gridCell->S/6)*velocitySouthBoundaryCondition(gridCell->centerX);
			}
		}

		//pressure

		auxiliarElement = new Element;
		auxiliarElement->cell = gridCell;
		auxiliarElement->pressure = true;
   	    auxiliarElement->value = 1;
   	    auxiliarElement->column = auxiliarElement->cell->fifthElement->column;
   	    element->next = auxiliarElement;
   	    //element = element->next;       Not needed: last element of the row


		/********* Quarta linha **********/

		element = gridCell->fourthElement;

		//pD or vD is already set in initializeDiagonalElementsFull
		//pD + 1/3*S*vD + 1/6*S*vU - pressure = 0

		if (!gridCell->lowerBoundary)
		{
			//Means that pD is the main variable of this line (already set)

			//vD = vU_{ i, j+1}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->south;
			auxiliarElement->right = false;
			auxiliarElement->horizontal = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/3;
       	    auxiliarElement->column = auxiliarElement->cell->thirdElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			if (lowerBoundaryType == 'p')
			{
				//vD is the main variable of this line
				//pD is set by boundary condition
				gridCell->bFourth -= southBoundaryCondition(gridCell->centerX);
			}
			else
			{
				//pD is the main variable of this line
				//vD is set by boundary condition
				gridCell->bFourth += -(gridCell->S/3)*velocitySouthBoundaryCondition(gridCell->centerX);
			}
		}


		if (!gridCell->upperBoundary || upperBoundaryType == 'p')
		{
			//vU is a variable
			//vU = vU_{ i, j}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell;
			auxiliarElement->right = false;
			auxiliarElement->horizontal = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/6;
       	    auxiliarElement->column = auxiliarElement->cell->thirdElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			//gridCell->upperBoundary =  true && upperBoundaryType == 'v'
			//vU is set by boundary condition
			gridCell->bFourth += -(gridCell->S/6)*velocityNorthBoundaryCondition(gridCell->centerX);
		}

		//pressure

		auxiliarElement = new Element;
		auxiliarElement->cell = gridCell;
		auxiliarElement->pressure = true;
   	    auxiliarElement->value = -1;
   	    auxiliarElement->column = auxiliarElement->cell->fifthElement->column;
   	    element->next = auxiliarElement;
   	    //element = element->next;       Not needed: last element of the row


		/********* Quinta linha **********/

		element = gridCell->fifthElement;

		// 2*pressure + 1/2*S*vL - pL + 1/2*vU - pU = 0
		// pressure is the main variable

		if (!gridCell->leftBoundary)
		{
			//Means that vL is stored on this cell, and pL in the left neighbour
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell;
			auxiliarElement->right = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/2;
       	    auxiliarElement->column = auxiliarElement->cell->firstElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;

       	   	//pL = pR_{ i-1, j}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->west;
			auxiliarElement->right = true;
			auxiliarElement->vel = false;
       	    auxiliarElement->value = -1;
       	    auxiliarElement->column = auxiliarElement->cell->secondElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			//either pL or vL are given by boundary condition
			if(leftBoundaryType == 'p')
			{
				//pL is given and vL is unknown

				// vL
				auxiliarElement = new Element;
				auxiliarElement->cell = gridCell;
				auxiliarElement->right = false;
				auxiliarElement->vel = true;
	       	    auxiliarElement->value = gridCell->S/2;
	       	    auxiliarElement->column = auxiliarElement->cell->firstElement->column;
	       	    element->next = auxiliarElement;
	       	    element = element->next;

	       	    // pL
	       	    gridCell->bFifth += westBoundaryCondition(gridCell->centerY);
			}
			else
			{
				//vL is given and pL is stored in the cell
				gridCell->bFifth -= (gridCell->S/2)*velocityWestBoundaryCondition(gridCell->centerY);

				//pL
				auxiliarElement = new Element;
				auxiliarElement->cell = gridCell;
				auxiliarElement->right = false;
				auxiliarElement->vel = false;
		   	    auxiliarElement->value = -1;
		   	    auxiliarElement->column = auxiliarElement->cell->secondElement->column;
		   	    element->next = auxiliarElement;
		   	    element = element->next;
			}

		}


		if (!gridCell->upperBoundary)
		{
			//Means that vU is stored on this cell, and pU in the upper neighbour

			// vU
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell;
			auxiliarElement->right = false;
			auxiliarElement->horizontal = false;
			auxiliarElement->vel = true;
       	    auxiliarElement->value = gridCell->S/2;
       	    auxiliarElement->column = auxiliarElement->cell->thirdElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;

       	   	//pU = pD_{ i, j-1}
			auxiliarElement = new Element;
			auxiliarElement->cell = gridCell->north;
			auxiliarElement->right = true;
			auxiliarElement->horizontal = false;
			auxiliarElement->vel = false;
       	    auxiliarElement->value = -1;
       	    auxiliarElement->column = auxiliarElement->cell->fourthElement->column;
       	    element->next = auxiliarElement;
       	    element = element->next;
		}
		else
		{
			//either pL or vL are given by boundary condition
			if(upperBoundaryType == 'p')
			{
				//pU is given and vU is unknown

				// vU
				auxiliarElement = new Element;
				auxiliarElement->cell = gridCell;
				auxiliarElement->right = false;
				auxiliarElement->horizontal = false;
				auxiliarElement->vel = true;
	       	    auxiliarElement->value = gridCell->S/2;
	       	    auxiliarElement->column = auxiliarElement->cell->thirdElement->column;
	       	    element->next = auxiliarElement;
	       	    element = element->next;

	       	    // pU
	       	    gridCell->bFifth += northBoundaryCondition(gridCell->centerX);
			}
			else
			{
				//vU is given and pU is stored in the cell
				gridCell->bFifth -= (gridCell->S/2)*velocityNorthBoundaryCondition(gridCell->centerX);

				//pU
				auxiliarElement = new Element;
				auxiliarElement->cell = gridCell;
				auxiliarElement->right = false;
				auxiliarElement->horizontal = false;
				auxiliarElement->vel = false;
		   	    auxiliarElement->value = -1;
		   	    auxiliarElement->column = auxiliarElement->cell->thirdElement->column;
		   	    element->next = auxiliarElement;
		   	    element = element->next;
			}

		}



		gridCell = gridCell->next;
	}


}

/*______________________________________________________________________________

                      FUNCTION calculateFullData()

    Sets v and p for all edges of the grid;
______________________________________________________________________________*/
void Grid::calculateFullData()
{
	Cell *gridCell;
	double A;

	currentDeltaT = 1.0;

	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		if(!gridCell->leftBoundary)
		{
			gridCell->pL = gridCell->west->pR;
		}
		else
		{
			if(leftBoundaryType == 'p')
				gridCell->pL = westBoundaryCondition(gridCell->centerY);
			if(leftBoundaryType == 'v')
				gridCell->vL = velocityWestBoundaryCondition(gridCell->centerY);
		}

		if(!gridCell->rightBoundary)
		{
			gridCell->vR = gridCell->east->vL;
		}
		else
		{
			if(rightBoundaryType == 'p')
				gridCell->pR = eastBoundaryCondition(gridCell->centerY);
			if(rightBoundaryType == 'v')
				gridCell->vR = velocityEastBoundaryCondition(gridCell->centerY);
		}

		if(!gridCell->upperBoundary)
		{
			gridCell->pU = gridCell->north->pD;
		}
		else
		{
			if(upperBoundaryType == 'p')
				gridCell->pU = northBoundaryCondition(gridCell->centerX);
			if(upperBoundaryType == 'v')
				gridCell->vU = velocityNorthBoundaryCondition(gridCell->centerX);
		}

		if(!gridCell->lowerBoundary)
		{
			gridCell->vD = gridCell->south->vU;
		}
		else
		{
			if(lowerBoundaryType == 'p')
				gridCell->pD = southBoundaryCondition(gridCell->centerX);
			if(lowerBoundaryType == 'v')
				gridCell->vD = velocitySouthBoundaryCondition(gridCell->centerX);
		}
		/*
		A = gridCell->faceLength*gridCell->faceLength*phi;

		if ( fabs(A/( fmax(gridCell->vR,0) - fmin(gridCell->vL,0)) ) < currentDeltaT)
		currentDeltaT = A/( fmax(gridCell->vR,0) - fmin(gridCell->vL,0) );
		//cout << "currentDeltaT = " << currentDeltaT << endl;
		if ( fabs(A/( fmax(gridCell->vD,0) - fmin(gridCell->vU,0)) ) < currentDeltaT)
		currentDeltaT = A/( fmax(gridCell->vD,0) - fmin(gridCell->vU,0) );
		*/

		gridCell = gridCell->next;
	}
	//cout << "current delta T = " << currentDeltaT << endl;
	//system("pause");

}

/*______________________________________________________________________________

                            FUNCTION solveFullDarcy()

     Solves Darcy's equation, in the general case, through the following steps:

	 1) Initializes grid.
     2) Sets grid discretization matrix A and constant term b. (5 lines per cell)
     3) Solves linear system Ax = b by conjugate gradient method.

     Its boolean argument will determine if the program will draw on the screen
     the grid and its associated Peano's curve (true) or not (false).

//____________________________________________________________________________*/

void Grid::solveFullDarcy(bool drawGrid, int refinementLevel)
{
	Cell *gridCell;
	Cell *aux;
	Element *element;
	Element *auxiliarElement;
	double biConjugateGradientTotalTime = 0;

	orderGridCells();
	setM();
	initializeDiagonalElementsFull();
	fillDiscretizationMatrixElementsFull( );
	//if(refinementLevel >4)
	//{

		printFullGridMatrix();
		printB();
	//}
	//system("pause");

/* Editado por Yuri */
////////////////////////////////////////////////////////////////////////////
	double biConjugateGradientTime = glutGet(GLUT_ELAPSED_TIME) / hertz;

	if(refinementLevel <= 3)
		biConjugateGradientStabilized( );
	else
	{
	/* Editado por Yuri */
        // Aqui sera colocado a leitura por matlab
        MATLAB_READ
	}

	biConjugateGradientTotalTime = glutGet(GLUT_ELAPSED_TIME) / hertz - biConjugateGradientTime;
	//log

/*
    CPerfTimer biConjugateGradientTime;
    biConjugateGradientTime.Start();
	if(refinementLevel <= 3)
		biConjugateGradientStabilized( );
	else
	{
		system("D:\\Yuri\\Documents\\GitHub\\GRIDPROUS_YURI\\bin\\Debug\\a.exe ");
		readMatlab();
	}
    biConjugateGradientTime.Stop();
    biConjugateGradientTotalTime += biConjugateGradientTime.Elapsedms()/1000;
*/
////////////////////////////////////////////////////////////////////////////

	cout << "Tempo gasto no Gradiente Biconjugado: " << biConjugateGradientTotalTime << endl;
	//system("pause");
	calculateFullData();
	//calculateVolumeFluxes(currentDeltaT);

}

/*______________________________________________________________________________

                FUNCTION readMatlab()

     Reads the output from matlab

//____________________________________________________________________________*/
void Grid::readMatlab()
{
	Cell *gridCell;
	double flag;
	double aux;

	std::ifstream result( "result.txt" );  //  The input file
	result >> flag;

	cout << "flag = " << flag << endl;

	gridCell = firstGridCell;
	while(gridCell!= 0)
	{
		if(!gridCell->leftBoundary || leftBoundaryType == 'p')
		{
			result >> gridCell->vL;
            cout << gridCell->vL << endl;
		}
		else
		{
			result >> gridCell->pL;
			cout << gridCell->pL << endl;
		}

		if(!gridCell->rightBoundary || rightBoundaryType == 'v')
		{
			result >> gridCell->pR;
		}
		else
		{
			result >> gridCell->vR;
		}


		if(!gridCell->upperBoundary || upperBoundaryType == 'p')
		{
			result >> gridCell->vU;
			cout << gridCell->vU << endl;
		}
		else
		{
			result >> gridCell->pU;
			cout << gridCell->pU << endl;
		}

		if(!gridCell->lowerBoundary || lowerBoundaryType == 'v')
		{
			result >> gridCell->pD;
			cout << gridCell->pD << endl;
		}
		else
		{
			result >> gridCell->vD;
			cout << gridCell->vD << endl;
		}

		result >> gridCell->pressure;

		gridCell = gridCell->next;
	}

}




/*______________________________________________________________________________

                FUNCTION biconjugateGradientStabilized()

     Solves the full system

//____________________________________________________________________________*/
void Grid::biConjugateGradientStabilized( )
{
	Cell *gridCell;
    Element *element;

    double error = 0.0,	     //Euclidean norm of second resídue r after each iteration.
           tolerance = 1e-9, //Error tolerance
           pNew,
           rho = 1.0,
           rhoNew,
           alpha = 1.0,
           beta,
           omega = 1.0,
           r1ScalarAp,
           rScalarR = 0.0,
           sScalarAs,
           AsScalarAs;

    int numberOfIterations = 0;
    int iteration = 500*numberOfCells;

    //__________________________________________________________________________
    //Sets Ax. Uses Ap to store Ax.
	//Our initial condition is zero. Thus, r is set as b - Ax = b.
    //Sets second residual vector r1 as r (r1 is an arbitrary vector, not
    //orthogonal to r).
    gridCell = firstGridCell;
    while( gridCell != 0 )
    {
        gridCell->Ap = 0.0;
        gridCell->Ap_ = 0.0;
        gridCell->ApThird = 0.0;
        gridCell->ApFourth = 0.0;
        gridCell->ApFifth = 0.0;

        //First Line
        element = gridCell->firstElement;
        while( element != 0 )
        {

			gridCell->Ap += element->pressure * element->value * element->cell->pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vL        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vD

			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pL
			+(1-element->pressure)*(element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pD;

            element = element->next;
        }

        //Second Line
        element = gridCell->secondElement;
        while( element != 0 )
        {

			gridCell->Ap_ += element->pressure * element->value * element->cell->pressure								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vL        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vD

			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pL
			+(1-element->pressure)*(element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pD;

            element = element->next;
        }

        //Third Line
        element = gridCell->thirdElement;
        while( element != 0 )
        {

			gridCell->ApThird += element->pressure * element->value * element->cell->pressure								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vL        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vD

			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pL
			+(1-element->pressure)*(element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pD;

            element = element->next;
        }

        //Fourth Line
        element = gridCell->fourthElement;
        while( element != 0 )
        {

			gridCell->ApFourth += element->pressure * element->value * element->cell->pressure								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vL        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vD

			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pL
			+(1-element->pressure)*(element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pD;

            element = element->next;
        }

        //Fifth Line
        element = gridCell->fifthElement;
        while( element != 0 )
        {

			gridCell->ApFifth += element->pressure * element->value * element->cell->pressure								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vL        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(element->vel)*element->value * element->cell->vU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(element->vel)*element->value * element->cell->vD

			+(1-element->pressure)*(element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pL
			+(1-element->pressure)*(element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pR
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*(1-element->vel)*element->value * element->cell->pU
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*(1-element->vel)*element->value * element->cell->pD;

            element = element->next;
        }






        gridCell->r = gridCell->b;// - gridCell->Ap;
        //cout << gridCell->b << endl << gridCell->b_ << endl << gridCell->bThird << endl << gridCell->bFourth << endl << gridCell->bFifth << endl ;
        gridCell->r_ = gridCell->b_;// - gridCell->Ap_;
        gridCell->rThird = gridCell->bThird;// - gridCell->ApThird;
        gridCell->rFourth = gridCell->bFourth;// - gridCell->ApFourth;
        gridCell->rFifth = gridCell->bFifth;// - gridCell->ApFifth;



        rScalarR += gridCell->r * gridCell->r;
        rScalarR += gridCell->r_ * gridCell->r_;
        rScalarR += gridCell->rThird * gridCell->rThird;
        rScalarR += gridCell->rFourth * gridCell->rFourth;
        rScalarR += gridCell->rFifth * gridCell->rFifth;


        gridCell = gridCell->next;
    }

    error = sqrt(rScalarR);




    gridCell = firstGridCell;
    while( gridCell != 0 )
    {

		gridCell->r1 = gridCell->r;         //r_ should never be orthogonal to r.
		gridCell->r1_ = gridCell->r_;
		gridCell->r1Third = gridCell->rThird;
		gridCell->r1Fourth = gridCell->rFourth;
		gridCell->r1Fifth = gridCell->rFifth;


		/*
		//Setting the initial r1
		gridCell->r1 = 0.0;
        element = gridCell->firstElement;
        while( element != 0 )
        {

			gridCell->r1 += element->pressure * element->value * element->cell->rFifth								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

            element = element->next;
        }

		gridCell->r1_ = 0.0;
        element = gridCell->secondElement;
        while( element != 0 )
        {

			gridCell->r1_ += element->pressure * element->value * element->cell->rFifth								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

            element = element->next;
        }

		gridCell->r1Third = 0.0;
        element = gridCell->thirdElement;
        while( element != 0 )
        {

			gridCell->r1Third += element->pressure * element->value * element->cell->rFifth								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

            element = element->next;
        }

        gridCell->r1Fourth = 0.0;
        element = gridCell->fourthElement;
        while( element != 0 )
        {

			gridCell->r1Fourth += element->pressure * element->value * element->cell->rFifth								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

            element = element->next;
        }

		gridCell->r1Fifth = 0.0;
        element = gridCell->fifthElement;
        while( element != 0 )
        {

			gridCell->r1Fifth += element->pressure * element->value * element->cell->rFifth								 //pressure
			+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
			+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
			+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
			+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

            element = element->next;
        }
        // End of r1 setting
		*/



		gridCell = gridCell->next;

	}


	//cout << "error = "<< error << endl;
	//system("pause");
    //__________________________________________________________________________
    //Biconjugate gradient stabilized iterations.
    while( numberOfIterations < iteration && error > tolerance )
    {
        //cout  << "entrou no while" << endl;
        //if(numberOfIterations % 10 == 0)
        //system("pause");
		//______________________________________________________________________
        //Computes rhoNew = < r1, r >.
        gridCell = firstGridCell;
        rhoNew = 0.0;
        while( gridCell != 0 )
        {
            rhoNew += gridCell->r1 * gridCell->r;
            rhoNew += gridCell->r1_ * gridCell->r_;
            rhoNew += gridCell->r1Third * gridCell->rThird;
            rhoNew += gridCell->r1Fourth * gridCell->rFourth;
            rhoNew += gridCell->r1Fifth * gridCell->rFifth;

            gridCell = gridCell->next;
        }

        if(rhoNew == 0 || error > 1e5)//|| (numberOfIterations % 10000 == 0 && error > 1))
        {
			gridCell = firstGridCell;
			while( gridCell != 0 )
        	{

				gridCell->r1 = -gridCell->r;         //restarts the algorithm.
				gridCell->r1_ = -gridCell->r_;
				gridCell->r1Third = -gridCell->rThird;
				gridCell->r1Fourth = -gridCell->rFourth;
				gridCell->r1Fifth = -gridCell->rFifth;

				/*
				//Setting the initial r1
				gridCell->r1 = 0.0;
		        element = gridCell->firstElement;
		        while( element != 0 )
		        {

					gridCell->r1 += element->pressure * element->value * element->cell->rFifth								 //pressure
					+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
					+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
					+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
					+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

		            element = element->next;
		        }

				gridCell->r1_ = 0.0;
		        element = gridCell->secondElement;
		        while( element != 0 )
		        {

					gridCell->r1_ += element->pressure * element->value * element->cell->rFifth								 //pressure
					+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
					+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
					+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
					+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

		            element = element->next;
		        }

				gridCell->r1Third = 0.0;
		        element = gridCell->thirdElement;
		        while( element != 0 )
		        {

					gridCell->r1Third += element->pressure * element->value * element->cell->rFifth								 //pressure
					+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
					+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
					+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
					+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

		            element = element->next;
		        }

		        gridCell->r1Fourth = 0.0;
		        element = gridCell->fourthElement;
		        while( element != 0 )
		        {

					gridCell->r1Fourth += element->pressure * element->value * element->cell->rFifth								 //pressure
					+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
					+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
					+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
					+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

		            element = element->next;
		        }

				gridCell->r1Fifth = 0.0;
		        element = gridCell->fifthElement;
		        while( element != 0 )
		        {

					gridCell->r1Fifth += element->pressure * element->value * element->cell->rFifth								 //pressure
					+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->r        //left value
					+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->r_         //right value
					+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->rThird //upper value
					+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->rFourth; //bottom value

		            element = element->next;
		        }
		        // End of r1 setting
				*/

				rhoNew += gridCell->r1 * gridCell->r;
	            rhoNew += gridCell->r1_ * gridCell->r_;
	            rhoNew += gridCell->r1Third * gridCell->rThird;
	            rhoNew += gridCell->r1Fourth * gridCell->rFourth;
	            rhoNew += gridCell->r1Fifth * gridCell->rFifth;

				gridCell = gridCell->next;
			}
			cout << "restarting" << endl;
			//system("pause");
		}

        //______________________________________________________________________
        //Computes beta and updates rho.
        beta = (rhoNew/rho)*(alpha/omega);
        //cout << "beta =  " << beta << endl;
        rho = rhoNew;
        //cout << "rhoNew =  " << rho << endl;
        //______________________________________________________________________
        //Updates p.
        gridCell = firstGridCell;
        while( gridCell != 0 )
        {
            gridCell->p = gridCell->r + beta*( gridCell->p - omega*gridCell->Ap );
            gridCell->p_ = gridCell->r_ + beta*( gridCell->p_ - omega*gridCell->Ap_ );
            gridCell->pThird = gridCell->rThird + beta*( gridCell->pThird - omega*gridCell->ApThird );
            gridCell->pFourth = gridCell->rFourth + beta*( gridCell->pFourth - omega*gridCell->ApFourth );
            gridCell->pFifth = gridCell->rFifth + beta*( gridCell->pFifth - omega*gridCell->ApFifth );

            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes Ap and < r1, Ap >.
        gridCell = firstGridCell;
        r1ScalarAp = 0.0;
        while( gridCell != 0 )
        {
            //First Line
			gridCell->Ap = 0.0;
            element = gridCell->firstElement;
            while( element != 0 )
            {

				gridCell->Ap += element->pressure * element->value * element->cell->pFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->p        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->p_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->pThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->pFourth; //bottom value

                element = element->next;
            }
            r1ScalarAp += gridCell->r1 * gridCell->Ap;

            //Second Line
            gridCell->Ap_ = 0.0;
            element = gridCell->secondElement;
            while( element != 0 )
            {

				gridCell->Ap_ += element->pressure * element->value * element->cell->pFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->p        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->p_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->pThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->pFourth; //bottom value

                element = element->next;
            }
            r1ScalarAp += gridCell->r1_ * gridCell->Ap_;

			//Third line
			gridCell->ApThird = 0.0;
            element = gridCell->thirdElement;
            while( element != 0 )
            {

				gridCell->ApThird += element->pressure * element->value * element->cell->pFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->p        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->p_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->pThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->pFourth; //bottom value

                element = element->next;
            }
            r1ScalarAp += gridCell->r1Third * gridCell->ApThird;

			//Fourth line
			gridCell->ApFourth = 0.0;
            element = gridCell->fourthElement;
            while( element != 0 )
            {

				gridCell->ApFourth += element->pressure * element->value * element->cell->pFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->p        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->p_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->pThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->pFourth; //bottom value

                element = element->next;
            }
            r1ScalarAp += gridCell->r1Fourth * gridCell->ApFourth;

			//Fifth line
			gridCell->ApFifth = 0.0;
            element = gridCell->fifthElement;
            while( element != 0 )
            {

				gridCell->ApFifth += element->pressure * element->value * element->cell->pFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->p        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->p_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->pThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->pFourth; //bottom value

                element = element->next;
            }
            r1ScalarAp += gridCell->r1Fifth * gridCell->ApFifth;

			//cout << "r1ScalarAp = " << r1ScalarAp << endl;

            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes alpha.
        alpha = rhoNew/r1ScalarAp;
        //cout << "alpha =  " << alpha << endl;
        //______________________________________________________________________
        //Updates second residue s = r - alpha*Ap and sScalarS.
        gridCell = firstGridCell;
        while( gridCell != 0 )
        {
            gridCell->s = gridCell->r - alpha * gridCell->Ap;
            gridCell->s_ = gridCell->r_ - alpha * gridCell->Ap_;
            gridCell->sThird = gridCell->rThird - alpha * gridCell->ApThird;
            gridCell->sFourth = gridCell->rFourth - alpha * gridCell->ApFourth;
			gridCell->sFifth = gridCell->rFifth - alpha * gridCell->ApFifth;

			gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes As, sScalarAs and AsScalarAs.
        gridCell = firstGridCell;
        sScalarAs = 0.0;
        AsScalarAs = 0.0;
        while( gridCell != 0 )
        {
			//First Line
			gridCell->As = 0.0;
            element = gridCell->firstElement;
            while( element != 0 )
            {

				gridCell->As += element->pressure * element->value * element->cell->sFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->s        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->s_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->sThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->sFourth; //bottom value

                element = element->next;
            }
			sScalarAs += gridCell->s * gridCell->As;
            AsScalarAs += gridCell->As * gridCell->As;

            //Second Line
			gridCell->As_ = 0.0;
            element = gridCell->secondElement;
            while( element != 0 )
            {
				gridCell->As_ += element->pressure * element->value * element->cell->sFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->s        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->s_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->sThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->sFourth; //bottom value

                element = element->next;
            }
			sScalarAs += gridCell->s_ * gridCell->As_;
            AsScalarAs += gridCell->As_ * gridCell->As_;


            //Third Line
			gridCell->AsThird = 0.0;
            element = gridCell->thirdElement;
            while( element != 0 )
            {
				gridCell->AsThird += element->pressure * element->value * element->cell->sFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->s        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->s_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->sThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->sFourth; //bottom value

                element = element->next;
            }
			sScalarAs += gridCell->sThird * gridCell->AsThird;
            AsScalarAs += gridCell->AsThird * gridCell->AsThird;

            //Fourth Line
			gridCell->AsFourth = 0.0;
            element = gridCell->fourthElement;
            while( element != 0 )
            {
				gridCell->AsFourth += element->pressure * element->value * element->cell->sFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->s        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->s_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->sThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->sFourth; //bottom value

                element = element->next;
            }
			sScalarAs += gridCell->sFourth * gridCell->AsFourth;
            AsScalarAs += gridCell->AsFourth * gridCell->AsFourth;

            //Fifth Line
			gridCell->AsFifth = 0.0;
            element = gridCell->fifthElement;
            while( element != 0 )
            {
				gridCell->AsFifth += element->pressure * element->value * element->cell->sFifth								 //pressure
				+(1-element->pressure)*(element->horizontal)*(1-element->right)*element->value * element->cell->s        //left value
				+(1-element->pressure)*(element->horizontal)*(element->right)*element->value * element->cell->s_         //right value
				+(1-element->pressure)*(1-element->horizontal)*(1-element->right)*element->value * element->cell->sThird //upper value
				+(1-element->pressure)*(1-element->horizontal)*(element->right)*element->value * element->cell->sFourth; //bottom value

                element = element->next;
            }
			sScalarAs += gridCell->sFifth * gridCell->AsFifth;
            AsScalarAs += gridCell->AsFifth * gridCell->AsFifth;




            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Computes omega.
        omega = sScalarAs/AsScalarAs;
        //cout << "omega =  " << omega << endl;
        //______________________________________________________________________
        //Updates solution x = x + alpha*p + omega*s and computes
        //residue r = s - omega*As and <r,r>.
        gridCell = firstGridCell;
        rScalarR = 0.0;
        while( gridCell != 0 )
        {
            if(!gridCell->leftBoundary || leftBoundaryType == 'p')
				gridCell->vL += alpha*gridCell->p + omega*gridCell->s;
			else
				gridCell->pL += alpha*gridCell->p + omega*gridCell->s;

			if(!gridCell->rightBoundary || rightBoundaryType == 'v')
				gridCell->pR += alpha*gridCell->p_ + omega*gridCell->s_;
			else
				gridCell->vR += alpha*gridCell->p_ + omega*gridCell->s_;

			if(!gridCell->upperBoundary || upperBoundaryType == 'p')
				gridCell->vU += alpha*gridCell->pThird + omega*gridCell->sThird;
			else
				gridCell->pU += alpha*gridCell->pThird + omega*gridCell->sThird;

			if(!gridCell->lowerBoundary || lowerBoundaryType == 'v')
				gridCell->pD += alpha*gridCell->pFourth + omega*gridCell->sFourth;
			else
				gridCell->vD += alpha*gridCell->pFourth + omega*gridCell->sFourth;

			gridCell->pressure += alpha*gridCell->pFifth + omega*gridCell->sFifth;

			gridCell->r = gridCell->s - omega * gridCell->As;
			gridCell->r_ = gridCell->s_ - omega * gridCell->As_;
			gridCell->rThird = gridCell->sThird - omega * gridCell->AsThird;
			gridCell->rFourth = gridCell->sFourth - omega * gridCell->AsFourth;
			gridCell->rFifth = gridCell->sFifth - omega * gridCell->AsFifth;

			//cout << "iteration = " << numberOfIterations << endl;
            rScalarR += gridCell->r * gridCell->r;
            //if(numberOfIterations > 963)
			//cout << gridCell->r * gridCell->r << endl;
            rScalarR += gridCell->r_ * gridCell->r_;
            //cout << gridCell->r_ * gridCell->r_ << endl;
			rScalarR += gridCell->rThird * gridCell->rThird;
            //cout << gridCell->rThird * gridCell->rThird << endl;
			rScalarR += gridCell->rFourth * gridCell->rFourth;
            //cout << gridCell->rFourth * gridCell->rFourth << endl;
			rScalarR += gridCell->rFifth * gridCell->rFifth;
           // cout << gridCell->rFifth * gridCell->rFifth << endl;
            //if(numberOfIterations > 963)
            //system("pause");






            gridCell = gridCell->next;
        }
        //______________________________________________________________________
        //Updates error.
        error = sqrt(rScalarR);
		//if(numberOfIterations > 963)
		//cout << "error = " << error << endl;
		//if(numberOfIterations > 963)
		//cout << "iterations = " << numberOfIterations << endl;
		//system("pause");
        numberOfIterations++;
    }//end of biconjugate gradient stabilized iterations.


    cout << "Numero de Celulas: " << numberOfCells
     << "\tNumero de iteracoes do gradiente biconjugado: " << numberOfIterations << endl;
	 cout << "Erro = " << error << endl;
    //system("pause");
}//end biConjugateGradientStabilized() function.



/*______________________________________________________________________________

                            FUNCTION sign(a)

     Returns the sign of a. If a is zero, returns zero

//____________________________________________________________________________*/
double Grid::sign( double x)
{
	return (x > 0) - (x < 0 ) ;
}
/*______________________________________________________________________________

                            FUNCTION muscl(a,b)

  Calculates the slope of the linear reconstruction of the tracer concentrations

//____________________________________________________________________________*/
double Grid::muscl(double a, double b)
{
	if (a*b >= 0)
	{
		return 0;//sign(a)*fmin(2*fabs(a), fmin(2*fabs(b), fabs(a+b)/2 ));
	}
	else
	return 0;
}

/*______________________________________________________________________________

                            FUNCTION setInitialCondition

  		Sets concentrations on the initial time

//____________________________________________________________________________*/

void Grid::setInitialCondition( int numero)
{
	Cell *gridCell;
	initialMass = 0;

	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		if(numero == 1)
		{
			if((gridCell->centerX-0.5)*(gridCell->centerX-0.5) + (gridCell->centerY-0.5)*(gridCell->centerY-0.5) <=0.1)
			gridCell->c = 0.8;
			//gridCell->c = 1-2*(gridCell->centerX-0.5)*(gridCell->centerX-0.5)-2*(gridCell->centerY-0.5)*(gridCell->centerY-0.5);
			initialMass += gridCell->c;

			setBlueColor(gridCell->c);
			drawQuad(600*(gridCell->centerX-gridCell->faceLength/2),
					600*(gridCell->centerY-gridCell->faceLength/2),
					600*(gridCell->centerX+gridCell->faceLength/2),
					600*(gridCell->centerY+gridCell->faceLength/2));
		}

		if(numero == 2)
		{
			if( fabs(gridCell->centerX-0.5) < 0.2 && fabs(gridCell->centerY-0.5) < 0.2 )
			gridCell->c = 0.8;

			initialMass += gridCell->c*gridCell->faceLength*gridCell->faceLength*phi;

			setBlueColor(1-gridCell->c);
			drawQuad(600*(gridCell->centerX-gridCell->faceLength/2),
					600*(gridCell->centerY-gridCell->faceLength/2),
					600*(gridCell->centerX+gridCell->faceLength/2),
					600*(gridCell->centerY+gridCell->faceLength/2));
		}
		if(numero == 3)
		{
			if(gridCell->centerX < 0.2)// || gridCell->centerX+ gridCell->halfFaceLength == 1)
			{
				gridCell->c = 0.8;
			}
			initialMass += gridCell->c*gridCell->faceLength*gridCell->faceLength*phi;

			setBlueColor(gridCell->c);
			drawQuad(600*(gridCell->centerX-gridCell->faceLength/2),
					600*(gridCell->centerY-gridCell->faceLength/2),
					600*(gridCell->centerX+gridCell->faceLength/2),
					600*(gridCell->centerY+gridCell->faceLength/2));
		}
		gridCell = gridCell->next;
	}
	cout << "initial mass = " << initialMass << endl;

}


void Grid::drawPressure()
{
	double pMax = 1;
	double pMin = 0;
	Cell *gridCell;

	gridCell = firstGridCell;
	pMax = gridCell->pressure;
	pMin = gridCell->pressure;

	/*
	double calculatedVL = 0;
	double calculatedVR = 0;
	double calculatedVU = 0;
	double calculatedVD = 0;

	double twoToLevel = 1;
	double level;
	double meanOfPressures = 0;
	*/

	orderGridCells();
	while(gridCell != 0)
	{
		if(gridCell->pressure > pMax)
		pMax = gridCell->pressure;

		if(gridCell->pressure < pMin)
		pMin = gridCell->pressure;
		/*
		cout << "Cell " << gridCell->peanoNumber << endl;

		cout << "vR = " << gridCell->vR << endl;
		cout << "vL = " << gridCell->vL << endl;
		cout << "vU = " << gridCell->vU << endl;

		cout << "vU = " << (double) gridCell->vU/gridCell->faceLength << endl << endl;
		//if((gridCell->vU/gridCell->faceLength != -1))
			//system("pause");

		cout << "VR = " << gridCell->VR << endl;
		cout << "VL = " << gridCell->VL << endl;
		cout << "VU = " << gridCell->VU << endl;
		cout << "VD = " << gridCell->VD << endl << endl;

		cout << "pR = " << gridCell->pR << endl;
		cout << "pL = " << gridCell->pL << endl;
		cout << "pU = " << gridCell->pU << endl;
		cout << "pD = " << gridCell->pD << endl;

		cout << "pressure = " << gridCell->pressure << endl << endl;
		*/
		gridCell = gridCell->next;
	}


	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		//setBlueColor(1-(gridCell->vR - pMin)/(pMax-pMin));
		setBlueColor((gridCell->pressure -pMin )/(pMax-pMin));
		drawQuad(600*(gridCell->centerX-gridCell->faceLength/2), //+1
				600*(gridCell->centerY-gridCell->faceLength/2),  //+1
				600*(gridCell->centerX+gridCell->faceLength/2),  //-1
				600*(gridCell->centerY+gridCell->faceLength/2)); //-1

		//cout << gridCell->pressure << "  " << gridCell->peanoNumber << endl;

		gridCell = gridCell->next;
	}


}

/*______________________________________________________________________________

                    FUNCTION updateConcentration

	  Updates the time by deltaT units of time

//____________________________________________________________________________*/

void Grid::updateConcentrations( double deltaT)
{
	Cell *gridCell;
	double A;

	gridCell = firstGridCell;
	/*
	while(gridCell != 0)
	{
		gridCell->cNew = gridCell->c;
		gridCell = gridCell->next;
	}
	*/
	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		A = gridCell->faceLength*gridCell->faceLength*phi;
		gridCell->cNew = gridCell->c + (gridCell->southFlux + gridCell->northFlux + gridCell->eastFlux + gridCell->westFlux)/A;


		gridCell = gridCell->next;
	}


	initialMass = 0;
	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		A = gridCell->faceLength*gridCell->faceLength*phi;
		initialMass += gridCell->cNew*A;
		gridCell->c = gridCell->cNew;
		//cout << "Cell " << gridCell->peanoNumber << endl << gridCell->c <<endl;

		if  (gridCell->c < -1e-5)
		{
			cout << "low warning " << gridCell->c << " (" << gridCell->centerX << "," << gridCell->centerY << ")" << endl;
			//gridCell->c = 0;
		}

		if  (gridCell->c > 1)
		{
			cout << "warning " << gridCell->c << endl;
			gridCell->c = 1;
		}


		gridCell = gridCell->next;
	}
	//cout << "total mass = " << totalMass << endl;


	//Draws on screen
	gridCell = firstGridCell;
	while(gridCell != 0)
	{
		if  (gridCell->c < -1e-5)
		{
			setColor(1);
			drawQuad(600*(gridCell->centerX-gridCell->faceLength/2),
					600*(gridCell->centerY-gridCell->faceLength/2),
					600*(gridCell->centerX+gridCell->faceLength/2),
					600*(gridCell->centerY+gridCell->faceLength/2));


			//cout << "low warning " << gridCell->c << " (" << gridCell->centerX << "," << gridCell->centerY << ")" << endl;
			//gridCell->c = 0;

		}
		else
		{
			//if  (gridCell->c > 1)
			//gridCell->c = 1;
			if  (gridCell->c > 1)
			cout << "warning " << gridCell->c << endl;
			//cout << "c = " << gridCell->c << endl;

			if(0==1)//(gridCell->leftBoundary && (fabs(gridCell->c)>1e-9))
			{
				setColor(2);
				drawQuad(600*(gridCell->centerX-gridCell->faceLength/2),
						600*(gridCell->centerY-gridCell->faceLength/2),
						600*(gridCell->centerX+gridCell->faceLength/2),
						600*(gridCell->centerY+gridCell->faceLength/2));
				cout << "left warning " << gridCell->c << " (" << gridCell->centerX << "," << gridCell->centerY << ")" << endl;


			}
			else
			{
				setBlueColor(gridCell->c);
				drawQuad(600*(gridCell->centerX-gridCell->faceLength/2),  //+1
						600*(gridCell->centerY-gridCell->faceLength/2),  //+1
						600*(gridCell->centerX+gridCell->faceLength/2),  //-1
						600*(gridCell->centerY+gridCell->faceLength/2));  //-1
			}
		}
		setColor(0);
		gridCell = gridCell->next;
	}
	elapsedDeltaT += 0.95*deltaT;// - currentDeltaT;
	cout << "grid time elapsed = " << elapsedDeltaT << endl;
	//cout.precision(9);
	//cout << "deltaT = " << currentDeltaT << endl;
	//cout << "total mass  = " << totalMass << endl;

}





//end program Grid.cpp

