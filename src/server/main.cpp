#include "database.h"
#include "connection.h"
#include <iostream>
#include <zmq.hpp>

int main(int argc, char **argv) {
    std::string port = "31111";
    if (argc == 2) {
        port = argv[1];
    }
    Database db("offsets.db", "robots.db");
    db.add({123, 3.1415, "Bender"});
    db.add({100500, 2.71828, "Wall-e"});
    DBConnection db_connection(db);
    zmq::context_t context(1);
    zmq::socket_t socket(context, ZMQ_REP);
    socket.bind("tcp://*:" + port);
    std::clog << "Started server on port " << port << '\n';
    while (true) {
        zmq::message_t req;
        socket.recv(&req);
        std::string req_str(static_cast<char*>(req.data()), req.size());
        std::clog << req_str;
        std::string rep_str = db_connection.process(req_str);
        zmq::message_t rep(rep_str.size());
        memcpy(rep.data(), rep_str.c_str(), rep_str.size());
        socket.send(rep);
    }
    return 0;
}
