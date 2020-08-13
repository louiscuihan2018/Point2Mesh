#include "meshconvert.h"
#include "utils.h"
//#include "clock.h"
#include <iostream>
#include <fstream>


using namespace std;

namespace CGL {

    MeshConvert::MeshConvert(string file, double radius) {
        m_filename = file;
        m_radius = radius;
        count_i = 0;
        count_j = 0;

        init_data();
        init_ocsearch();
    }

    void MeshConvert::init_data() {
        pair< vector<Vector3D>&, vector<Vertex*>& > data(m_range, m_vertices);
        m_vtx_count = read_and_range(m_filename, &data);
        cout << "init data" << endl;
    }

    void MeshConvert::init_ocsearch() {
        Vector3D& box_min = m_range[0], box_max = m_range[1];
        // some margin is reserved to avoid edge case
        Vector3D margin = 0.1 * (box_max - box_min);
        Vector3D size = 1.2 * (box_max - box_min);
        Vector3D origin = box_min - margin;

        uint tree_depth = (uint)(log(m_vtx_count) / log(8)) + 1;

        cout << box_min << " " << box_max << endl;
        cout << origin << " " << size << " " << tree_depth << endl;
        m_tree = new OcTree(origin, size, tree_depth);

        // construct octree from data read in init_data
        m_tree->populate_tree(m_vertices.begin(), m_vertices.end());
        m_ocsearch = new OcSearch(m_tree, 2 * m_radius);
    }

    bool MeshConvert::findSeedTriangle() {
        OcNode* start = m_tree->root;
        return findSeedTriangle(start);
    }

    bool MeshConvert::findSeedTriangle(OcNode* node) {
        if (node->depth != 0) {
            for (int i = 0; i < 8; i++) {
                if (node->children[i] != NULL) {
                    bool found = findSeedTriangle(node->children[i]);
                    if (found) return found;
                }
            }
            return false;
        }
        else {
            for (Vertex* x : node->pts) {
                if (x->type == ORPHAN) {
                    if (trySeedVertex(x)) return true;
                }
            }
            return false;
        }
    }

    bool MeshConvert::trySeedVertex(Vertex* v) {
        Neighbor_map n2r;
        m_ocsearch->set_radius(2 * m_radius);
        m_ocsearch->get_sorted_neighbors(v->point, &n2r);

        Neighbor_iter it = n2r.begin();

        if (n2r.size() < 3) return false;

        while (it != n2r.end()) {
            Vertex* u = (*it).second;
            if (u->type != ORPHAN || u == v) {
                it++;
                continue;
            }

            Neighbor_iter itt = it;
            itt++;

            Vertex* cand = NULL;
            while (itt != n2r.end()) {
                Vertex* w = (*itt).second;
                if (w->type != ORPHAN) {
                    itt++;
                    continue;
                }

                if (trySeedVertices(v, u, w, &n2r)) {
                    cand = w;
                    break;
                }
                itt++;
            }

            if (cand != NULL) {
                Edge* e1 = v->edgeTo(*cand);
                Edge* e2 = u->edgeTo(*cand);
                Edge* e3 = v->edgeTo(*u);
                
                if ((e1 != NULL && e1->type != e_type::E_FRONT) ||
                    (e2 != NULL && e2->type != e_type::E_FRONT) ||
                    (e3 != NULL && e3->type != e_type::E_FRONT)) {
                    it++;
                    continue;
                }

                // * create the triangle and add it to this class member
                Triangle* tri = new Triangle(v, u, cand);
//                Vector3D t_range = m_range[1] - m_range[0];
//
//
//                double x_max = max(abs(v->point.x - cand->point.x), max(abs(v->point.x - u->point.x), abs(u->point.x - cand->point.x)));
//                double y_max = max(abs(v->point.y - cand->point.y), max(abs(v->point.y - u->point.y), abs(u->point.y - cand->point.y)));
//                double z_max = max(abs(v->point.z - cand->point.z), max(abs(v->point.z - u->point.z), abs(u->point.z - cand->point.z)));
//                if (x_max > 0.05 * t_range.x || y_max > 0.05 * t_range.y  || z_max > 0.05 * t_range.z ){
//                    std::cout<< "seed";
//                    std::cout<< "\n";
//                    continue;
//                }
                m_triangles.push_back(tri);
                count_i += 1;
                e1 = v->edgeTo(*cand);
                e2 = u->edgeTo(*cand);
                e3 = v->edgeTo(*u);

                if (e1->type == e_type::E_FRONT) m_front_edges.push_front(e1);
                if (e2->type == e_type::E_FRONT) m_front_edges.push_front(e2);
                if (e3->type == e_type::E_FRONT) m_front_edges.push_front(e3);

                if (m_front_edges.size() > 0) return true;
            }

            it++;
        }
        return false;
    }

