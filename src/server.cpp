#include "database.h"
#include <iostream>

int main() {
    Database db("offsets.db", "robots.db");
    Entry e;
    std::cout << "TOTAL: " <<  db.total_entries << '\n';
    Robot r = {123, 3.14, "Bender"};
    db.add(r);
    r = db.find(0).robot;
    std::cout << "Name: " << r.name << "\nPrice: " << r.price << "\nWeight: " << r.weight << '\n';
    r.name = "R2D2";
    r.price = 100500;
    r.weight = 2.71828;
    db.add(r);
    Robot q = db.find(1).robot;
    std::cout << "Name: " << q.name << "\nPrice: " << q.price << "\nWeight: " << q.weight << '\n';
    return 0;
}
