//flux.cpp

#include "Cell.h"
#include "Grid.h"
#include "functions.h"
#include <iostream>
	using namespace std;
#include <cmath>
#include <stdlib.h>

/*______________________________________________________________________________

								FUNCTION setGridFlux()
								
    Traverses the grid, computing the fluxes in all directions (north, south, 
    east, west) due to all neighboring cells in order to compute the total flux.
    In order to do this, calls function setCellFlux().	
	
______________________________________________________________________________*/
void setGridFlux( Grid *grid )
{
    Cell *gridCell,
         *neighborGridCell;
    char direction;  
    
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {
        gridCell->totalFlux = 0.0;
		gridCell->northFlux = 0.0;
        gridCell->southFlux = 0.0;
        gridCell->eastFlux = 0.0;
        gridCell->westFlux = 0.0;
        
        gridCell = gridCell->next;
    }        
        
    gridCell = grid->firstGridCell;
    while ( gridCell != 0 )
    {
       //Computes southFlux.
       neighborGridCell = gridCell->south;
       direction = 's';
       setCellFlux( gridCell, neighborGridCell, direction, grid->currentDeltaT);
       
       //Computes northFlux.
       neighborGridCell = gridCell->north;
       direction = 'n';
       setCellFlux( gridCell, neighborGridCell, direction, grid->currentDeltaT);              
       
       //Computes eastFlux.
       neighborGridCell = gridCell->east;
       direction = 'e';
       setCellFlux( gridCell, neighborGridCell, direction, grid->currentDeltaT);
       
       //Computes westFlux.
       neighborGridCell = gridCell->west;
       direction = 'w';
       setCellFlux( gridCell, neighborGridCell, direction, grid->currentDeltaT);
                                             
        gridCell = gridCell->next;
    }
    
    
}


