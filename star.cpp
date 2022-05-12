/***********************************************************************
 * Header File:
 *    Star : Represents the stars in the background of the lunar lander game
 * Author:
 *    Braxton Meyer
 * Summary:
 *    Where the star is located and what phase of blinking it is in
 ************************************************************************/


#include "star.h"

 /******************************************
  * STAR : CONSTRUCTOR WITH POINT
  * Initialize the star at the passed position
  *****************************************/


Star::Star(Point newPosition)
{
	this->phase = 0;
	this->position = newPosition;
}

void Star::incrementPhase(int& phase)
{
	phase++;
}