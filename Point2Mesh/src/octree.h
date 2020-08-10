//
// octree is the core data structure together with octreeNodes
//

#ifndef octree_h
#define octree_h

#include "ocnode.h"
#include "vertex.h"
#include <list>
#include <vector>

using namespace std;

namespace CGL {
    class OcTree 
    {
    public:
        // constructor/destructor
        OcTree();
        OcTree(uint depth);
        OcTree(Vector3D og, Vector3D sz, uint depth);
        ~OcTree();

        // class members
        OcNode* root;

        uint max_depth;
        uint binsize;

        Vector3D size;
        Vector3D origin;

        // class methods
        void populate_tree(vector<Vertex*>::iterator start, vector<Vertex*>::iterator end);

        // debugging helpers
        void print_node_info(bool verbose);
    };
}

#endif