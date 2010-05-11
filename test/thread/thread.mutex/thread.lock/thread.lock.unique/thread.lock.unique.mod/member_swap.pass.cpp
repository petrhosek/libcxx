//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <mutex>

// template <class Mutex> class unique_lock;

// void swap(unique_lock& u);

#include <mutex>
#include <cassert>

struct mutex
{
    void lock() {}
    void unlock() {}
};

mutex m;

int main()
{
    std::unique_lock<mutex> lk1(m);
    std::unique_lock<mutex> lk2;
    lk1.swap(lk2);
    assert(lk1.mutex() == nullptr);
    assert(lk1.owns_lock() == false);
    assert(lk2.mutex() == &m);
    assert(lk2.owns_lock() == true);
}
