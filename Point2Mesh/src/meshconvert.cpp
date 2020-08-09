#include "meshconvert.h"
#include "utils.h"

using namespace std;

namespace CGL {

    MeshConvert::MeshConvert(string file, double radius) {
        m_filename = file;
        m_radius = radius;

        init_data();
    }

    void MeshConvert::init_data() {
        pair< vector<Vector3D>&, vector<Vertex>& > data(m_range, m_vertices);
        m_vtx_count = read_and_range(m_filename, &data);
    }
}