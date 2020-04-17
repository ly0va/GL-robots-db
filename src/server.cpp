#include "database.h"
#include <iostream>

int main() {
    Database db("offsets.db", "robots.db");
    /* std::cout << db.get_offset(0) << '\n'; */
    Robot r = {0, 1, 123, 3.14, "Bender"};
    db.add(r);
    Robot t = db.find(1);
    std::cout << "Name: " << t.name << "\nPrice: " << t.price << "\nWeight: " << t.weight << '\n';
    return 0;
}
