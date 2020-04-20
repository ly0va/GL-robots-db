#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <zmq.hpp>
#include <json/json.h>

class Client {
private:
    zmq::context_t context;
    zmq::socket_t socket;
    Json::FastWriter writer;
    Json::Reader reader;
    Json::Value send_recv(const std::string& request);
public:
    Client(const std::string& host, const std::string& port);
    void mainloop();
    void ping();
    void add();
    void update();
    void remove();
    void find();
    void find_all();
};

#endif
