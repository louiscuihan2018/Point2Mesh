//
//  vertex.h
//  ColladaViewer
//
//  Created by louis on 8/4/20.
//

#ifndef vertex_h
#define vertex_h
#include <vector>
//#include "edge.h"
//#include "triangle.h"
#include "types.h"
#include "CGL/CGL.h"
#include "CGL/vector3D.h"

using namespace std;
using namespace CGL;

enum v_type { ORPHAN = 0, FRONT = 1, INNER = 2 };

class Vertex{
    public:
        Vector3D point;
        Vector3D normal;
        v_type type;
        Edge_list adjacent_edges;
        Tri_list adjacent_triangles;
        Vertex(){};
        Vertex(Vector3D a, Vector3D b);
        void add_edge(Edge *edge);
        void add_triangle(Triangle *triangle);
        bool compatible(Edge &edge);
        bool compatible(Vertex &v1, Vertex &v2);
        Edge* edgeTo(Vertex& v);
        void updateType();
        Vertex* post_Helper(Vertex* v);
};

#endif /* vertex_h */
