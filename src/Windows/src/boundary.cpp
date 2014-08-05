//boundary.cpp
//Estabelece as condições de fronteira.

#include <cmath>
#include <stdlib.h>

//Use esta função se a condição de fronteira é descrita por uma única expressão.
double boundaryCondition( double x, double y )
{
    return x;
    // return x*x-y*y;
}

//Use as quatro funções a seguir se a condição de fronteira é descrita por
//diferentes expressões em cada lado da malha quadrada.


double eastBoundaryCondition( double y )
{
    return 11;
    // return 1;
    // return -4*(y-0.25)*(y-0.75);
    //return -10*(y-0.25)*(y-0.75);//x*x-y*y;
}

double northBoundaryCondition( double x )
{
    return 0;
    // return 2;
    // return -4*(x-0.25)*(x-0.75);
    // return -10*(x-0.25)*(x-0.75); //1000-880*x;
}

double westBoundaryCondition( double y )
{
	double x = 0.0; // ????
	return 22;
    // return 2;
    // return -4*(y-0.25)*(y-0.75);
    // return -10*(y-0.25)*(y-0.75);
}
double southBoundaryCondition( double x )
{
	double y = 0.0;
    return 0;
    // return 1;
    // return -4*(x-0.25)*(x-0.75);
    // return -10*(x-0.25)*(x-0.75);//1000-880*x;
}


double velocityNorthBoundaryCondition(double x)
{
	return 0;
}

double velocitySouthBoundaryCondition(double x)
{
	return 0;
}

double velocityWestBoundaryCondition(double y)
{
	return 0;
}

double velocityEastBoundaryCondition(double y)
{
	return 0;
}
