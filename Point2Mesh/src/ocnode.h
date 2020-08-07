#ifndef ocnode_h
#define ocnode_h

#include "vertex.h"
#include <list>

using namespace std;

namespace CGL {
    class OcNode {
    public:
        // constructor/desctructor
        OcNode();
        OcNode(Vector3D og, Vector3D sz, Vector3D lc, OcNode* pr, uint dp);
        ~OcNode();

        // important methods
        void addChildren(uint index, Vector3D og, Vector3D sz, Vector3D lc);

        // class members

        // parents & children
        OcNode* parent;
        OcNode* children[8];

        uint depth;
        bool is_leaf;

        Vector3D size;
        Vector3D origin;
        // locational codes for this node
        Vector3D locs;

        list<Vertex*> pts;
    };
}

#endif