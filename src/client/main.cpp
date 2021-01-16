#include <string>
#include "client.h"

int main(int argc, char **argv) {
    std::string host = "localhost";
    std::string port = "31111";
    if (argc == 3) {
        host = argv[1];
        port = argv[2];
    }
    Client client(host, port);
    client.mainloop();
    return 0;
}
