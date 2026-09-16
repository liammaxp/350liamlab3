#include <cstdlib>
#include <cassert>
#include <utility>
#include "SharedPtr.h"

int main() {
   SharedPtr<int> test;
   assert(test.get() == nullptr);
   assert(test.useCount() == 0);
   assert(!test);
   SharedPtr<int> a(new int(5));
   assert(*a == 5);
   assert(a.useCount() == 1);
   assert(a);
   SharedPtr<int> b(a);
   assert(a == b);
   assert(a.useCount() == 2);
   assert(b.useCount() == 2);

   SharedPtr<int> c(std::move(b));
   assert(*c == 5);
   assert(b.get() == nullptr);
   assert(b.useCount() == 0);
   assert(c.useCount() == 2);

   SharedPtr<int> d(new int(10));
   d = a;
   assert(d == a);
   assert(a.useCount() == 3);
   assert(d.useCount() == 3);

   SharedPtr<int> e(new int(20));
   e = std::move(d);

   assert(*e == 5);
   assert(d.get() == nullptr);
   assert(d.useCount() == 0);

   *e = 15;
   assert(*a == 15);
   assert(*c == 15);
   int* raw = a.get();
   assert(raw != nullptr);
   assert(*raw == 15);
   SharedPtr<int> x(new int(1));
   SharedPtr<int> y(new int(2));
   x.swap(y);
   assert(*x == 2);
   assert(*y == 1);

   SharedPtr<int> r(new int(50));
   r.reset();
   assert(r.get() == nullptr);
   assert(r.useCount() == 0);

   SharedPtr<int> r2(new int(60));
   r2.reset(new int(70));
   assert(*r2 == 70);
   assert(r2.useCount() == 1);

   int* samePtr = r2.get();
   r2.reset(r2.get());
   assert(r2.get() == samePtr);
   assert(*r2 == 70);

   SharedPtr<int> basic = makeSharedBasic<int>(123);
   assert(*basic == 123);
   assert(basic.useCount() == 1);
   return EXIT_SUCCESS;
}
