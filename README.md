# Simple Robots Database

Simple one-table remote database that can store robots. Made during GlobalLogic C++ practice.

## Installation

**Dependencies:** `libjsoncpp`, `libzmq`

On Ubuntu:

```bash
apt-get install libjsoncpp-dev libzmq3-dev
```

On Arch:

```bash
pacman -S jsoncpp zeromq
```

After installing dependencies, run:

```bash
git clone https://github.com/lyova-potyomkin/GL-robots-db.git
cd GL-robots-db
make
```

## Usage

### Server

To start, run `./server [port]`. Default port is `31111`.

### Client

To use, run `./client [host] [port]`. Default host is `localhost`, default port is `31111`.

Using client, you can add/remove/update/find robots in the database. Robot is defined as:

```cpp
struct Robot {
    int price;
    float weight;
    std::string name;
};

// Examples
Robot bumblebee = {100500, 3.1415, "Bumblebee"};
Robot bender = {18, 2.71828, "Bender"};
```

## Efficiency

Let S be the size of the entry, and T be the total size of the database.
Then, time efficiency of the operations can be expressed as

| Add    | Delete | Update | Find by id | Find by field |
| ------ | ------ | ------ | ---------- | ------------- |
| `O(S)` | `O(1)` | `O(S)` | `O(S)`     | `O(T)`        |

As per space, whole database is stored in 2 files (instead of 1 file per entry), 
so there is practically no overhead due to file system block sizes.

## Features

- Basic database operations
- Time and space efficient
- Server can handle multiple clients at the same time
- Endianness-agnostic serialization
- Crossplatform
