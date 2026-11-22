#include <mystl/allocator.hpp>
#include <cassert>
#include <iostream>

struct Tracer {
    static inline int constructed = 0;
    static inline int destroyed = 0;
    int value;

    explicit Tracer(int v) : value(v) { ++constructed; }
    ~Tracer() { ++destroyed; }
};

void test_allocate_deallocate_basic() {
    mystl::allocator<int> alloc;

    int* p = alloc.allocate(5);
    assert(p != nullptr);

    for (int i = 0; i < 5; ++i) p[i] = i;
    for (int i = 0; i < 5; ++i) assert(p[i] == i);

    alloc.deallocate(p, 5);
}

void test_allocate_zero_returns_nullptr() {
    mystl::allocator<int> alloc;
    assert(alloc.allocate(0) == nullptr);
}

void test_deallocate_nullptr_is_noop() {
    mystl::allocator<int> alloc;
    alloc.deallocate(nullptr, 0); 
}

void test_construct_runs_constructor() {
    mystl::allocator<Tracer> alloc;
    Tracer* p = alloc.allocate(1);

    int before = Tracer::constructed;
    alloc.construct(p, 42);

    assert(Tracer::constructed == before + 1);
    assert(p->value == 42);

    alloc.destroy(p);
    alloc.deallocate(p, 1);
}

void test_destroy_runs_destructor() {
    mystl::allocator<Tracer> alloc;
    Tracer* p = alloc.allocate(1);
    alloc.construct(p, 1);

    int before = Tracer::destroyed;
    alloc.destroy(p);

    assert(Tracer::destroyed == before + 1);
    alloc.deallocate(p, 1);
}

void test_construct_forwards_arguments() {
    struct Pair {
        int a; int b;
        Pair(int a_, int b_) : a(a_), b(b_) {}
    };
    mystl::allocator<Pair> alloc;
    Pair* p = alloc.allocate(1);
    alloc.construct(p, 3, 4);

    assert(p->a == 3 && p->b == 4);

    alloc.destroy(p);
    alloc.deallocate(p, 1);
}

int main() {
    test_allocate_deallocate_basic();
    test_allocate_zero_returns_nullptr();
    test_deallocate_nullptr_is_noop();
    test_construct_runs_constructor();
    test_destroy_runs_destructor();
    test_construct_forwards_arguments();

    std::cout << "All allocator tests passed.\n";
    return 0;
}
