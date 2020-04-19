#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <string>
#include <zmq.hpp>

class Client {
private:
    zmq::context_t context;
    zmq::socket_t socket;
public:
    Client(std::string host, std::string port);
    void mainloop();
    void ping();
    void add();
    void update();
    void remove();
    void find();
    void find_all();
};

#endif
