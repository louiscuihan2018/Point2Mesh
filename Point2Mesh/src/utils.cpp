//
//  utils.cpp
//  CGL
//
//  Created by louis on 8/5/20.
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include "utils.h"
#include "types.h"
#include "vertex.h"
#include "edge.h"
#include "sphere.h"
#include "triangle.h"
#include "ocsearch.h"
using namespace CGL;

void demo_read() {
    std::string line;
    std::ifstream rfile;
    rfile.open("../Point2Mesh/bun_zipper.xyz");
    int i = 0;
    if (rfile.is_open()) {
        while (i < 20) {
            double a, b, c;
            rfile >> a;
            rfile >> b;
            rfile >> c;
            cout << a << b << c << endl;
            i++;
        }
        rfile.close();
    }
    
}

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c) {
    Vertex v1 = *a;
    Vertex v2 = *b;
    Vertex v3 = *c;
    
    bool check = v1.compatible(v2, v3);
    if (check) {
        Triangle* t = new Triangle(a, b, c);
        return std::make_pair(t, true);
    } else {
        Triangle* t = NULL;
        return std::make_pair(t, true);
    }
}

pair< vector<Vector3D>,vector<Vertex> > read_and_range(string name) {
    // name of file change here
    // need to be in the target or working folder
    ifstream file;
    file.open(name);
    
    vector<Vertex> vertices_t;
    vector<Vector3D> range;
    
    if (!file.is_open()) return make_pair(range, vertices_t);
    
    uint v_number;

    file >> v_number;
    
    double minx = HUGE_VAL;
    double miny = HUGE_VAL;
    double minz = HUGE_VAL;
    double maxx = -HUGE_VAL;
    double maxy = -HUGE_VAL;
    double maxz = -HUGE_VAL;
    for (int i = 0; i < v_number; i ++) {
        double x;
        double y;
        double z;
        double dx;
        double dy;
        double dz;
        file >> x;
        file >> y;
        file >> z;
        file >> dx;
        file >> dy;
        file >> dz;
        
        if (x < minx) {
            minx = x;
        }
        if (x > maxx) {
            maxx = x;
        }
        if (y < miny) {
            miny = y;
        }
        if (y > maxy) {
            maxy = y;
        }
        if (z < minz) {
            minz = z;
        }
        if (z > maxz) {
            maxz = z;
        }
        
        Vector3D curr_point = Vector3D(x, y, z);
        Vector3D curr_normal = Vector3D(dx, dy, dz);
        vertices_t.push_back(Vertex(curr_point, curr_normal));
    }
    Vector3D min = Vector3D(minx, miny, minz);
    Vector3D max = Vector3D(maxx, maxy, maxz);
    range.push_back(min);
    range.push_back(max);
    
    pair< vector<Vector3D>, vector<Vertex> > result = make_pair(range, vertices_t);
    file.close();

    return result;
}

Triangle* FindSeedTriangle(OcTree* tree, double r) {
    Triangle* result = NULL;
    result = FindSeedTriangle(tree, tree->root, r);
    return result;
}

Triangle* FindSeedTriangle(OcTree* tree, OcNode* node, double r) {
//    Triangle* result = NULL;
    if (node->depth != 0) {
        for (int i = 0; i < 8; i ++) {
            if (node->children[i] != NULL) {
                Triangle* curr = FindSeedTriangle(tree, node->children[i], r);
                if (curr != NULL) {
                    return curr;
                }
            }
        }
        return NULL;
    } else {
        uint level = tree->max_depth - 1;
        OcSearch* curr = new OcSearch(tree, 2 * r, level);
        for (Vertex* x : node->pts) {
            multimap<double, Vertex*> n2r;
            curr->get_sorted_neighbors(x->point, &n2r);
            bool clear = true;
            for (auto y : n2r) {
                for (auto z: n2r) {
                    if (y == z) {
                        continue;
                    }

                    if ((!x->compatible(*(y.second), *(z.second))) || (!x->compatible(*(y.second), *(z.second)))) {
                        continue;
                    }

                    Triangle* t1 = new Triangle(x, y.second, z.second);
                    Sphere s1 = t1->construct_ball(r);

                    for (auto m : n2r) {
                        if (m == y || m == z) {
                            continue;
                        }
                        Vector3D diff = m.second->point - s1.center;
                        double dist = diff.norm();
                        if (dist < r) {
                            clear = false;
                        }
                    }

                    if (clear) {
                        return t1;
                    }
                }
            }
        }
        return NULL;
    }

}

Triangle* FindSeedTriangle(vector<Vertex*> vlist, double r) {
    Triangle* result = NULL;
    for (Vertex* x : vlist) {
        vector<Vertex*> curr_list;
        bool clear = true;
        for (Vertex* y : vlist) {
            if (y == x) {
                continue;
            }
            else {
                Vector3D diff = x->point - y->point;
                double dist = diff.norm();
                if (dist <= 2 * r) {
                    curr_list.push_back(y);
                }
            }
        }
        
        for (Vertex* i : curr_list) {
            for (Vertex* j : curr_list) {
                if (i == j) {
                    continue;
                }
                
                if ((!x->compatible(*(i), *(j))) || (!x->compatible(*(i), *(j)))) {
                    continue;
                }
                
                Triangle* t1 = new Triangle(x, i, j);
                Sphere s1 = t1->construct_ball(r);
                
                for (Vertex* k : curr_list) {
                    if (k == i || k == j) {
                        continue;
                    }
                    Vector3D diff = k->point - s1.center;
                    double dist = diff.norm();
                    
                    if (dist < r) {
                        clear = false;
                    }

                }
                if (clear) {
                    return t1;
                }
                
            }
        }
        
    }
    
}
