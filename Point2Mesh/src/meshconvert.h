#ifndef meshconvert_h
#define meshconvert_h

#include "types.h"
#include "utils.h"
#include "ocsearch.h"
#include <string>
#include <vector>

using namespace std;

namespace CGL {
    class MeshConvert {
    public:
        // constructor & destructor
        MeshConvert(string file, double radius);

        void init_data();
        void init_ocsearch();

        double m_radius;
        uint m_vtx_count;
        uint m_tri_count;
        string m_filename;

        OcSearch m_ocsearch;
        OcTree m_tree;

        vector<Vertex> m_vertices;
        vector<Triangle> m_triangles;
        vector<Edge> m_front_edges;
        vector<Edge> m_border_edges;

        vector<Vector3D> m_range; 

    };
}

#endif