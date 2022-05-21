/**********************************************************************
 * GL Demo
 * Just a simple program to demonstrate how to create an Open GL window,
 * draw something on the window, and accept simple user input
 **********************************************************************/

#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "ground.h"
#include "star.h"
#include "lm.h"
#include <stdlib.h> 
#include <vector>
#include <cstdlib>
#include <Windows.h>

using namespace std;


vector<Star> createStars(int amount) {

    vector<Star> stars;
    int randX;
    int randY;

    for (int c = 0; c <= amount; c ++) {
        randX = rand() % 400 + 1;
        randY = rand() % 400 + 200;
        stars.push_back(Star(Point(randX, randY)));
    }
    
    return stars;
}


/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/
class Game
{
public:
    Game(const Point& ptUpperRight) :
        ground(ptUpperRight)
    {
    
    }

    LM ship;
    vector<Star> stars = createStars(50);


    //Point starPosition = Point(ptUpperRight.getX() - 20.0, ptUpperRight.getY() - 20.0);
    //Star star1 = Star(starPosition);

    // this is just for test purposes.  Don't make member variables public!
    Point ptUpperRight;   // size of the screen
    Ground ground;

};

/*************************************
 * All the interesting work happens here, when
 * I get called back from OpenGL to draw a frame.
 * When I am finished drawing, then the graphics
 * engine will wait until the proper amount of
 * time has passed and put the drawing on the screen.
 **************************************/
void callBack(const Interface* pUI, void* p)
{
    ogstream gout;

    // the first step is to cast the void pointer into a game object. This
    // is the first step of every single callback function in OpenGL. 
    Game* pGame = (Game*)p;

    // draw our little star
    for (int i = 0; i < pGame->stars.size(); i++) {
        gout.drawStar(pGame->stars[i].position, pGame->stars[i].getPhase());
    }

    // draw the ground
    pGame->ground.draw(gout);


    if (pGame->ship.getFuel() > 0) {

        // input
        if (pUI->isRight()) {
            pGame->ship.updateAngle(-0.1);
        }
        if (pUI->isLeft()) {
            pGame->ship.updateAngle(0.1);
        }

        if (pUI->isDown()) {
            // Break thrust into x and y components
            pGame->ship.updateYVelocity(.29 * cos(pGame->ship.getAngle()));
            pGame->ship.updateXVelocity(.29 * -sin(pGame->ship.getAngle()));
            pGame->ship.updateFuel();
        }
    }

    pGame->ship.updateYPosition();
    pGame->ship.updateXPosition();



    if (pGame->ship.getYVelocity() > -5 && pGame->ground.onPlatform(pGame->ship.position, 1)) {
        gout.setPosition(Point(150.0, 380.0));
        gout << "Successful Landing";
        
        cout << "Success" << endl;
        
        Sleep(5000);
        gout << "Successful Landing";
        Sleep(1000);
        exit(0);
        
    }
    else if (pGame->ground.hitGround(pGame->ship.position, 20)) {
        gout.setPosition(Point(150.0, 380.0));
        gout << "Crash Landing";
        
        cout << "Crashed" << endl;
        pGame->ship.updateYVelocity(-pGame->ship.getYVelocity());
        pGame->ship.updateXVelocity(-pGame->ship.getXVelocity());
        gout.setPosition(Point(150.0, 380.0));
        gout << "Crash Landing";
        Sleep(1000);
        gout << "Crash Landing";
        Sleep(3000);

        Point ptUpperRight(400.0, 400.0);
        Interface ui(0, NULL,
            "Open GL Window",
            ptUpperRight);

        // Initialize the game class
        Game game(ptUpperRight);
        ui.run(callBack, &game);
        }
    else {
        pGame->ship.updateYVelocity(-0.1625);
    }




    // draw the lander and its flames
    gout.drawLander(pGame->ship.position /*position*/, pGame->ship.getAngle() /*angle*/);
    gout.drawLanderFlames(pGame->ship.position, pGame->ship.getAngle(), /*angle*/
        pUI->isDown(), pUI->isLeft(), pUI->isRight());

    // put some text on the screen
    gout.setPosition(Point(10.0, 380.0));
    gout << "Fuel: " << pGame->ship.getFuel() << "\nAltitude: " << pGame->ship.getY() << " meters\nSpeed: " << abs (pGame->ship.getYVelocity()) << " m/s";

}

/*********************************
 * Main is pretty sparse.  Just initialize
 * my Demo type and call the display engine.
 * That is all!
 *********************************/
#ifdef _WIN32_X
#include <windows.h>
int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ PWSTR pCmdLine,
    _In_ int nCmdShow)
#else // !_WIN32
int main(int argc, char** argv)
#endif // !_WIN32
{   

    while (true) {
        // Initialize OpenGL
        Point ptUpperRight(400.0, 400.0);
        Interface ui(0, NULL,
            "Open GL Window",
            ptUpperRight);

        // Initialize the game class

        Game game(ptUpperRight);
        // set everything into action

        ui.run(callBack, &game);
    }
    

    return 0;
}