    bool MeshConvert::trySeedVertices(Vertex* v, Vertex* u, Vertex* w, Neighbor_map* map) {
        if (!v->compatible(*u, *w)) return false;
        Edge* e1 = v->edgeTo(*w);
        Edge* e2 = u->edgeTo(*w);
        if ((e1 != NULL && e1->type == e_type::E_INNER) ||
            (e2 != NULL && e2->type == e_type::E_INNER)) return false;

        Sphere s;
        if (!construct_ball(v, u, w, m_radius, s)) return false;

        double r2 = m_radius * m_radius;

        for (Neighbor_iter it = map->begin(); it != map->end(); it++) {
            double dist = (s.center - (*it).second->point).norm2();
            if (dist < r2) return false;
        }
        return true;
    }

    Vertex* MeshConvert::FindCandidate(Edge* e) {
        Vertex* candidate = NULL;
        double min_theta = 2.0 * M_PI;
        Vertex* v_source = e->from();
        Vertex* v_target = e->to();
        
        Vector3D m = (v_source->point + v_target->point) / 2.0;
        
        double r_2 = m_radius * m_radius;
        double m_c = sqrt(r_2 - (m - v_source->point).norm2());
        double r_n = m_radius + m_c;
        
        Neighbor_map nrn;
        m_ocsearch->set_radius(r_n);
        m_ocsearch->get_sorted_neighbors(m, &nrn);
        m_ocsearch->set_radius(m_radius);
        
        Triangle* t1 = e->face1;
        Sphere s1;
        construct_ball(t1->a, t1->b, t1->c, m_radius, s1);
        Vector3D c = s1.center;
        
        Vector3D edg = v_target->point - v_source->point;
        Vector3D mc = c - m;
        
        for (auto x = nrn.begin(); x != nrn.end(); x ++) {
            Vertex* v = x->second;
            
            if ((v == t1->a) || (v == t1->b) || (v == t1->c)) continue;

            if ((v->type == v_type::INNER) || (!v->compatible(*e))) continue;
            
            Sphere s2;
            if (!construct_ball(v_source, v_target, v, m_radius, s2)) {
                continue;
            }
            Vector3D c_n = s2.center;
            
            Vector3D mc_n = c_n - m;
            mc.normalize();
            mc_n.normalize();
            
            double cosine = dot(mc, mc_n);
            cosine = max(-1.0, min(1.0, cosine));

            double theta = acos(cosine);
            Vector3D cro = cross(mc, mc_n);
            edg.normalize();
            
            if (dot(cro, edg) < 0) {
                theta = 2 * M_PI - theta;
            }
            
            if (theta > min_theta) {
                continue;
            }

            bool clear = true;
            for (Neighbor_iter it = nrn.begin(); it != nrn.end() && clear != false; it++) {
                Vertex* curr = (*it).second;
                if ((curr == v_source) || (curr == v_target) || (curr == v)) {
                    continue;
                }
                double dist2 = (curr->point - c_n).norm2();

                if (dist2 < r_2) {
                    clear = false;
                }
            }
            
            if (clear) {
                candidate = v;
                min_theta = theta;
            }
        }
        
        return candidate;
        
    }

