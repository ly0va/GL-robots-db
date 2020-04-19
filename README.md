# Simple Robots Database

Simple one-table database that can store robots. Made during GlobalLogic C++ practice.

## Installation

**Dependencies:** `libjsoncpp`, `libzmq`

On Ubuntu:

```bash
sudo apt-get install libjsoncpp-dev libzmq3-dev
```

On Arch:

```bash
sudo pacman -S jsoncpp zeromq 
```

After installing dependencies, run:

```bash
git clone https://github.com/lyova-potyomkin/GL-robots-db.git
cd GL-robots-db
make
```

## Usage

TODO


## Efficiency

Let S be the size of the entry, and T be the total size of the Database. 
Then, time efficiency of the operations can be expressed as

| Add  | Delete | Update | Find by id | Find by field |
| ---- | ------ | ------ | ---------- | ------------- |
| O(S) | O(1)   | O(S)   | O(S)       | O(T)          |

As per space, whole database is stored in 2 files (instead of 1 file per entry), 
so there is practically no overhead due to file system block sizes.

