//
//  sphere.cpp
//  test
//
//  Created by louis on 8/5/20.
//  Copyright © 2020 louis. All rights reserved.
//

#include <stdio.h>
#include "vertex.h"
#include "sphere.h"
#include <vector>
#include "CGL/vector3D.h"

using namespace std;

namespace CGL {
    
    Sphere::Sphere(Vector3D a, double r) {
        this->center = a;
        this->radius = r;
    }

}

