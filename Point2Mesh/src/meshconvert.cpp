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

    bool MeshConvert::findSeedTriangle() {
        OcNode* start = m_tree.root;
        return findSeedTriangle(start);
    }

    bool MeshConvert::findSeedTriangle(OcNode* node) {
        if (node->depth != 0) {
            for (int i = 0; i < 8; i++) {
                if (node->children[i] != NULL) {
                    bool found = findSeedTriangle(node->children[i]);
                    if (found) return found;
                }
            }
            return false;
        }
        else {
            for (Vertex* x : node->pts) {
                if (x->type == ORPHAN) {
                    if (trySeedVertex(x)) return true;
                }
            }
            return false;
        }
    }

    bool MeshConvert::trySeedVertex(Vertex* v) {
        Neighbor_map n2r;
        m_ocsearch.set_radius(2 * m_radius);
        m_ocsearch.get_sorted_neighbors(v->point, &n2r);

        Neighbor_iter it = n2r.begin();

        if (n2r.size() < 3) return false;

        while (it != n2r.end()) {
            Vertex* u = (*it).second;
            if (u->type != ORPHAN || u == v) {
                it++;
                continue;
            }

            Neighbor_iter itt = it;
            itt++;

            Vertex* cand = NULL;
            while (itt != n2r.end()) {
                Vertex* w = (*itt).second;
                if (w->type != ORPHAN) {
                    itt++;
                    continue;
                }

                if (trySeedVertices(v, u, w, &n2r)) {
                    cand = w;
                    break;
                }
                itt++;
            }

            if (cand != NULL) {
                //TODO
            }

            it++;
        }
    }

    bool MeshConvert::trySeedVertices(Vertex* v, Vertex* u, Vertex* w, Neighbor_map* map) {
        if (!v->compatible(*u, *w)) return false;
        //TODO
    }

    bool MeshConvert::emptyBallConfig(Vertex* v, Vertex* u, Vertex* w) {
        //TODO
        return false;
    }
}