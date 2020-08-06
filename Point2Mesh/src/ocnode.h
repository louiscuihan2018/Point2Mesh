#ifndef ocnode_h
#define ocnode_h

#include "CGL\Vector3D.h"
#include <list>

using namespace std;

namespace CGL {
    class OcNode {
    public:
        // constructor/desctructor
        OcNode();
        OcNode(Vector3D og, Vector3D sz, OcNode* pr, int dp);
        ~OcNode();

        // important methods
        void addChildren(int index, Vector3D child_og);

        // class members

        // parents & children
        OcNode* parent;
        OcNode* children[8];

        int depth;
        bool is_leaf;

        Vector3D size;
        Vector3D origin;
        // locational codes for this node
        Vector3D locs;

        list<Vector3D> pts;
    };
}

#endif