    void MeshConvert::expandTriangulation() {
        while (! m_front_edges.empty()) {
            Edge* e = m_front_edges.front();
            m_front_edges.pop_front();

            if (e->type != e_type::E_FRONT) {
                continue;
            }
            
            Vertex* v = FindCandidate(e);
            
            if (v == NULL) {
                e->type = e_type::BORDER;
                m_border_edges.push_front(e);
                continue;
            }
            if ((v->type == v_type::INNER) || (!v->compatible(*e))) {
                e->type = e_type::BORDER;
                m_border_edges.push_front(e);
                continue;
            }
            
            Vertex* vs =  e->from();
            Vertex* vt = e->to();
            Edge* e_s = v->edgeTo(*vs);
            Edge* e_t = v->edgeTo(*vt);
            
            if (e_s != NULL && e_s->type != E_FRONT) {
                e->type = BORDER;
                m_border_edges.push_front(e);
                continue;
            }
            if (e_t != NULL && e_t->type != E_FRONT) {
                e->type = BORDER;
                m_border_edges.push_front(e);
                continue;
            }
            
            Triangle* t = new Triangle(e->from(), e->to(), v);
//            Vector3D t_range = m_range[1] - m_range[0];
//
//            double x_max = max(abs(e->from()->point.x - e->to()->point.x), max(abs(e->to()->point.x - v->point.x), abs(v->point.x - e->from()->point.x)));
//            double y_max = max(abs(e->from()->point.y - e->to()->point.y), max(abs(e->to()->point.y - v->point.y), abs(v->point.y - e->from()->point.y)));
//            double z_max = max(abs(e->from()->point.z - e->to()->point.z), max(abs(e->to()->point.z - v->point.z), abs(v->point.z - e->from()->point.z)));
//            if (x_max > 0.05 * t_range.x || y_max > 0.05 * t_range.y  || z_max > 0.05 * t_range.z ){
//                std::cout<< "expand";
//                std::cout<< "\n";
//                continue;
//            }
            m_triangles.push_front(t);
            count_j +=1;
            e_s = v->edgeTo(*vs);
            e_t = v->edgeTo(*vt);
            
            if (e_s->face1 != NULL && e_s->face2 != NULL) {
                e_s->type = E_INNER;
            } else {
                m_front_edges.push_front(e_s);
            }
            
            if (e_t->face1 != NULL && e_t->face2 != NULL) {
                e_t->type = E_INNER;
            } else {
                m_front_edges.push_front(e_t);
            }
        }
    }

    void MeshConvert::postProcess() {
        list<Edge*>::iterator it = m_border_edges.begin();
        while (it != m_border_edges.end()) {
            Edge* e = *it;
            if (e->type != e_type::BORDER) {
                it = m_border_edges.erase(it);
                continue;
            }
            Vertex* vs = e->from(); 
            Vertex* vt = e->to();

            Vertex* v = NULL;

            Edge* e0 = vs->edgeTo(*vt);
            Triangle* tri = e0->face1;

            vector<Edge*>::iterator ei = vs->adjacent_edges.begin();
            while (ei != vs->adjacent_edges.end())
            {
                if ((*ei)->type != e_type::BORDER)
                {
                    ++ei;
                    continue;
                }
                Vertex* u = (*ei)->from();

                if (u == vs)
                {
                    ++ei;
                    continue;
                }

                if (u == tri->a || u == tri->b || u == tri->c)
                {
                    ++ei;
                    continue;
                }

                Edge* e1 = u->edgeTo(*vt);
                if (e1 == NULL)
                {
                    ++ei;
                    continue;
                }

                if (e1->type != e_type::BORDER || e1->from() != vt)
                {
                    ++ei;
                    continue;
                }
                v = u;
                break;
            }

            if (v == NULL) {
                it++;
                continue;
            }

            Triangle* t = new Triangle(vs, vt, v);
            m_triangles.push_back(t);

            it = m_border_edges.erase(it);
        }

    }
    void MeshConvert::sharp() {
        list<Triangle*>::iterator it = m_triangles.begin();
        while (it != m_triangles.end()) {
            Triangle* t = *it;
            Sphere s;
            if (!construct_ball(t->a, t->b, t->c, m_radius, s)) {
                it = m_triangles.erase(it);
                continue;
            }
            it ++;
            
        }
    }

