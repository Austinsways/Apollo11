//
//  Collisions.hpp
//  Lab03
//
//  Created by Conner Haar on 5/20/22.
//

#ifndef Collisions_hpp
#define Collisions_hpp
#include "point.h"
#include "ground.h"
#include "physics.h"
#include <stdio.h>
class Collisions
{
private:
    double maxVelocity = 4.0;
    int landerLength = 20;
public:
    bool isCrash(const Point pt, Physics LM, const Ground ground) {
        if (ground.hitGround(pt, landerLength)) {
            if (ground.onPlatform(pt, landerLength)) {
                if (LM.getSpeed() < maxVelocity) {
                    return false;
                }
                else { return true; }
            }
            else { return true; }
        };
        return false;
    };
};
#endif /* Collisions_hpp */

