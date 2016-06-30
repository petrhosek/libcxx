
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <unordered_map>

// Check that std::unordered_multimap and it's iterators can be instantiated with an incomplete
// type.

// XFAIL: gcc

#include <unordered_map>

template <class Tp>
struct MyHash {
  MyHash() {}
  std::size_t operator()(Tp const&) const {return 42;}
};

struct A {
    typedef std::unordered_multimap<A, A, MyHash<A> > Map;
    Map m;
    Map::iterator it;
    Map::const_iterator cit;
    Map::local_iterator lit;
    Map::const_local_iterator clit;
};

inline bool operator==(A const& L, A const& R) { return &L == &R; }

int main() {
    A a;
}
