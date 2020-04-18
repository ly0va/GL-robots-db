#include "database.h"
#include <iostream>

using namespace std;

void print(ostream& os, const Entry& e) {
    os << "  ID: " << e.id;
    os << "\n  NAME:   " << e.robot.name;
    os << "\n  PRICE:  " << e.robot.price;
    os << "\n  WEIGHT: " << e.robot.weight;
    os << "\n\n";
}

int main() {
    Database db("offsets.db", "robots.db");
    db.add({123, 3.1415, "Bender"});
    db.add({100500, 2.71828, "R2D2"});
    db.add({1818, 1.4142, "BMO, the greatest and the sweetest"});
    db.add({123, 1.618, "Wall-e"});
    db.add({1000000007, 2e6, "Optimus Prime"});
    /* Entry e; */
    /* cout << db.find(2) << '\n'; */
    /* cout << db.find(4) << '\n'; */
    db.update(1, {123, 2.71828, "C-3PO"});
    db.add({123, 6.28, "Bumblebee"});
    db.remove(0);
    /* db.remove(1); */
    for (auto e: db.find_all([](const Robot& r) {
        return r.price == 123;
    })) {
        print(cout, e);
    }
    return 0;
}
