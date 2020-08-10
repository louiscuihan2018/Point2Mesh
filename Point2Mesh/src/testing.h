#include "octree.h"
#include "ocsearch.h"
#include "utils.h"
#include "clock.h"
#include "meshconvert.h"
#include <random>
#include <vector>

using namespace CGL;
using namespace std;

namespace testing {
    /*Vector3D rand_vec() {
        double x = ((double)rand()) / RAND_MAX;
        double y = ((double)rand()) / RAND_MAX;
        double z = ((double)rand()) / RAND_MAX;
        return Vector3D(x, y, z);
    }*/

    //void octree_test() {
    //    cout << "starting..." << endl;
    //    Vector3D og(0, 0, 0);
    //    Vector3D sz(2, 2, 2);

    //    OcTree t(og, sz, 2);

    //    vector<Vertex*> vs;
    //    for (int i = 0; i < 5; i++) {
    //        double x = 2 * ((double)rand()) / RAND_MAX;
    //        double y = 2 * ((double)rand()) / RAND_MAX;
    //        double z = 2 * ((double)rand()) / RAND_MAX;
    //        Vertex* v = new Vertex(Vector3D(x, y, z), NULL);
    //        vs.push_back(v);
    //    }

    //    for (auto v : vs) cout << v->point << endl;
    //    t.populate_tree(vs.begin(), vs.end());
    //    t.print_node_info(true);
    //}

    Timer t;

    void octree_test_basic() {
        cout << "starting..." << endl;
        Vector3D og(0, 0, 0);
        Vector3D sz(2, 2, 2);

        OcTree t(og, sz, 2);

        vector<Vertex> vs;
        Vertex a = Vertex(Vector3D(0.1, 0.1, 1.9), NULL);
        Vertex b = Vertex(Vector3D(0.6, 0.1, 1.9), NULL);
        Vertex c = Vertex(Vector3D(1.1, 0.1, 1.9), NULL);
        vs.push_back(a);
        vs.push_back(b);
        vs.push_back(c);

        for (auto v : vs) cout << v.point << endl;
        t.populate_tree(vs.begin(), vs.end());
        t.print_node_info(true);
    }

    void data_struct_basic_test() {
        Vector3D a = Vector3D(1, 2, 3);
        Vector3D b = Vector3D(4, 5, 6);
        Vector3D c = Vector3D(7, 8, 9);
        Vector3D d = Vector3D(0, 0, 0);
        Vertex x = Vertex(a, b);
        Vertex y = Vertex(c, d);
        Edge e1 = Edge(&x, &y);
        std::cout << e1.b->point.y;
        std::cout << "\n";
        Sphere s1 = Sphere(a, 2.0);
        std::cout << s1.center.z;
        std::cout << "\n";
        Vector3D p1 = Vector3D(2, 0, 0);
        Vector3D p2 = Vector3D(0, 2, 0);
        Vector3D p3 = Vector3D(0, 0, 2);
        Vector3D n = Vector3D(1, 1, 1);
        Vertex x1 = Vertex(p1, n);
        Vertex x2 = Vertex(p2, n);
        Vertex x3 = Vertex(p3, n);
        Triangle t1 = Triangle(&x1, &x2, &x3);
        Sphere s2 = t1.construct_ball(2.0);
        std::cout << s2.center.x;
        std::cout << "\n";
        std::cout << s2.center.y;
        std::cout << "\n";
        std::cout << s2.center.z;
        std::cout << "\n";
        std::cout << x.adjacent_edges.size();
        std::cout << "\n";
        std::cout << x1.adjacent_edges.size();
        std::cout << "\n";
        std::cout << x1.adjacent_triangles.size();
        std::cout << "\n";
        bool test_1 = (t1.ab->face1 == &t1);
        std::cout << test_1;
        std::cout << "\n";
    }

    void compatible_test() {
        Vector3D x_pos = Vector3D(1, 0, 0);
        Vector3D y_pos_1 = Vector3D(0, 1, 0);
        Vector3D y_pos_2 = Vector3D(0, -1, 0);
        Vector3D n = Vector3D(1, 1, 1);
        Vertex plane1 = Vertex(x_pos, n);
        Vertex plane2 = Vertex(y_pos_1, n);
        Vertex plane3 = Vertex(y_pos_2, n);
        Edge plane_e = Edge(&plane2, &plane3);

        pair<Triangle*, bool> foo = check_and_initialize_tri(&plane1, &plane2, &plane3);
        std::cout << std::get<1>(foo);
        std::cout << "\n";
        Triangle* t = std::get<0>(foo);
        std::cout << t->c->point.y;
        std::cout << "\n";
        std::cout << "\n";
    }

    void read_range_test() {
        //remember to set folder and make sure the file is within the folder with correct format;

#ifdef _WIN32
        string name = "../Point2Mesh/bun_zipper.xyz";
#else 
        string name = "bun_zipper.xyz";
#endif

        vector<Vector3D> v1; vector<Vertex> v2;
        pair< vector<Vector3D>&, vector<Vertex>& > res(v1, v2);
        uint n_vertex = read_and_range(name, &res);
        
        cout << n_vertex << endl;

        vector<Vertex> vertices = res.second;
        Vector3D min = res.first[0];
        Vector3D max = res.first[1];
        
        // test of construct time
        Vector3D o = min;
        Vector3D sz = max - min;
        uint dep = 6;
        OcTree tree = OcTree(o, sz, dep);
        tree.populate_tree(vertices.begin(), vertices.end());

        // test of search time
        t.reset();

        Vector3D a = Vector3D(0,0,0);
        OcSearch s = OcSearch(&tree, 0.0003, dep - 1);
        Neighbor_map curr;
        s.get_sorted_neighbors(a, &curr);
        
        t.stop();
    }

