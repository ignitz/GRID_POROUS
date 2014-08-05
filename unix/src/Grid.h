/* Grid.h
Definition of class Grid.
An object Grid represents a square grid. This class is a singleton.
*/

#ifndef GRID_H
#define GRID_H

#include "Cell.h"


class Grid
{
    public:

        Grid();				 //constructor
        ~Grid();			 //destructor

        Cell *firstGridCell;
		Cell *auxiliarCell;

		Grid *auxiliarGrid;
        double sideLength;	//Lenght of the square grid. Default = 1.0.
        int numberOfCells;

        void initializeGrid( bool );
        void createAuxiliarCells();
        void drawPeanoCurve( int );
        void orderGridCells();
        void refineGridInitially( int, bool );

        //Print functions.
        int print();
        int printGridMatrix();
        int printFullGridMatrix();
        int printFullGridMatrixRefining();
        void printB();


    	//Previous calculations in order to solve the system
		void setM();

		//Methods used in solving "reduced" Darcy's equation.
        void solveReducedDarcy( bool, int );
		void calculatePressure();
        void calculateVelocities();

        //Methods used in solving Darcy in case of some no-flow boundary

        char leftBoundaryType;			/* 'p' indicates Dirichlet condition*/
        char rightBoundaryType;			/* (specified pressure) and 'v'     */
        char upperBoundaryType;			/* indicates Neumann condition      */
        char lowerBoundaryType;			/* (specified velocity)             */

        void solveFullDarcy( bool, int );
        void initializeDiagonalElementsFull();
        void fillDiscretizationMatrixElementsFull( );

        //Variables used on the convective calculations
        void setInitialCondition(int);
		double currentDeltaT;
        double elapsedDeltaT;

        double vMax;        //Maximum of the velocities on the whole grid
        double phi;         //Porosity of the medium
        double initialMass;

        Cell * setNeighbours( Cell *);

        void updateConcentrations(double);
        double muscl(double, double);
        double sideConcentration( Cell *, char );
		double correctedSideConcentration( Cell *, char );
		void drawPressure();
        void calculateFullData();
        void calculateVolumeFluxes(double);
        void calculateDeltaT();
        bool refineGrid( int, double, bool );

    private:

        //Methods used by solveReducedDarcy().
        void initializeDiagonalElements();
        void fillDiscretizationMatrixElements( );

		void conjugateGradient();
        void freeMemory();

        //Methods used by solveFullDarcy().


		void biConjugateGradientStabilized();
        void readMatlab();

        //Methods for convective computation
        double sign(double);

};//end class Grid


#endif
