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
#include "meshconvert.h"
using namespace CGL;

pair<Triangle*, bool> check_and_initialize_tri(Vertex* a, Vertex* b, Vertex* c) {
    Vertex v1 = *a;
    Vertex v2 = *b;
    Vertex v3 = *c;

    bool check = v1.compatible(v2, v3);
    if (check) {
        Triangle* t = new Triangle(a, b, c);
        return std::make_pair(t, true);
    }
    else {
        Triangle* t = NULL;
        return std::make_pair(t, true);
    }
}

uint read_and_range(string name, pair< vector<Vector3D>&, vector<Vertex*>& >* data) {
    // name of file change here
    // need to be in the target or working folder
    ifstream file;
    file.open(name);

    vector<Vertex*>& vertices_t = data->second;
    vector<Vector3D>& range = data->first;

    if (!file.is_open()) {
        cout << "cannot read data file " << name << " successfully" << endl;
        return 0;
    }

    uint v_number;

    file >> v_number;

    double minx = HUGE_VAL;
    double miny = HUGE_VAL;
    double minz = HUGE_VAL;
    double maxx = -HUGE_VAL;
    double maxy = -HUGE_VAL;
    double maxz = -HUGE_VAL;
    for (int i = 0; i < v_number; i++) {
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
        Vertex* c = new Vertex(curr_point, curr_normal);
        c->index = i;
        vertices_t.push_back(c);
    }

    Vector3D min = Vector3D(minx, miny, minz);
    Vector3D max = Vector3D(maxx, maxy, maxz);
    range.push_back(min);
    range.push_back(max);

    file.close();

    return v_number;
}


Triangle* FindSeedTriangle(OcTree* tree, double r) {
    Triangle* result = NULL;
    result = FindSeedTriangle(tree, tree->root, r);
    return result;
}

