#include "octree.h"

using namespace std;

namespace CGL {
    OcTree::OcTree() {
        root = NULL;
        max_depth = 0;
        binsize = 0;
        size = Vector3D();
        origin = Vector3D();
    }

    OcTree::OcTree(int depth) : OcTree() { max_depth = depth; binsize = (int)pow(2, depth); }

    OcTree::OcTree(Vector3D og, Vector3D sz, int depth) {
        origin = og;
        size = sz;
        max_depth = depth;
        binsize = (int)pow(2, depth);
        root = new OcNode();
    }

    OcTree::~OcTree() {
        origin = NULL;
        size = NULL;
        max_depth = 0;
        binsize = 0;
        if (root != NULL) {
            delete root;
            root = NULL;
        }
    }
}