    void seed_tri_test() {
        
        // test of octree build and then search for seed triangle based on the tree
        
        // construct position and normal vectors for vertices
        Vector3D x_pos = Vector3D(1, 0, 0);
        Vector3D y_pos_1 = Vector3D(0, -1, 0);
        Vector3D y_pos_2 = Vector3D(0, 1, 0);
        Vector3D z_pos_1 = Vector3D(0, 0, 3);
        Vector3D z_pos_2 = Vector3D(0, 0, -3);
        Vector3D n = Vector3D(0, 0, 1);
        
        //construct vertices and store in a vector
        Vertex v1 = Vertex(x_pos, n);
        Vertex v2 = Vertex(y_pos_1, n);
        Vertex v3 = Vertex(y_pos_2, n);
        Vertex v4 = Vertex(z_pos_1, n);
        Vertex v5 = Vertex(z_pos_2, n);
        vector<Vertex> curr;
        curr.push_back(v5);
        curr.push_back(v3);
        curr.push_back(v4);
        curr.push_back(v1);
        curr.push_back(v2);
        
        //construct the octree
        Vector3D o = Vector3D(0, -1, -3);
        Vector3D s = Vector3D(1, 2, 6);

        OcTree tree = OcTree(o, s, 2);
        tree.populate_tree(curr.begin(), curr.end());
        
        // run find triangle for the tree, to check out the result, use breakpoints to check the vertices of the triangle
        Triangle* t = FindSeedTriangle(&tree, 1.0);
    }

    void seed_tri_test_simple() {
        
        Vector3D x_pos = Vector3D(1, 0, 0);
        Vector3D y_pos_1 = Vector3D(0, -1, 0);
        Vector3D y_pos_2 = Vector3D(0, 1, 0);
        Vector3D z_pos_1 = Vector3D(0, 0, 3);
        Vector3D z_pos_2 = Vector3D(0, 0, -3);
        Vector3D n = Vector3D(0, 0, 1);
        
        Vertex v1 = Vertex(x_pos, n);
        Vertex v2 = Vertex(y_pos_1, n);
        Vertex v3 = Vertex(y_pos_2, n);
        Vertex v4 = Vertex(z_pos_1, n);
        Vertex v5 = Vertex(z_pos_2, n);
        vector<Vertex*> curr;
        curr.push_back(&v5);
        curr.push_back(&v3);
        curr.push_back(&v4);
        curr.push_back(&v1);
        curr.push_back(&v2);

        Triangle* t = FindSeedTriangle(curr, 1.0);
    }

    void search_neighbor_test() {
        vector<Vertex> vss;

        // create a 11 x 11 grid for testing
        for (int i = 0; i < 11; i++)
            for (int j = 0; j < 11; j++) {
                Vector3D pos(i, j, 0);
                vss.push_back(Vertex(pos, pos));
            }
        
        Vector3D og(-1, -1, -1);
        Vector3D sz(12, 12, 2);

        OcTree t(og, sz, 4);
        t.populate_tree(vss.begin(), vss.end());
        t.print_node_info(true);

        OcSearch s(&t, 5);

        cout << "calculated level: " << s.s_level << endl;

        Vector3D query(2, 3, 0);

        Neighbor_map m;
        s.get_sorted_neighbors(query, &m);

        Neighbor_iter ni;

        for (ni = m.begin(); ni != m.end(); ni++) {
            pair<double, Vertex*> p = *ni;
            cout << p.first << " " << p.second->point << endl;
        }
        //for (pair<double, Vertex*> p : m)
        //    cout << p.first << " " << p.second->point << endl;
    }

    void mesher_test() {
        t.reset();

        string name = "../Point2Mesh/bun_zipper.xyz";
        MeshConvert converter(name, 1);
        cout << converter.m_vertices.size() << endl;

        t.stop();
    }
    
    void find_candidate_test() {
           Vector3D x_pos = Vector3D(1, 0, 0);
           Vector3D y_pos_1 = Vector3D(0, -1, 0);
           Vector3D y_pos_2 = Vector3D(0, 1, 0);
           Vector3D z_pos_1 = Vector3D(0, 0, 3);
           Vector3D z_pos_2 = Vector3D(0, 0, -3);
           Vector3D z_pos_3 = Vector3D(2, 1, 0);
           Vector3D n = Vector3D(0, 0, 1);
           
           //construct vertices and store in a vector
           Vertex v1 = Vertex(x_pos, n);
           Vertex v2 = Vertex(y_pos_1, n);
           Vertex v3 = Vertex(y_pos_2, n);
           Vertex v4 = Vertex(z_pos_1, n);
           Vertex v5 = Vertex(z_pos_2, n);
           Vertex v6 = Vertex(z_pos_3, n);
           vector<Vertex> curr;
           curr.push_back(v5);
           curr.push_back(v3);
           curr.push_back(v4);
           curr.push_back(v1);
           curr.push_back(v2);
           curr.push_back(v6);
           
           //construct the octree
           Vector3D o = Vector3D(0, -1, -3);
           Vector3D s = Vector3D(2, 2, 6);

           OcTree tree = OcTree(o, s, 2);
           tree.populate_tree(curr.begin(), curr.end());

           Triangle* t = FindSeedTriangle(&tree, 1.0);
           Vertex* v = FindCandidate(t->ab, &tree, 1.0);
       }
}