Triangle* FindSeedTriangle(OcTree* tree, OcNode* node, double r) {
    //    Triangle* result = NULL;
    if (node->depth != 0) {
        for (int i = 0; i < 8; i++) {
            if (node->children[i] != NULL) {
                Triangle* curr = FindSeedTriangle(tree, node->children[i], r);
                if (curr != NULL) {
                    return curr;
                }
            }
        }
        return NULL;
    }
    else {
        uint level = tree->max_depth - 1;
        OcSearch* curr = new OcSearch(tree, 2 * r, level);
        for (Vertex* x : node->pts) {
            Neighbor_map n2r;
            curr->get_sorted_neighbors(x->point, &n2r);
            bool clear = true;
            for (auto& y : n2r) {
                for (auto& z : n2r) {
                    if (y == z) {
                        continue;
                    }

                    if ((!x->compatible(*(y.second), *(z.second))) || (!x->compatible(*(z.second), *(y.second)))) {
                        continue;
                    }

                    Sphere s1;
                    Triangle* t1 = NULL;
                    if (construct_ball(x, y.second, z.second, r, s1)){
                        t1 = new Triangle(x, y.second, z.second);
                    }

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

                if ((!x->compatible(*(i), *(j))) || (!x->compatible(*(j), *(i)))) {
                    continue;
                }

                Sphere s1;
                Triangle* t1 = NULL;
                if (construct_ball(x, i, j,r, s1)){
                    t1 = new Triangle(x,i,j);
                }
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

//bool construct_ball(Vertex* v1, Vertex* v2, Vertex* v3, double p, Sphere& s) {
//    Vector3D& a = v1->point;
//    Vector3D& b = v2->point;
//    Vector3D& c = v3->point;
//    Vector3D ac = c - a;
//    Vector3D ab = b - a;
//    Vector3D abXac = cross(ab, ac);
//    Vector3D acXab = cross(ac, ab);
//    Vector3D curr = ac.norm2() * (cross(abXac, ab)) + ab.norm2() * (cross(acXab, ac));
//    curr = curr / (2.0 * abXac.norm2());
//    Vector3D circumcenter = a + curr;
//
//    double dist = curr.norm();
//
//    if (p < dist) return false;
//
//    double adjustment = sqrt(p * p - dist * dist);
//    abXac.normalize();
//    Vector3D facenormal = abXac;
//    Vector3D center = circumcenter - abXac * adjustment;
//
//    s = Sphere(center, p);
//    return true;
//}

bool construct_ball(Vertex* v1, Vertex* v2, Vertex* v3, double p, Sphere& s) {
    double r_2 = p * p;
    Vector3D& a = v1->point;
    Vector3D& b = v2->point;
    Vector3D& c = v3->point;

    double ab = (a - b).norm2();
    double bc = (b - c).norm2();
    double ca = (c - a).norm2();

    double alpha = ab * (bc + ca - ab);
    double beta = bc * (ab + ca - bc);
    double gamma = ca * (ab + bc - ca);
    double temp = alpha + beta + gamma;

    if (temp < 1e-10) return false;

    double x = alpha * a.x + beta * b.x + gamma * c.x;
    double y = alpha * a.y + beta * b.y + gamma * c.y;
    double z = alpha * a.z + beta * b.z + gamma * c.z;
    Vector3D circum_center(x, y, z);

    double circumr_2 = ab * bc * ca;

    ab = sqrt(ab); bc = sqrt(bc); ca = sqrt(ca);

    circumr_2 /= ((ab + bc + ca) * (bc + ca - ab) * (ca + ab - bc) * (ab + bc - ca));

    if (r_2 - circumr_2 < 0) return false;

    Vector3D normal;
    triangle_normal(a, b, c, normal);

    Vector3D center = circum_center + sqrt(r_2 - circumr_2) * normal;
    s = Sphere(center, p);
    return true;
}

static void triangle_normal(Vector3D& v1, Vector3D& v2, Vector3D& v3, Vector3D& normal) {
    normal = cross(v2 - v1, v3 - v1);
    normal.normalize();
    Vector3D avg = v1 + v2 + v3;
    avg.normalize();

    if (dot(avg, normal) < 0) {
        normal = -normal;
    }
}

Vertex* FindCandidate(Edge* e, OcTree* tree, double r) {
    Vertex* candidate = NULL;
    double min_theta = 2.0 * M_PI;
    Triangle* t1 = e->face1;
    Triangle* t2 = e->face2;
    if (t1 != NULL) {
        Sphere s;
        construct_ball(t1->a, t1->b, t1->c, r, s);
        Vector3D c = s.center;
        Vector3D v1 = e->from()->point;
        Vector3D v2 = e->to()->point;
        Vector3D m = (v1 + v2) / 2.0;
        bool check = (c.x != c.x || c.y != c.y || c.z != c.z);
        if (!check) {
            double r_n = (m - c).norm() + r;
            uint level = tree->max_depth - 1;

            OcSearch* curr_1 = new OcSearch(tree, r_n, level);
            multimap<double, Vertex*> nrn;
            curr_1->get_sorted_neighbors(m, &nrn);

            OcSearch* curr_2 = new OcSearch(tree, 2 * r, level);
            multimap<double, Vertex*> n2r;
            curr_2->get_sorted_neighbors(m, &n2r);

            for (auto x : n2r) {
                Vertex* v = x.second;

                if (v->type == v_type::INNER || v == e->from() || v == e->to()) {
                    continue;
                }
                if (!v->compatible(*e)) {
                    continue;
                }
                if (v == t1->a || v == t1->b || v == t1->c) {
                    continue;
                }

                Sphere s_curr;
                if (!(construct_ball(e->from(), e->to(), v, r, s_curr))){
                    continue;
                }
                Vector3D c_new = s_curr.center;

                Vector3D vec1 = c_new - m;
                vec1.normalize();
                Vector3D vec2 = c - m;
                vec2.normalize();
                double cosine = dot(vec1, vec2);
                double theta = acos(cosine);

                Vector3D cro = cross(vec2, vec1);
                Vector3D edg = e->to()->point - e->from()->point;
                if (dot(cro, edg) < 0) {
                    theta = 2.0 * M_PI - theta;
                }
                if (theta > min_theta) {
                    continue;
                }
                bool clear = true;
                for (auto y : nrn) {
                    if (y == x) {
                        continue;
                    }
                    Vertex* c = y.second;
                    if (c == e->from() || c == e->to()) {
                        continue;
                    }
                    Vector3D diff = y.second->point - c_new;
                    double dist = diff.norm();
                    if (dist < r) {
                        clear = false;
                    }

                }

                if (clear) {
                    candidate = v;
                    min_theta = theta;
                }
            }
        }
    }

    if (t2 != NULL) {
        Sphere s;
        construct_ball(t2->a, t2->b, t2->c, r, s);
        Vector3D c = s.center;
        Vector3D v1 = e->from()->point;
        Vector3D v2 = e->to()->point;
        Vector3D m = (v1 + v2) / 2.0;
        bool check = (c.x != c.x || c.y != c.y || c.z != c.z);
        if (!check) {
            double r_n = (m - c).norm() + r;
            uint level = tree->max_depth - 1;

            OcSearch* curr_1 = new OcSearch(tree, r_n, level);
            multimap<double, Vertex*> nrn;
            curr_1->get_sorted_neighbors(m, &nrn);

            OcSearch* curr_2 = new OcSearch(tree, 2 * r, level);
            multimap<double, Vertex*> n2r;
            curr_2->get_sorted_neighbors(m, &n2r);

            for (auto x : n2r) {
                Vertex* v = x.second;

                if (v->type == v_type::INNER || v == e->from() || v == e->to()) {
                    continue;
                }
                if (!v->compatible(*e)) {
                    continue;
                }
                if (v == t1->a || v == t1->b || v == t1->c) {
                    continue;
                }

                Sphere s_curr;
                if (!(construct_ball(e->from(), e->to(), v, r, s_curr))){
                    continue;
                }
                Vector3D c_new = s_curr.center;

                Vector3D vec1 = c_new - m;
                vec1.normalize();
                Vector3D vec2 = c - m;
                vec2.normalize();
                double cosine = dot(vec1, vec2);
                double theta = acos(cosine);

                Vector3D cro = cross(vec2, vec1);
                Vector3D edg = e->from()->point - e->to()->point;
                if (dot(cro, edg) < 0) {
                    theta = 2.0 * M_PI - theta;
                }
                if (theta > min_theta) {
                    continue;
                }
                bool clear = true;
                for (auto y : nrn) {
                    if (y == x) {
                        continue;
                    }
                    Vertex* c = y.second;
                    if (c == e->from() || c == e->to()) {
                        continue;
                    }
                    Vector3D diff = y.second->point - c_new;
                    double dist = diff.norm();
                    if (dist < r) {
                        clear = false;
                    }

                }

                if (clear) {
                    candidate = v;
                    min_theta = theta;
                }
            }
        }
    }

    return candidate;

}
