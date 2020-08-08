#ifndef ocsearch_h
#define ocsearch_h

#include "ocnode.h"
#include "octree.h"
#include <map>

using namespace std;

namespace CGL {
    class OcSearch {
    public:
        // search radius
        double s_radius;
        // search level (max)
        uint s_level;

        OcTree* s_tree;

        OcSearch();
        OcSearch(OcTree* tree, double radius);
        OcSearch(OcTree* tree, double radius, uint level);
        ~OcSearch();

        // locating methods
        void go_to_level(Vector3D locs, uint lv, OcNode** node);
        uint get_sorted_neighbors(Vector3D& query, Neighbor_map* neighbors);  
        uint calc_level(double radius);

    private:
        void go_to_level(uint xloc, uint yloc, uint zloc, uint lv, OcNode** node);
        OcNode* find_node_from_point(Vector3D& query);
        void iterate_node(Vector3D& query, OcNode* node, Neighbor_map* neighbors);

        uint xloc_left(OcNode* node) const;
        uint xloc_right(OcNode* node) const;
        uint yloc_left(OcNode* node) const;
        uint yloc_right(OcNode* node) const;
        uint zloc_left(OcNode* node) const;
        uint zloc_right(OcNode* node) const;
    };
}

#endif