    void MeshConvert::construct() {
        //if (m_front_edges.empty()) {
        //    bool found = findSeedTriangle();
        //    if (!found) {
        //        std::cout<< "no seed";
        //    }
        //    expandTriangulation();
        //}
        //else {
        //    expandTriangulation();
        //}
        //while (findSeedTriangle()) {
        //    expandTriangulation();
        //}

        while (m_front_edges.empty()) {
            if (!findSeedTriangle()) break;
            else expandTriangulation();
        }
//        postProcess();
//        sharp();
        
    }

    void MeshConvert::write_to_file() {
        string out_p = "point.txt";
        string out_n = "normal.txt";
        string out_t = "triangle.txt";
        ofstream out_1;
        out_1.open(out_p);
        if (!out_1) {
            std::cout<<"error p";
            std::cout<< "\n";
        }
        for (Vertex* v : m_vertices) {
            out_1 << v->point.x<< " ";
            out_1 << v->point.y<< " ";
            out_1 << v->point.z;
            out_1 << "\n";
        }
        out_1.close();

        ofstream out_2;
        out_2.open(out_n);
        if (!out_1) {
            std::cout<<"error n";
            std::cout<< "\n";
        }
        for (Vertex* v : m_vertices) {
            out_2 << v->normal.x<< " ";
            out_2 << v->normal.y<< " ";
            out_2 << v->normal.z;
            out_2 << "\n";
        }
        out_2.close();

        ofstream out_3;
        out_3.open(out_t);
        if (!out_3) {
            std::cout<<"error t";
            std::cout<< "\n";
        }
        for (Triangle* t : m_triangles) {
            out_3 << t->a->index<< " ";
            out_3 << t->b->index<< " ";
            out_3 << t->c->index;
            out_3 << "\n";
        }
        out_3.close();
    }
    
    void MeshConvert::data_output(string name,list<double> timing) {
        string out_p = "log.txt";
        ofstream out_1;
        out_1.open(out_p);
        if (!out_1) {
            std::cout<<"error output";
            std::cout<< "\n";
        }
        out_1<< name;
        out_1<< "\n";
        
        out_1<< "radius : ";
        out_1<< m_radius;
        out_1<< "\n";
        
        out_1<<"max depth : ";
        out_1<< m_tree->max_depth;
        out_1<< "\n";
        
        out_1<<"vertex number : ";
        out_1<< m_vertices.size();
        out_1<< "\n";
        
        out_1<<"triangles number : ";
        out_1<< m_triangles.size();
        out_1<< "\n";
        
        out_1<<"front edge number : ";
        out_1<< m_front_edges.size();
        out_1<< "\n";
        
        out_1<<"border edge number : ";
        out_1<< m_border_edges.size();
        out_1<< "\n";
        
        out_1<<"triangles created by findseed : ";
        out_1<< count_i;
        out_1<< "\n";
        
        out_1<<"triangles created by expand : ";
        out_1<< count_j;
        out_1<< "\n";
        
        auto it = timing.begin();
        out_1<<"initilization time  : ";
        out_1<< *it;
        out_1<< " ms";
        out_1<< "\n";
        
        it ++;
        out_1<<"construction time  : ";
        out_1<< *it;
        out_1<< " ms";
        out_1<< "\n";
        
        it ++;
        out_1<<"postprocess time  : ";
        out_1<< *it;
        out_1<< " ms";
        out_1<< "\n";
            
        out_1.close();
        
    }
}
