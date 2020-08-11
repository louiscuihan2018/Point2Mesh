//
//  main.cpp
//  test
//
//  Created by louis on 8/4/20.
//  Copyright © 2020 louis. All rights reserved.
//
#include <iostream>
#include "vertex.h"
#include "edge.h"
#include "sphere.h"
#include "triangle.h"
//#include "utils.h"

#include "testing.h"

using namespace CGL;

int main(int argc, const char * argv[]) {
    //testing::seed_tri_test_simple();
//    testing::search_neighbor_test();
//    testing::seed_tri_test();
//    testing::seed_tri_test();
//    testing::mesher_test();
    //demo_read();
    
    string name = "tex.xyz";
    double radius = 1.0;
    MeshConvert m = MeshConvert(name, radius);
    m.construct();
}
