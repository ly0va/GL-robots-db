#!/usr/bin/python3

import zmq
import random
import string
import sys
import time

def random_add(num_entries):
    length = random.randint(5, 15)
    name = random.sample(string.ascii_letters, length)
    return {
        "command": "add",
        "arg": {
            "name": ''.join(name), 
            "price": random.randint(1, 10**9), 
            "weight": random.random()
        }
    }

def random_find(num_entries):
    return {
        "command": "find",
        "arg": random.randint(0, num_entries*2)
    }

def random_remove(num_entries):
    return {
        "command": "remove",
        "arg": random.randint(0, num_entries)
    }

def bench(cmd, num_entries, num_iters):
    start = time.time()
    for _ in range(num_iters):
        socket.send_json(cmd(num_entries))
        socket.recv_json()
    duration = time.time() - start
    print("Finished in", round(duration, 3), "sec\n")


if __name__ == '__main__':

    if len(sys.argv) < 2:
        print("USAGE: ./bench.py num_entries")
        exit(1)
    else:
        num_entries = int(sys.argv[1])

    random.seed(18)
    ctx = zmq.Context()
    socket = ctx.socket(zmq.REQ)
    socket.setsockopt(zmq.SNDTIMEO, 5000)
    socket.setsockopt(zmq.RCVTIMEO, 5000)
    socket.setsockopt(zmq.LINGER, 0)
    socket.connect("tcp://localhost:31111")
    try:
        socket.send_json({"command": "ping", "arg": None})
        socket.recv_json()
    except Exception:
        print("Connection timed out")
        exit(1)

    print("Adding", num_entries, "random entries")
    bench(random_add, num_entries, num_entries)

    print("Trying to find", num_entries, "entries")
    bench(random_find, num_entries, num_entries)

    print("Trying to remove", num_entries//2, "entries")
    bench(random_remove, num_entries, num_entries//2)
