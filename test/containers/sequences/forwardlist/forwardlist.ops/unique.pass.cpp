//===----------------------------------------------------------------------===//
//
// ΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚΚThe LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <forward_list>

// void unique();

#include <forward_list>
#include <iterator>
#include <cassert>

int main()
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {0, 5, 0, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.unique();
        assert(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 0, 0, 0};
        const T t2[] = {0};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.unique();
        assert(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.unique();
        assert(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c1;
        C c2;
        c1.unique();
        assert(c1 == c2);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        c1.unique();
        assert(c1 == c2);
    }
}
