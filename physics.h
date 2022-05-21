#pragma once
//
//  physics.hpp
//  Lab03
//
//  Created by Conner Haar and Austin Eldredge on 5/13/22.
//


/*
Thruster class for applying angles to the LM, this is currently just stubbed out til physics is fully implemented.
*/
class Thruster {
private:
    double forceToApply;
    double maxFuelBurn;
    double maxForce;
    double fuelBurn;
    double direction;
    double lockInputDirection;
    void burnFuel();
public:
    bool ThrusterOn = false;
    double getThrusterForce();
    double getDirection();
    double getFuelBurned();
    void turnOnThruster();
    void turnOffThruster();

};



#ifndef physics_hpp
#define physics_hpp

#include "point.h"
#include <stdio.h>
#include <math.h> // for math reasons
#include <corecrt_math_defines.h>
class Physics {
private:
    Point lm;
    double GRAVITY = -1.625;
    double WEIGHT = 1503.00;
    double THRUST = 9000;
    double time = 0.1;
    double Angle;
    double radians;
    double xPosition;
    double yPosition;
    double xVelocity = 0;
    double yVelocity = 0;
    double totalVelocity;
    double xThrust;
    double yThrust;
    double totalThrust = computeAcceleration(THRUST, WEIGHT);
    double computeDistance(double s, double v, double a, double t) {
        // Instantiate a variable to hold the new value.
        double newPosition;
        // Apply the equation to get the new position.
        newPosition = s + v * t + .5 * a * (pow(t, 2));
        return newPosition;
    }
    double computeAcceleration(double f, double m) {
        //instantiate a variable to hold the accelleration in meters/second^2.
        double a;
        //compute the new accelleration.
        a = f / m;
        return a;
    }
    double computeVelocity(double v, double a, double t) {
        // Instantiate a new variable to hold the new velocity.
        double newVelocity;
        // Calculate new velocity.
        newVelocity = v + a * t;
        return newVelocity;
    }
    double computeVerticalComponent(double a, double total) {
        // Create a variable to hold the vertical componenent of the total.
        double newTotal;
        // Use the equation above to create the new total.
        newTotal = cos(a) * total;
        return newTotal;
    }
    double computeHorizontalComponent(double a, double total) {
        // Make a variable to hold the vertical component of the total.
        double newTotal;
        // Compute the new total.
        newTotal = sin(a) * total;
        return newTotal;
    }
    double computeTotalComponent(double x, double y) {
        // Create a new variable to hold the total.
        double total;
        // Calculate the total.
        total = pow(x, 2) + pow(y, 2);
        total = sqrt(total);
        //return the total
        return total;
    }
    double convertToRadians(double d) {
        // Create variable to hold radians.
        double radians;
        // Calculate the radians.
        radians = 2 * M_PI * (d / 360);
        return radians;
    }
public:

    void computeMovement(double x, double y) {
        radians = convertToRadians(Angle);
        xThrust = computeHorizontalComponent(radians, totalThrust);
        yThrust = computeVerticalComponent(radians, totalThrust) + GRAVITY;

        xVelocity = computeVelocity(xVelocity, xThrust, time);
        yVelocity = computeVelocity(yVelocity, yThrust, time);

        totalVelocity = computeTotalComponent(xVelocity, yVelocity);

        xPosition = computeDistance(x, xVelocity, xThrust, time);
        yPosition = computeDistance(y, yVelocity, yThrust, time);
    }
    void constantFall(double x, double y) {
        yThrust = GRAVITY;
        xVelocity = computeVelocity(xVelocity, 0, time);
        yVelocity = computeVelocity(yVelocity, yThrust, time);
        xPosition = computeDistance(x, xVelocity, xThrust, time);
        yPosition = computeDistance(y, yVelocity, yThrust, time);
        totalVelocity = computeTotalComponent(xVelocity, yVelocity);
    }
    double getSpeed() {
        return totalVelocity;
    }
    double getX() {
        return xPosition;
    }
    double getY() {
        return yPosition;
    }
    void setAngle(double ang) {
        Angle = ang;
    }
};
#endif /* physics_hpp */
