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
#include "utils.h"

#include "testing.h"

using namespace CGL;

int main(int argc, const char * argv[]) {
    //testing::seed_tri_test_simple();
//    testing::search_neighbor_test();
//    testing::seed_tri_test();
//    testing::seed_tri_test();
//    testing::mesher_test();n
    //demo_read();
    
    string name = "Armadillo.xyz";
    double radius = 3;
    MeshConvert m = MeshConvert(name, radius);
    m.construct();
//    m.sharp();
    std::cout<< m.count_i;
    std::cout<< "\n";
    std::cout<< m.count_j;
    std::cout<< "\n";
    m.write_to_file();
}
