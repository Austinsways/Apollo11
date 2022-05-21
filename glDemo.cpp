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
        ptLM(ptUpperRight.getX() / 2.0, ptUpperRight.getY() / 1.5),
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

    if (pDemo->collision.isCrash(pDemo->ptLM, pDemo->ptPhysics, pDemo->ground)) {
        gout.setPosition(Point(200, 200));
        gout << "you crashed";
    }
    else if (pDemo->ground.onPlatform(pDemo->ptLM, 20)) {
        if ((pDemo)->ptPhysics.getSpeed() < 4.0) {
            gout.setPosition(Point(200, 200));
            gout << "You won!";
        }
    }
    
    else {

        // move the ship around
        if (pUI->isRight()) {
            pDemo->angle -= 0.1;
            pDemo->ptPhysics.setAngle(pDemo->angle + 10);  // Shifts angle of thrust
        }
        if (pUI->isLeft()) {
            pDemo->angle += 0.1;
            pDemo->ptPhysics.setAngle(pDemo->angle - 10);  // Shifts angle of thrust
        }
        if (pUI->isDown()) {
            pDemo->ptPhysics.computeMovement(pDemo->ptLM.getX(), pDemo->ptLM.getY()); // calculates position, velocity and
            pDemo->ptLM.setY(pDemo->ptPhysics.getY()); // Gets y from the predicted measurements and sets value.
            pDemo->ptLM.setX(pDemo->ptPhysics.getX()); // Gets x from the predicted measurements and sets value to LM.
        }
        else {
            pDemo->ptPhysics.constantFall(pDemo->ptLM.getX(), pDemo->ptLM.getY()); // if not pressing anything, gravity pulls down.
            pDemo->ptLM.setY(pDemo->ptPhysics.getY());         // sets y
            pDemo->ptLM.setX(pDemo->ptPhysics.getX());
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
        gout << "Fuel: ";   // value of current fuel level goes here

        gout.setPosition(Point(20, 350));
        gout << "Altitude: " << (int)pDemo->ground.getElevation(pDemo->ptLM) << " meters" << "\n";

        gout.setPosition(Point(20, 325));
        gout.precision(3);
        gout << "Speed: " << (double)pDemo->ptPhysics.getSpeed() << " m/s ";  // value of current velocity goes here

        // draw our little star
        gout.drawStar(pDemo->ptStar, pDemo->phase++);




        if (pDemo->placed == false)
        {
            for (int i = 0; i < 50; i++)
            {
                pDemo->placedStars.resize(50);
                int groundHorizontalCord = random(0, 400);
                Point groundHorizontalPoint = Point(groundHorizontalCord, 0.0);
                int starAttemptedPlacement = 0;

                int groundVerticalCord = pDemo->ground.getElevation(groundHorizontalPoint) * -1;
                Point starPosition = Point(groundHorizontalCord, random(groundVerticalCord, 400));

                gout.drawStar(starPosition);

                pDemo->placedStars[i] = starPosition;


            }
            pDemo->placed = true;
        }
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



