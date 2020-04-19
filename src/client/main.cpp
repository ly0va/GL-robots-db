#include <string>
#include <iostream>
#include "client.h"

int main(int argc, char **argv) {
    std::string host, port;
    if (argc == 3) {
        host = argv[1];
        port = argv[2];
    } else {
        host = "localhost";
        port = "31111";
    }
    Client client(host, port);
    client.mainloop();
    return 0;
}
