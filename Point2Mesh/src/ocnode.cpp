#include "ocnode.h"
#include <iostream>

using namespace std;

namespace CGL {

    OcNode::OcNode() {
        parent = NULL;
        depth = 0;
        is_leaf = true;
        size = origin = locs = Vector3D(0);

        for (int i = 0; i < 8; i++)
            children[i] = NULL;
    }

    OcNode::OcNode(Vector3D og, Vector3D sz, Vector3D lc, OcNode* pr, uint dp) {
        parent = pr;
        depth = dp;
        origin = og;
        size = sz;
        locs = lc;
        is_leaf = (dp == 0);

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

    void OcNode::addChildren(uint index, Vector3D og, Vector3D sz) {
        cout << "index: " << index << endl;
        uint child_dp = depth - 1;
        children[index] = new OcNode(og, sz, NULL, this, child_dp);
    }
}