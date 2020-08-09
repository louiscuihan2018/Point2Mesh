#include "meshconvert.h"
#include "utils.h"

using namespace std;

namespace CGL {

    MeshConvert::MeshConvert(string file, double radius) {
        m_filename = file;
        m_radius = radius;

        init_data();
        init_ocsearch();
    }

    void MeshConvert::init_data() {
        pair< vector<Vector3D>&, vector<Vertex>& > data(m_range, m_vertices);
        m_vtx_count = read_and_range(m_filename, &data);
    }

    void MeshConvert::init_ocsearch() {
        Vector3D& box_min = m_range[0], box_max = m_range[1];
        // some margin is reserved to avoid edge case
        Vector3D margin = 0.1 * (box_max - box_min);
        Vector3D size = 1.2 * (box_max - box_min);
        Vector3D origin = box_min - margin;

        uint tree_depth = (uint)(log(m_vtx_count) / log(8));
        m_tree = OcTree(origin, size, tree_depth);

        // construct octree from data read in init_data
        m_tree.populate_tree(m_vertices.begin(), m_vertices.end());

        m_ocsearch = OcSearch(&m_tree, 2 * m_radius);
    }
}