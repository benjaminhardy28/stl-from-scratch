#include <cstddef>
#include <new>
#include <utility>

namespace mystl{

template <typename T>
class allocator{
public:
    using value_type = T;

    allocator() = default;

    // used to allocate memory without calling object constructor
    T* allocate(std::size_t n){
        if(n == 0) return nullptr;
        // does not call constructor. ::operator new(...) only allocates
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, std::size_t n){
        if(p == nullptr) return;

        ::operator delete(p, n * sizeof(T));
    }

    // used to construct object in already owned memory
    template <typename... Args>
    void construct(T* p, Args&&... args){
        // placement new, variation of new that constructs object in pre-allocated memory
        // syntax: new (memory addres) Type(arguments)
        new(p) T(std::forward<Args>(args)...);
     }

    void destroy(T* p){
        p->~T();
    }

};
}
