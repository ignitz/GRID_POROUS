//Cell.cpp
//Cell class implementation. 

#include "Cell.h"

#include <iostream>
    using std::cout;
    using std::endl;

//constructor
Cell::Cell():
    type('b'),
    active(false),
    level(0),
    bunchNumber(0),
    north(0),
    south(0),
    east(0),
    west(0),
    previous(0),
    next(0),
    singleConnector(0),
    doubleConnector1(0),
    doubleConnector2(0),
    peanoNumber(0),
    peanoCoordinate(0),
    fatherPeanoCoordinate(0),
    centerX(0.0),
    centerY(0.0),
    faceLength(1.0),
    halfFaceLength(0.5),    
    u(0.0),
    totalFlux(0.0),
    c(0.0),
    cNew(0.0),
    pL(0.0),
	pR(0.0),
    pU(0.0),
    pD(0.0),
    vL(0.0),
    vR(0.0),
    vU(0.0),
    vD(0.0),
   	K(1.0),		  //Permeability
   	S(1.0),
    w(0.0),		  //Well rate in the cell
    pressure(0.0),
    b(0.0),
	b_(0.0),
	bThird(0.0),
	bFourth(0.0),
	bFifth(0.0),
    Ap(0.0),
    Ap_(0.0),
    ApThird(0.0),
	ApFourth(0.0),
	ApFifth(0.0),
    r(0.0),
    r_(0.0),
    rThird(0.0),
	rFourth(0.0),
	rFifth(0.0),
    p(0.0),
    p_(0.0),
    pThird(0.0),
	pFourth(0.0),
	pFifth(0.0),
    p1(0.0),
    p1_(0.0),
    p1Third(0.0),
	p1Fourth(0.0),
	p1Fifth(0.0),
    rightBoundary(false),
   	leftBoundary (false),
   	lowerBoundary(false),
   	upperBoundary (false),
   	refined(false),
   	northFlux(0.0),
    southFlux(0.0),
    eastFlux(0.0),
    westFlux(0.0)
    
{
    firstElement = new Element;			//All the five lines of the full system
    secondElement = new Element;
    thirdElement = new Element;
    fourthElement = new Element;
    fifthElement = new Element;
    //sixthElement = new Element;
    //seventhElement = new Element;
    
}

//destructor
Cell::~Cell()
{}

//Prints cell information.
void Cell::print()
{
    cout << "Cell " << peanoNumber << "\n\n";
    
    cout << "Type = " << type << "\n";
    cout << "Active = " << active << "\n";
    cout << "Level = " << level << "\n";
    cout << "bunchNumber = " << bunchNumber << "\n";
    if( east != 0 )
    {
        cout << "   East neighbor = " << east->peanoNumber << " has type " 
             << east->type  << " and has level " << east->level << "\n";
        if( east->type == 'w' )
        {
            if( east->singleConnector != 0 )
            cout << "\tsingleConnector = " << east->singleConnector->peanoNumber
                 << " has type " << east->singleConnector->type << "\n";
            if( east->doubleConnector1 != 0 )
            cout << "\tdoubleConnector1 = " << east->doubleConnector1->peanoNumber
                 << " has type " << east->doubleConnector1->type << "\n";
            if( east->doubleConnector2 != 0 )
            cout << "\tdoubleConnector2 = " << east->doubleConnector2->peanoNumber
                 << " has type " << east->doubleConnector2->type << "\n";
        }
    }    
    if( north != 0 )
    {
        cout << "   North neighbor = " << north->peanoNumber << " has type " 
             << north->type  << " and has level " << north->level << "\n";
        if( north->type == 'w' )
        {
            if( north->singleConnector != 0 )
            cout << "\tsingleConnector = " << north->singleConnector->peanoNumber
                 << " has type " << north->singleConnector->type << "\n";
            if( north->doubleConnector1 != 0 )
            cout << "\tdoubleConnector1 = " << north->doubleConnector1->peanoNumber
                 << " has type " << north->doubleConnector1->type << "\n";
            if( north->doubleConnector2 != 0 )
            cout << "\tdoubleConnector2 = " << north->doubleConnector2->peanoNumber
                 << " has type " << north->doubleConnector2->type << "\n";
        }
    }    
    if( west != 0 )
    {
        cout << "   West neighbor = " << west->peanoNumber << " has type " 
             << west->type  << " and has level " << west->level << "\n";
        if( west->type == 'w' )
        {
            if( west->singleConnector != 0 )
            cout << "\tsingleConnector =" << west->singleConnector->peanoNumber
                 << " has type " << west->singleConnector->type << "\n";
            if( west->doubleConnector1 != 0 )
            cout << "\tdoubleConnector1 = " << west->doubleConnector1->peanoNumber
                 << " has type " << west->doubleConnector1->type << "\n";
            if( west->doubleConnector2 != 0 )
            cout << "\tdoubleConnector2 = " << west->doubleConnector2->peanoNumber
                 << " has type " << west->doubleConnector2->type << "\n";
        }
    }    
    if( south != 0 )
    {
        cout << "   South neighbor = " << south->peanoNumber << " has type " 
             << south->type  << " and has level " << south->level << "\n";
        if( south->type == 'w' )
        {
            if( south->singleConnector != 0 )
            cout << "\tsingleConnector = " << south->singleConnector->peanoNumber
                 << " has type " << south->singleConnector->type << "\n";
            if( south->doubleConnector1 != 0 )
            cout << "\tdoubleConnector1 = " << south->doubleConnector1->peanoNumber
                 << " has type " << south->doubleConnector1->type << "\n";
            if( south->doubleConnector2 != 0 )
            cout << "\tdoubleConnector2 = " << south->doubleConnector2->peanoNumber;
            cout << "\n";
        }
    }    
    if( previous != 0 )
        cout << "Previous cell = " << previous->peanoNumber << "\n";
    if( next != 0 )
        cout << "Next cell = " << next->peanoNumber << "\n";
    cout << "Center = ( " << centerX << ", " << centerY << " )\n";
    cout << "Face length = " << faceLength << "\n";
    cout << "Half face length " << halfFaceLength << "\n";
    
    cout << "Total Flux = " << totalFlux << "\n";
    cout << "u = " << u << "\n";
    cout << 
    "________________________________________________________________________\n" 
    << endl;
}
//end program Cell.cpp
