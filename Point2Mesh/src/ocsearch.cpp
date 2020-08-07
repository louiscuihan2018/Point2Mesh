#include "ocsearch.h"

using namespace std;

namespace CGL {
    OcSearch::OcSearch() {
        s_radius = s_level = 0;
        s_tree = NULL;
    }

    OcSearch::OcSearch(OcTree* tree, double radius, uint level) {
        s_tree = tree;
        s_radius = radius;
        s_level = level;
    }

    OcSearch::~OcSearch() {
        if (s_tree != NULL) {
            delete s_tree;
            s_tree = NULL;
        }
    }

    void OcSearch::go_to_level(Vector3D locs, uint lv, OcNode** node) {
        uint xloc = (uint)locs.x; uint yloc = (uint)locs.y; uint zloc = (uint)locs.z;
        uint l = (*node)->depth - 1;

        while ((*node)->depth > lv) {
            uint branch_bit = 1 << l;
            uint x = (xloc & branch_bit) >> l;
            uint y = (yloc & branch_bit) >> l;
            uint z = (zloc & branch_bit) >> l;
            uint index = (x << 2) + (y << 1) + z;

            if ((*node)->children[index] != NULL) {
                *node = (*node)->children[index];
                l--;
            }
            else break;
        }
    }
}