// Lab 9 - Task 3: Weighted M-ary Tree - Galaxy Representation
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;

class StarSystem;

// Edge of the tree: a travel route with a cost, leading to a child system
class TravelRoute {
public:
    float travel_cost;
    StarSystem* destination;
    TravelRoute(float cost, StarSystem* dest) : travel_cost(cost), destination(dest) {}
};

// Node of the tree
class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;
    vector<TravelRoute> routes;   // children (at most M)

    StarSystem(int id, const string& n, int planets,
               float px, float py, float pz, bool habitable)
        : system_id(id), name(n), num_planets(planets),
          x(px), y(py), z(pz), is_habitable(habitable) {}
};

class Galaxy {
    int M;
    StarSystem* root;

    // Returns subtree size, or -1 if any node in the subtree is unbalanced.
    // One pass over the tree -> O(n).
    int checkSize(StarSystem* x) const {
        if (!x) return 0;
        int total = 1;
        int minSize = 1e9, maxSize = -1;

        for (const TravelRoute& r : x->routes) {
            int s = checkSize(r.destination);
            if (s == -1) return -1;
            total += s;
            minSize = min(minSize, s);
            maxSize = max(maxSize, s);
        }
        // Empty child slots (fewer than M children) count as size 0
        if ((int)x->routes.size() < M && !x->routes.empty()) minSize = 0;

        if (!x->routes.empty() && maxSize - minSize > 1) return -1;
        return total;
    }

    void destroy(StarSystem* x) {
        if (!x) return;
        for (TravelRoute& r : x->routes) destroy(r.destination);
        delete x;
    }

    void print(StarSystem* x, int depth, float cost) const {
        if (!x) return;
        cout << string(depth * 4, ' ') << "- [" << x->system_id << "] " << x->name
             << " (planets: " << x->num_planets
             << ", habitable: " << (x->is_habitable ? "yes" : "no") << ")";
        if (depth > 0) cout << "  cost: " << cost;
        cout << endl;
        for (const TravelRoute& r : x->routes) print(r.destination, depth + 1, r.travel_cost);
    }

public:
    // Overloaded constructors: with M only, or with M and a root node
    Galaxy(int m) : M(m), root(nullptr) {}
    Galaxy(int m, StarSystem* r) : M(m), root(r) {}
    ~Galaxy() { destroy(root); }

    StarSystem* getRoot() const { return root; }

    // Add a child system under parent with a travel cost
    bool addRoute(StarSystem* parent, StarSystem* child, float cost) {
        if ((int)parent->routes.size() >= M) {
            cout << "Cannot add " << child->name << ": " << parent->name
                 << " already has " << M << " routes.\n";
            delete child;
            return false;
        }
        parent->routes.push_back(TravelRoute(cost, child));
        return true;
    }

    // Balanced if, for every node, child subtree sizes differ by at most 1
    bool is_balanced(StarSystem* x) const {
        return checkSize(x) != -1;
    }

    // Count leaf nodes (systems with no outgoing routes)
    int count_leaves(StarSystem* x) const {
        if (!x) return 0;
        if (x->routes.empty()) return 1;
        int leaves = 0;
        for (const TravelRoute& r : x->routes) leaves += count_leaves(r.destination);
        return leaves;
    }

    void display() const {
        cout << "Galaxy (M = " << M << "):\n";
        print(root, 0, 0);
    }
};

int main() {
    cout << "===== Task 3: Weighted M-ary Tree - Galaxy =====\n\n";

    // ---------- Galaxy 1: balanced, M = 3 ----------
    StarSystem* sol = new StarSystem(1, "Sol", 8, 0, 0, 0, true);
    Galaxy g1(3, sol);

    StarSystem* alpha = new StarSystem(2, "Alpha Centauri", 3, 4.3f, 1.2f, 0.5f, true);
    StarSystem* sirius = new StarSystem(3, "Sirius", 2, 8.6f, -2.1f, 1.0f, false);
    StarSystem* vega = new StarSystem(4, "Vega", 5, 25.0f, 3.3f, -4.2f, false);
    g1.addRoute(sol, alpha, 12.5f);
    g1.addRoute(sol, sirius, 20.0f);
    g1.addRoute(sol, vega, 35.75f);

    g1.addRoute(alpha, new StarSystem(5, "Proxima b", 1, 4.5f, 1.3f, 0.6f, true), 2.0f);
    g1.addRoute(sirius, new StarSystem(6, "Sirius B", 0, 8.7f, -2.0f, 1.1f, false), 1.5f);
    g1.addRoute(vega, new StarSystem(7, "Vega Prime", 2, 25.5f, 3.0f, -4.0f, true), 3.25f);

    g1.display();
    cout << "Is Balanced: " << (g1.is_balanced(g1.getRoot()) ? "Yes" : "No") << endl;
    cout << "Leaf Nodes:  " << g1.count_leaves(g1.getRoot()) << endl << endl;

    // ---------- Galaxy 2: unbalanced, M = 2 ----------
    StarSystem* andromeda = new StarSystem(10, "Andromeda Core", 12, 0, 0, 0, false);
    Galaxy g2(2, andromeda);

    StarSystem* a1 = new StarSystem(11, "M32", 4, 1, 2, 3, false);
    StarSystem* a2 = new StarSystem(12, "M110", 6, -1, 2, -3, true);
    g2.addRoute(andromeda, a1, 10.0f);
    g2.addRoute(andromeda, a2, 15.0f);

    StarSystem* a3 = new StarSystem(13, "Outpost 1", 1, 2, 3, 4, true);
    g2.addRoute(a1, a3, 5.0f);
    g2.addRoute(a1, new StarSystem(14, "Outpost 2", 2, 2, 4, 5, false), 6.0f);
    g2.addRoute(a3, new StarSystem(15, "Deep Station", 0, 3, 5, 6, false), 7.5f);
    g2.addRoute(a3, new StarSystem(16, "Far Reach", 1, 3, 6, 7, true), 8.0f);
    g2.addRoute(a1, new StarSystem(17, "Extra", 1, 0, 0, 0, false), 1.0f); // exceeds M

    g2.display();
    cout << "Is Balanced: " << (g2.is_balanced(g2.getRoot()) ? "Yes" : "No") << endl;
    cout << "Leaf Nodes:  " << g2.count_leaves(g2.getRoot()) << endl;

    return 0;
}