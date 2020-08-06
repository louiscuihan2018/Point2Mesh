//
//  utils.cpp
//  CGL
//
//  Created by louis on 8/5/20.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include "utils.h"
#include "vertex.h"
#include "edge.h"
#include "sphere.h"
#include "triangle.h"
using namespace CGL;

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c) {
    Vertex v1 = *a;
    Vertex v2 = *b;
    Vertex v3 = *c;
    
    bool check = v1.compatible(v2, v3);
    if (check) {
        Triangle* t = new Triangle(a, b, c);
        return std::make_pair(t, true);
    } else {
        Triangle* t = NULL;
        return std::make_pair(t, true);
    }
}

vector<Vector3D> read_and_range() {
    // name of file change here
    // need to be in the target or working folder 
    ifstream file("tex.xyz");
    int v_number;
    file >> v_number;
    std::cout<< v_number;
    std::cout<< "\n";
    std::cout<< "\n";
    vector<Vertex*> vertices;
    vector<Vector3D> range;
    double minx = HUGE_VAL;
    double miny = HUGE_VAL;
    double minz = HUGE_VAL;
    double maxx = -HUGE_VAL;
    double maxy = -HUGE_VAL;
    double maxz = -HUGE_VAL;
    for (int i = 0; i < v_number; i ++) {
        double x;
        double y;
        double z;
        double dx;
        double dy;
        double dz;
        file >> x;
        file >> y;
        file >> z;
        file >> dx;
        file >> dy;
        file >> dz;
        
        if (x < minx) {
            minx = x;
        }
        if (x > maxx) {
            maxx = x;
        }
        if (y < miny) {
            miny = y;
        }
        if (y > maxy) {
            maxy = y;
        }
        if (z < minz) {
            minz = z;
        }
        if (z > maxz) {
            maxz = z;
        }
        
        Vector3D curr_point = Vector3D(x, y, z);
        Vector3D curr_normal = Vector3D(dx, dy, dz);
        Vertex *curr = new Vertex(curr_point, curr_normal);
        vertices.push_back(curr);
//        std::cout<< curr_normal.x;
//        std::cout<< "\n";
    }
    Vector3D min = Vector3D(minx, miny, minz);
    Vector3D max = Vector3D(maxx, maxy, maxz);
    range.push_back(min);
    range.push_back(max);
    return range;
}


