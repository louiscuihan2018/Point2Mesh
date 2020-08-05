//
//  main.cpp
//  test
//
//  Created by louis on 8/4/20.
//  Copyright © 2020 louis. All rights reserved.
//

#include <iostream>
#include "CGL/vector3D.cpp"
//#include "vertex.h"
//#include "edge.h"
//#include "sphere.h"
#include "triangle.h"

int main(int argc, const char * argv[]) {
    // insert code here...
    CGL::Vector3D a = CGL::Vector3D(1,2,3);
    CGL::Vector3D b = CGL::Vector3D(4,5,6);
    CGL::Vector3D c = CGL::Vector3D(7,8,9);
    CGL::Vector3D d = CGL::Vector3D(0,0,0);
    CGL::Vertex x = CGL::Vertex(a, b);
    CGL::Vertex y = CGL::Vertex(c, d);
    CGL::Edge e1 = CGL::Edge(x,y);
    std::cout << e1.b.point.y;
    std::cout << "\n";
    CGL::Sphere s1 = CGL::Sphere(a, 2.0);
    std::cout << s1.center.z;
    std::cout << "\n";
    CGL::Vector3D p1 = CGL::Vector3D(2,0,0);
    CGL::Vector3D p2 = CGL::Vector3D(0,2,0);
    CGL::Vector3D p3 = CGL::Vector3D(0,0,2);
    CGL::Vector3D n = CGL::Vector3D(1,1,1);
    CGL::Vertex x1 = CGL::Vertex(p1, n);
    CGL::Vertex x2 = CGL::Vertex(p2, n);
    CGL::Vertex x3 = CGL::Vertex(p3, n);
    CGL::Triangle t1 = CGL::Triangle(x1, x2, x3);
    CGL::Sphere s2 = t1.construct_ball(2.0);
    std::cout << s2.center.x;
    std::cout << "\n";
    std::cout << s2.center.y;
    std::cout << "\n";
    std::cout << s2.center.z;
    std::cout << "\n";
    std::cout << x1.adjacent_triangles.size();
    std::cout << "\n";
    return 0;
}
