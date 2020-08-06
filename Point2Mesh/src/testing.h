#include "octree.h"
#include <random>
#include <vector>

using namespace CGL;
using namespace std;

namespace testing {
    /*Vector3D rand_vec() {
        double x = ((double)rand()) / RAND_MAX;
        double y = ((double)rand()) / RAND_MAX;
        double z = ((double)rand()) / RAND_MAX;
        return Vector3D(x, y, z);
    }*/

    void octree_test() {
        cout << "starting..." << endl;
        Vector3D og(0, 0, 0);
        Vector3D sz(2, 2, 2);

        OcTree t(og, sz, 2);

        vector<Vertex*> vs;
        for (int i = 0; i < 5; i++) {
            double x = 2 * ((double)rand()) / RAND_MAX;
            double y = 2 * ((double)rand()) / RAND_MAX;
            double z = 2 * ((double)rand()) / RAND_MAX;
            Vertex* v = new Vertex(Vector3D(x, y, z), NULL);
            vs.push_back(v);
        }

        for (auto v : vs) cout << v->point << endl;
        t.populate_tree(vs.begin(), vs.end());
        t.print_node_info(true);
    }

    void octree_test_basic() {
        cout << "starting..." << endl;
        Vector3D og(0, 0, 0);
        Vector3D sz(2, 2, 2);

        OcTree t(og, sz, 2);

        vector<Vertex*> vs;
        vs.push_back(&Vertex(Vector3D(1.9, 1.9, 1.9), NULL));

        for (auto v : vs) cout << v->point << endl;
        t.populate_tree(vs.begin(), vs.end());
        t.print_node_info(true);
    }
}