#include "ocnode.h"

using namespace std;

namespace CGL {

    OcNode::OcNode() {
        parent = NULL;
        depth = 0;
        is_leaf = false;
        size = origin = locs = Vector3D(0);

        for (int i = 0; i < 8; i++)
            children[i] = NULL;
    }

    OcNode::OcNode(Vector3D og, Vector3D sz, OcNode* pr, int dp) {
        parent = pr;
        depth = dp;
        origin = og;
        size = sz;
        is_leaf = (dp == 0);
        locs = Vector3D(0);

        for (int i = 0; i < 8; i++)
            children[i] = NULL;
    }

    OcNode::~OcNode() { 
        pts.clear();
        size = origin = locs = NULL;
        depth = 0;
        is_leaf = false;

        for (int i = 0; i < 8; i++)
            delete children[i];
    }

    void OcNode::addChildren(int index, Vector3D child_og) {
        Vector3D child_sz = size / 2.0;
        int child_dp = depth - 1;

        children[index] = new OcNode(child_og, child_sz, this, child_dp);
    }
}