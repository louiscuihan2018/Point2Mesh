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
#include "CGL/vector3D.h"

using namespace std;
namespace CGL {
    
class Edge;
    class Vertex{
        public:
            Vector3D point;
            Vector3D normal;
            bool is_inner;
            Edge_list adjacent_edges;
            Tri_list adjacent_triangles;
        Vertex(){};
        Vertex(Vector3D a, Vector3D b);
//        void add_edge(Edge* edge);
    };
}

#endif /* vertex_h */
