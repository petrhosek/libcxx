//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <string>

// void clear();

#include <string>
#include <cassert>

template <class S>
void
test(S s)
{
    s.clear();
    assert(s.size() == 0);
}

int main()
{
    typedef std::string S;
    S s;
    test(s);

    s.assign(10, 'a');
    s.erase(5);
    test(s);

    s.assign(100, 'a');
    s.erase(50);
    test(s);
}