/*______________________________________________________________________________
    
                            FUNCTION setCellFlux()
    
    Computes the flux into this cell due to a neighbor cell.
	 
    flux = (u(thisCell) - u(neighborCell)/2* leastDistance,
    
    where 2* leastDistance is two times the least of the lengths of the faces of
	this cell and its neighbor.
    
______________________________________________________________________________*/
void setCellFlux( Cell *gridCell, Cell *neighborGridCell, char direction, double deltaT )
{
    int levelDifference, //difference between levels of gridCell and neighborGridCell
        twoToLevelDifference = 1; //2*levelDifference
    
	double leastDistance = gridCell->halfFaceLength,
       	   localFlux;
  	
  	
  	
  	//When neighborGridCell is a transition node, looks for the next neighbor 
    //cell which is a cell node.
  	if ( neighborGridCell->level > gridCell->level )
    	while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
        	neighborGridCell = neighborGridCell->doubleConnector2;
    	}   	
  	else
    	while( ( neighborGridCell->type == 'w' ) && 
               ( neighborGridCell->singleConnector != 0 ) 
             )
    	{
    	    neighborGridCell = neighborGridCell->singleConnector;
   		}

    //It enters this "if" only if neighborGridCell is connected to null, i.e.,
    //only if gridCell belongs to the boundary of the domain. 
    if( neighborGridCell->type == 'w' )
    {    
        
        if( direction == 's')      
        {
			if ( gridCell->VD >=0 )
	        	gridCell->southFlux -= gridCell->c * gridCell->VD;
	        else
	        	gridCell->southFlux = 0;
        }
        else if( direction == 'n' ) 
        {
            if ( gridCell->VU <=0 )
            {
	        	gridCell->northFlux += gridCell->c * gridCell->VU;
			}
	        else
	        {
				gridCell->northFlux = 0;
			}
        }
        else if( direction == 'e' ) 
        {
            if ( gridCell->VR >=0 )
	        	gridCell->eastFlux -= gridCell->c * gridCell->VR;
	        else
	        	gridCell->eastFlux = 0;
        }
        else if( direction == 'w' ) 
        {
            if ( gridCell->VL <=0 )
	        	gridCell->northFlux += gridCell->c * gridCell->VL;
	        else
	        	gridCell->northFlux = 0;          
        }
    }
    //Enters here almost always, except in the case indicated above.  
    else if( neighborGridCell->type == 'b' )
    {
        
		//Dictionary order avoids computing the flux twice for the same cell.
        if( (gridCell->level > neighborGridCell->level) ||
    		  ( (gridCell->level == neighborGridCell->level) &&
            	( (gridCell->centerX < neighborGridCell->centerX) ||
           		  ( ( gridCell->centerX == neighborGridCell->centerX ) && 
                              ( gridCell->centerY > neighborGridCell->centerY ) )
                )
              )
            ) 
			
		{                    
			double alpha = 0.1*gridCell->faceLength*gridCell->faceLength;
			double beta = 1e-9;
			double normV = 0;
			
			if(fabs(gridCell->c - neighborGridCell->c) > gridCell->totalFlux)
					gridCell->totalFlux = fabs(gridCell->c-neighborGridCell->c);
			if(fabs(gridCell->c - neighborGridCell->c) > neighborGridCell->totalFlux)		
					neighborGridCell->totalFlux = fabs(gridCell->c-neighborGridCell->c);
			
			
			
			if( direction == 's')      
	        {
				if ( gridCell->VD >=0 ){
	                 gridCell->southFlux -= gridCell->c * gridCell->VD;
	                 neighborGridCell->northFlux += gridCell->c * gridCell->VD;
					 }
	            else{
					//if(gridCell->peanoNumber == 93)
				//		cout << gridCell->c << " " << gridCell->vU << " " << gridCell->c * gridCell->VU/(0.27* gridCell->faceLength* gridCell->faceLength) << endl;
					 gridCell->southFlux -= neighborGridCell->c * gridCell->VD;
	                 neighborGridCell->northFlux += neighborGridCell->c * gridCell->VD;
					 }
					
				normV = gridCell->vD * gridCell->vD	+ (gridCell->vR + gridCell->vL)*(gridCell->vR + gridCell->vL)/4;
				normV = sqrt(normV);
				if(normV != 0)
				{
					gridCell->southFlux += (0.9*alpha*fabs(gridCell->VD)*fabs(gridCell->VD)/normV + 0.1*alpha*normV*deltaT +beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (0.9*alpha*fabs(gridCell->VD)*fabs(gridCell->VD)/normV + 0.1*alpha*normV*deltaT+beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
				else
				{
					gridCell->southFlux += (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
				
			}
	        else if( direction == 'n' ) 
	        {
	            if ( gridCell->VU >=0 ){
	                gridCell->northFlux += neighborGridCell->c * gridCell->VU;
	                neighborGridCell->southFlux -= neighborGridCell->c * gridCell->VU;}
	            else {
	                gridCell->northFlux += gridCell->c * gridCell->VU;
	                neighborGridCell->southFlux -= gridCell->c * gridCell->VU;}
	            
				normV = gridCell->vU * gridCell->vU	+ (gridCell->vR + gridCell->vL)*(gridCell->vR + gridCell->vL)/4;
				normV = sqrt(normV);
				if(normV != 0)
				{
					gridCell->southFlux += (0.9*alpha*fabs(gridCell->VU)*fabs(gridCell->VU)/normV + 0.1*alpha*normV*deltaT +beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (0.9*alpha*fabs(gridCell->VU)*fabs(gridCell->VU)/normV + 0.1*alpha*normV*deltaT+beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
				else
				{
					gridCell->southFlux += (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
	        }
	        else if( direction == 'e' ) 
	        {
	            if ( gridCell->VR >=0 ){
	                 gridCell->eastFlux -= gridCell->c * gridCell->VR;
	                 neighborGridCell->westFlux += gridCell->c * gridCell->VR;}
	            else{
	                 gridCell->eastFlux -= neighborGridCell->c * gridCell->VR;
	                 neighborGridCell->westFlux += neighborGridCell->c * gridCell->VR;}
	            normV = gridCell->vR * gridCell->vR	+ (gridCell->vU + gridCell->vD)*(gridCell->vU + gridCell->vD)/4;
				normV = sqrt(normV);
				if(normV != 0)
				{
					gridCell->southFlux += (0.9*alpha*fabs(gridCell->VR)*fabs(gridCell->VR)/normV + 0.1*alpha*normV*deltaT +beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (0.9*alpha*fabs(gridCell->VR)*fabs(gridCell->VR)/normV + 0.1*alpha*normV*deltaT+beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
				else
				{
					gridCell->southFlux += (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
	        }
	        else if( direction == 'w' ) 
	        {
	            if ( gridCell->VL >=0 ){
	                gridCell->westFlux += neighborGridCell->c * gridCell->VL;
	                neighborGridCell->eastFlux -= neighborGridCell->c * gridCell->VL;}
	            else{
	                gridCell->westFlux += gridCell->c * gridCell->VL;
	                neighborGridCell->eastFlux -= gridCell->c * gridCell->VL;}
				
				normV = gridCell->vL * gridCell->vL	+ (gridCell->vU + gridCell->vD)*(gridCell->vU + gridCell->vD)/4;
				normV = sqrt(normV);
				if(normV != 0)
				{
					gridCell->southFlux += (0.9*alpha*fabs(gridCell->VL)*fabs(gridCell->VL)/normV + 0.1*alpha*normV*deltaT +beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (0.9*alpha*fabs(gridCell->VL)*fabs(gridCell->VL)/normV + 0.1*alpha*normV*deltaT+beta*deltaT)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}
				else
				{
					gridCell->southFlux += (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
					neighborGridCell->southFlux -= (beta)*(neighborGridCell->c - gridCell->c)*gridCell->faceLength/(gridCell->halfFaceLength + neighborGridCell->halfFaceLength );
				}          
	        }
		}
        
    }
    

}

