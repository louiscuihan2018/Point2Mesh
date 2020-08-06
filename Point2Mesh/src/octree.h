//
// octree is the core data structure together with octreeNodes
//

#ifndef octree_h
#define octree_h

#include "CGL\Vector3D.h"
#include "ocnode.h"
#include <list>
#include <vector>

using namespace std;

namespace CGL {
    class OcTree {
    public:
        // constructor/destructor
        OcTree();
        OcTree(int depth);
        OcTree(Vector3D og, Vector3D sz, int depth);
        ~OcTree();

        // class members
        OcNode* root;

        int max_depth;
        int binsize;

        Vector3D size;
        Vector3D origin;

        // class methods
        void populate_tree(vector<Vector3D>::iterator start, vector<Vector3D>::iterator end);

    protected:
    };
}

#endif