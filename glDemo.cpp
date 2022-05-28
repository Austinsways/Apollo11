/*
Made By Conner Haar and Austin Eldredge
*/




/**********************************************************************
 * GL Demo
 * Just a simple program to demonstrate how to create an Open GL window,
 * draw something on the window, and accept simple user input
 **********************************************************************/

#include "point.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "ground.h"
#include "physics.h"
#include "Collisions.h"
#include <vector>
using namespace std;

/*************************************************************************
 * Demo
 * Test structure to capture the LM that will move around the screen
 *************************************************************************/
class Demo
{
public:

    Demo(const Point& ptUpperRight) :
        angle(0.0),
        ptStar(ptUpperRight.getX() - 20.0, ptUpperRight.getY() - 20.0),
        ptLM(ptUpperRight.getX() / 2.0, ptUpperRight.getY() ),
        ground(ptUpperRight)

    {

        bool placed = false;
        phase = random(0, 255);
    }

    // this is just for test purposes.  Don't make member variables public!
    Point ptLM;           // location of the LM on the screen
    Point ptUpperRight;   // size of the screen
    double angle;         // angle the LM is pointing
    unsigned char phase;  // phase of the star's blinking
    Ground ground;
    Point ptStar;
    Physics ptPhysics;   // physics adjustments
    vector<Point> placedStars;
    bool placed;
    Collisions collision;
    Fuel fuel;

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
    Demo* pDemo = (Demo*)p;

    //check if the lander crashed
    if (pDemo->collision.isCrash(pDemo->ptLM, pDemo->ptPhysics, pDemo->ground)) {
        gout.setPosition(Point(170, 200));
        gout << "you crashed";
    }
    //check if the lander hit the platform
    else if (pDemo->ground.onPlatform(pDemo->ptLM, 20)) {
        //check if it hit soft enough to win
        if ((pDemo)->ptPhysics.getSpeed() < 4.0) {
            gout.setPosition(Point(170, 200));
            gout << "You won!";
        }
        //if it hit too hard let the user know
        else {
            gout.setPosition(Point(170, 200));
            gout << "You hit too hard!";
        }
    }
    
    else {

        //check if the ship has fuel to move
        if (pDemo->fuel.fuelLeft != 0) {
            // if the ship has fuel move the ship around
            if (pUI->isRight()) {
                pDemo->angle -= 0.1;
                pDemo->ptPhysics.setAngle(pDemo->angle + 10);  // Shifts angle of thrust
                pDemo->fuel.changeFuel(-1);
            }
            if (pUI->isLeft()) {
                pDemo->angle += 0.1;
                pDemo->ptPhysics.setAngle(pDemo->angle - 10);  // Shifts angle of thrust
                pDemo->fuel.changeFuel(-1);
            }
            if (pUI->isDown()) {
                pDemo->ptPhysics.computeMovement(pDemo->ptLM.getX(), pDemo->ptLM.getY()); // calculates position, velocity and
                pDemo->ptLM.setY(pDemo->ptPhysics.getY()); // Gets y from the predicted measurements and sets value.
                pDemo->ptLM.setX(pDemo->ptPhysics.getX()); // Gets x from the predicted measurements and sets value to LM.
                pDemo->fuel.changeFuel(-10);
            }
            else {
                pDemo->ptPhysics.constantFall(pDemo->ptLM.getX(), pDemo->ptLM.getY()); // if not pressing anything, gravity pulls down.
                pDemo->ptLM.setY(pDemo->ptPhysics.getY());         // sets y
                pDemo->ptLM.setX(pDemo->ptPhysics.getX());
            }
        }
        else {
            //apply a ocnstant fall if the user is out of fuel
            pDemo->ptPhysics.constantFall(pDemo->ptLM.getX(), pDemo->ptLM.getY()); // if not pressing anything, gravity pulls down.
            pDemo->ptLM.setY(pDemo->ptPhysics.getY());         // sets y
            pDemo->ptLM.setX(pDemo->ptPhysics.getX());
            //display the our of fuel indicator
            gout.setPosition(Point(170, 200));
            gout << "Out of Fuel!";
        }
        
    }

        // draw the ground
        pDemo->ground.draw(gout);

        // draw the lander and its flames
        gout.drawLander(pDemo->ptLM /*position*/, pDemo->angle /*angle*/);
        gout.drawLanderFlames(pDemo->ptLM, pDemo->angle, /*angle*/
            pUI->isDown(), pUI->isLeft(), pUI->isRight());

        // put some text on the screen
        gout.setPosition(Point(20, 375));
        gout << "Fuel: " << pDemo->fuel.fuelLeft;   // value of current fuel level goes here

        gout.setPosition(Point(20, 350));
        gout << "Altitude: " << (int)pDemo->ground.getElevation(pDemo->ptLM) << " meters" << "\n";

        gout.setPosition(Point(20, 325));
        gout.precision(3);
        gout << "Speed: " << (double)pDemo->ptPhysics.getSpeed() << " m/s ";  // value of current velocity goes here

        // draw our little star
        gout.drawStar(pDemo->ptStar, pDemo->phase++);



        //this algoirthm creates the stars in the background. 
        //the while loop ensures this only chooses the stars placements once.
        if (pDemo->placed == false)
        {
            //randomly place 50 stars above ground level.
            for (int i = 0; i < 50; i++)
            {
                pDemo->placedStars.resize(50);
                //gain vertical and horizontal cords the ground level
                int groundHorizontalCord = random(0, 400);
                Point groundHorizontalPoint = Point(groundHorizontalCord, 0.0);

                //convert the chosen star location to a point data type  
                int groundVerticalCord = pDemo->ground.getElevation(groundHorizontalPoint) * -1;
                Point starPosition = Point(groundHorizontalCord, random(groundVerticalCord, 400));

                //draw the star
                gout.drawStar(starPosition);

                //add the star to the star  list
                pDemo->placedStars[i] = starPosition;


            }
            pDemo->placed = true;
        }
        //this draws the stars after they have been placed.
        else {
            for (int i = 0; i < pDemo->placedStars.size() - 1; i++)
            {
                gout.drawStar(pDemo->placedStars[i]);
            }
        }
    
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
    // Initialize OpenGL
    Point ptUpperRight(400.0, 400.0);
    Interface ui(0, NULL,
        "Open GL Demo",
        ptUpperRight);

    // Initialize the game class
    Demo demo(ptUpperRight);

    // set everything into action
    ui.run(callBack, &demo);

    return 0;
}



