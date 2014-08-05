/* Element.h
Definition of class Element.
Element is a linked list of entries that will play the role of a row in the grid
discretization matrix associated to this cell.
For performance reasons (i.e., in order to avoid multiple function calls),
all members are declared public.
*/

#ifndef ELEMENT_H
#define ELEMENT_H

#include "Cell.h"

class Cell;

class Element
{
    public:
        
        Element();			//Default constructor.
        ~Element();			//Destructor.

        double value;       //Coefficient for the pressure or velocity on the right or lower side of the cell
		bool right;         //Determines if the value is multiplied by the pressure on the right side (true) or on the left side (false)
		bool horizontal;    //If false, left becomes up and right becomes down
		bool vel;			//Determines if the value is multiplied by the velocity (true) or by the pressure on the cell side(false)
        bool pressure;		//Determines if the value is multiplied by the pressure inside the cell
		int column;			//Column of the matrix to which this element belongs.
        Element *next;      //Next element in the list.
        Cell *cell;         //Cell to which this element's row is associated.
        
};//end class Element

#endif
