#include "ocsearch.h"

using namespace std;

namespace CGL {
    OcSearch::OcSearch() {
        s_radius = s_level = 0;
        s_tree = NULL;
    }

    OcSearch::OcSearch(OcTree* tree, double radius) {
        s_tree = tree;
        s_radius = radius;
        s_level = calc_level(radius);
    }

    OcSearch::OcSearch(OcTree* tree, double radius, uint level) {
        s_tree = tree;
        s_radius = radius;
        s_level = level;
    }

    OcSearch::~OcSearch() {
        if (s_tree != NULL) {
//            delete s_tree;
            s_tree = NULL;
        }
    }

    uint OcSearch::get_sorted_neighbors(Vector3D& query, Neighbor_map* neighbors) {
        OcNode* q_node = find_node_from_point(query);
        double radius = s_radius;

        Vector3D& q_og = q_node->origin; Vector3D& q_sz = q_node->size;
        Vector3D& r_og = s_tree->origin; Vector3D& r_sz = s_tree->size;

        list<uint> xlist, ylist, zlist;

        xlist.push_back((uint)q_node->locs.x);
        ylist.push_back((uint)q_node->locs.y);
        zlist.push_back((uint)q_node->locs.z);

        if ((query.x - radius >= r_og.x) && (query.x - radius < q_og.x))
            xlist.push_back(xloc_left(q_node));
        if ((query.x + radius <= r_og.x + r_sz.x) && (query.x + radius > q_og.x + q_sz.x))
            xlist.push_back(xloc_right(q_node));
        if ((query.y - radius >= r_og.y) && (query.y - radius < q_og.y))
            xlist.push_back(yloc_left(q_node));
        if ((query.y + radius <= r_og.y + r_sz.y) && (query.y + radius > q_og.y + q_sz.y))
            xlist.push_back(yloc_right(q_node));
        if ((query.z - radius >= r_og.z) && (query.z - radius < q_og.z))
            xlist.push_back(xloc_left(q_node));
        if ((query.z + radius <= r_og.z + r_sz.z) && (query.z + radius > q_og.z + q_sz.z))
            xlist.push_back(xloc_right(q_node));

        uint dp = q_node->depth;

        for (auto xi = xlist.begin(); xi != xlist.end(); xi++)
            for (auto yi = ylist.begin(); yi != ylist.end(); yi++)
                for (auto zi = zlist.begin(); zi != zlist.end(); zi++) {
                    OcNode* n_node = s_tree->root;
                    go_to_level(*xi, *yi, *zi, dp, &n_node);
                    if (n_node != NULL && n_node->depth == dp) {
                        iterate_node(query, n_node, neighbors);
                    }
                }

        return neighbors->size();
    }

    void OcSearch::iterate_node(Vector3D& query, OcNode* node, Neighbor_map* neighbors) {
        if (node->depth != 0) {
            for (int i = 0; i < 8; i++)
                if (node->children[i] != NULL)
                    iterate_node(query, node->children[i], neighbors);
        }

        list<Vertex*>& vs = node->pts;
        for (auto vi = vs.begin(); vi != vs.end(); vi++) {
            Vector3D& p = (*vi)->point;
            double dist = (query - p).norm();
            if (dist <= s_radius)
                neighbors->insert(pair<double, Vertex*>(dist, *vi));
        }
    }

    void OcSearch::go_to_level(Vector3D locs, uint lv, OcNode** node) {
        uint xloc = (uint)locs.x; uint yloc = (uint)locs.y; uint zloc = (uint)locs.z;
        go_to_level(xloc, yloc, zloc, lv, node);
    }

    void OcSearch::go_to_level(uint xloc, uint yloc, uint zloc, uint lv, OcNode** node) {
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

    OcNode* OcSearch::find_node_from_point(Vector3D& query) {
        OcNode* node = s_tree->root;
        int binsize = s_tree->binsize;
        Vector3D& og = node->origin;
        Vector3D& sz = node->size;

        uint xloc = (uint)binsize * (query.x - og.x) / sz.x;
        uint yloc = (uint)binsize * (query.y - og.y) / sz.y;
        uint zloc = (uint)binsize * (query.z - og.z) / sz.z;

        go_to_level(xloc, yloc, zloc, s_level, &node);
        return node;
    }

    uint OcSearch::calc_level(double radius) {
        uint lv = s_tree->root->depth;
        Vector3D sz = s_tree->root->size / 2.0;

        while (lv != 0) {
            if (radius < min(min(sz.x, sz.y), sz.z)) {
                lv--;
                sz = sz / 2.0;
            }
            else return lv;
        }

        return 0;
    }

    uint OcSearch::xloc_left(OcNode* node) const {
        return node->locs.x - (uint)pow(2, node->depth);
    }

    uint OcSearch::yloc_left(OcNode* node) const {
        return node->locs.y - (uint)pow(2, node->depth);
    }

    uint OcSearch::zloc_left(OcNode* node) const {
        return node->locs.z - (uint)pow(2, node->depth);
    }

    uint OcSearch::xloc_right(OcNode* node) const {
        return node->locs.x + (uint)pow(2, node->depth);
    }

    uint OcSearch::yloc_right(OcNode* node) const {
        return node->locs.y + (uint)pow(2, node->depth);
    }

    uint OcSearch::zloc_right(OcNode* node) const {
        return node->locs.z + (uint)pow(2, node->depth);
    }
}
