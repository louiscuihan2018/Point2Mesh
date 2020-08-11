#include "meshconvert.h"
#include "utils.h"


using namespace std;

namespace CGL {

    MeshConvert::MeshConvert(string file, double radius) {
        m_filename = file;
        m_radius = radius;

        init_data();
        init_ocsearch();
    }

    void MeshConvert::init_data() {
        pair< vector<Vector3D>&, vector<Vertex*>& > data(m_range, m_vertices);
        m_vtx_count = read_and_range(m_filename, &data);
    }

    void MeshConvert::init_ocsearch() {
        Vector3D& box_min = m_range[0], box_max = m_range[1];
        // some margin is reserved to avoid edge case
        Vector3D margin = 0.1 * (box_max - box_min);
        Vector3D size = 1.2 * (box_max - box_min);
        Vector3D origin = box_min - margin;

        uint tree_depth = (uint)(log(m_vtx_count) / log(8));
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
                m_triangles.push_back(tri);

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
        
        Vector3D m = v_source->point + v_target->point / 2.0;
        
        double r_n = m_radius + (m - v_source->point).norm();
        
        Neighbor_map nrn;
        m_ocsearch->set_radius(r_n);
        m_ocsearch->get_sorted_neighbors(m, &nrn);
        m_ocsearch->set_radius(m_radius);
        
        Triangle* t1 = e->face1;
        Sphere s1;
        construct_ball(t1->a, t1->b, t1->c, m_radius, s1);
        Vector3D c = s1.center;
        
        Vector3D edg = v_target->point - v_source->point;
        Vector3D mc = m - c;
        
        for (auto x = nrn.begin(); x != nrn.end(); x ++) {
            Vertex* v = x->second;
            
            if ((v == t1->a) || (v == t1->b) || (v == t1->c)) {
                continue;
            }
            
            Sphere s2;
            if (!construct_ball(v_source, v_target, v, m_radius, s2)) {
                continue;
            }
            Vector3D c_n = s2.center;
            
            Vector3D mc_n = c_n - m;
            mc.normalize();
            mc_n.normalize();
            
            double cosine = dot(mc, mc_n);
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
            for (auto x : nrn) {
                Vertex* curr = x.second;
                if ((curr == v_source) || (curr == v_target) || (curr == v)) {
                    continue;
                }
                Vector3D diff = curr->point - c_n;
                double dist = diff.norm();
                
                if (dist < m_radius) {
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

            if (e->type == BORDER|| e->type == E_INNER) {
                continue;
            }
            
            Vertex* v = FindCandidate(e);
            
            if (v == NULL) {
                e->type = BORDER;
                m_border_edges.push_front(e);
                continue;
            }
            if ((v->type == INNER) || (!v->compatible(*e))) {
                e->type = BORDER;
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
            m_triangles.push_front(t);
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
        for (Edge* x : m_border_edges) {
            
            if (x->type != BORDER) {
                m_border_edges.remove(x);
                continue;
            }
            
            Vertex* vs = x->from();
            Vertex* vt = x->to();
            
            Vertex* target = vs->post_Helper(vt);
            
            if (target != NULL) {
                m_border_edges.remove(x);
                Triangle* t = new Triangle(vs, vt, target);
                m_triangles.push_back(t);
            }
            
        }
    }

    void MeshConvert::construct() {
        if (m_front_edges.empty()) {
            bool found = findSeedTriangle();
            if (!found) {
                std::cout<< "no seed";
            }
            expandTriangulation();
            postProcess();
        }
        else {
            expandTriangulation();
            postProcess();
        }
        
    }
    
}
