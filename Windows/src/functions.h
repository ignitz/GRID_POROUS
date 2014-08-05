//functions.h


#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Cell.h"
#include "Grid.h"

/* _____________________________________________________________________________
								graphics.cpp
______________________________________________________________________________*/

    void setColor(int);
	void drawPoint(double, double);
	void drawQuad(double ,double , double , double );
    void drawLine(double,double,double,double);
    void drawLineScale ( double , double , double , double  );
    void drawString(const char* , void* , double , double ) ;
    void drawCenterString(const char* , void* , double);
    void setBlueColor(double);

/* _____________________________________________________________________________
								refinement.cpp
______________________________________________________________________________*/

	int computeHilbertShapeNumber( int, int );
	void simplify( Cell * );
	void refineCell( Cell *, bool);
	void redrawPeanoCurve( Cell *, int, bool );
	void solveFullDarcyRefining(Cell *, Grid *);
	void setBoundariesRefining(Grid *, Cell *);
	void initializeDiagonalElementsRefining(Grid *, Cell *);
	void fillDiscretizationMatrixElementsRefining(Grid *,  Cell *);
	void biConjugateGradientStabilizedRefining(Grid *);
	void calculateFullDataRefining(Grid *, Cell *);
	void setCellFlux( Cell *, Cell *, char, double );
	void setGridFlux( Grid * );
	void coarseData(Grid *, Grid *);

/* _____________________________________________________________________________
								derefinement.cpp
______________________________________________________________________________*/

    void derefineGrid( Grid *, double, bool );
    void derefineBunch( Cell *, bool );
    bool needsDerefinement( Cell *, double );

/* _____________________________________________________________________________
    							   boundary.cpp
______________________________________________________________________________*/

	double boundaryCondition( double, double );

	double eastBoundaryCondition( double );
	double northBoundaryCondition( double );
	double westBoundaryCondition( double );
	double southBoundaryCondition( double );

	double velocityNorthBoundaryCondition( double );
	double velocitySouthBoundaryCondition( double );
	double velocityWestBoundaryCondition( double );
	double velocityEastBoundaryCondition( double );

#endif
