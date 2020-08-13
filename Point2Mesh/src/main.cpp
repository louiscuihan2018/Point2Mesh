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
    
    string name = "bun_zipper.xyz";
    double radius = 0.0014;
    Timer t;
    t.reset();
    MeshConvert m = MeshConvert(name, radius);
    t.stop();
    m.construct();
//    m.sharp();
    t.stop();
    m.postProcess();
    t.stop();
    std::cout<< m.count_i;
    std::cout<< "\n";
    std::cout<< m.count_j;
    std::cout<< "\n";
    m.write_to_file();
    list<double> timing = t.get_all_stops();
    m.data_output(name, timing);
}
