#pragma once

#include <type_traits>
#include <cstring>
#include <typeinfo>
#include <typeindex>
#include <new>

//yes, this container sucks.
//this is only for use for myself so idc.

//probably just vector<std::any> would be better.

//if getting double free error, then add a move constructor to custom class
struct ArrayAny {

    ArrayAny() = delete;
    ArrayAny(const ArrayAny& other) = delete;
    ArrayAny& operator=(const ArrayAny& other) = delete;

    ArrayAny(ArrayAny&& other) {

        this->block = other.block;
        this->numElems = other.numElems;
        this->deallocators = other.deallocators;
        this->types = other.types;

        other.deallocators = nullptr;
        other.types = nullptr;
        other.numElems = 0;
        other.block = nullptr;
 
    }
    template <typename... TArgs>
    constexpr ArrayAny(const TArgs&... args) {
        static_assert((std::is_copy_constructible<TArgs>::value && ...), "args must be copy constructable");

        numElems = sizeof...(args);

        deallocators = (void (**)(void*))malloc(sizeof(void (*)(void*)) * numElems);
        types = (std::type_index*)malloc(sizeof(std::type_index) * numElems);
        void** temp = (void**)malloc(sizeof(void*) * numElems);

        int index = 0;
        (this->alloc(temp, args, index), ...);

        block = temp;
    }
    template <typename... TArgs> constexpr ArrayAny(TArgs&&... args) {
        
        static_assert((std::is_move_constructible<TArgs>::value && ...), "args must be move constructable");

        numElems = sizeof...(args);

        deallocators = (void (**)(void*))malloc(sizeof(void (*)(void*)) * numElems);
        types = (std::type_index*)malloc(sizeof(std::type_index) * numElems);
        void** temp = (void**)malloc(sizeof(void*) * numElems);

        int index = 0;
        (this->alloc(temp, std::forward<TArgs>(args), index), ...);

        block = temp;

    }

    constexpr const void* const* getBlock() const {
        return const_cast<const void* const*>(block);
    }

    template <int Index>
    constexpr const void* const at() const {

        if (Index >= numElems) {return nullptr;}
        return const_cast<const void* const>(block[Index]);
    }
    
    template <int Index, typename T>
    constexpr const T& get() const {
        
        if (std::type_index(typeid(T)) != types[Index]) throw std::bad_cast();
        return *(T*)this->at<Index>();
        
    }

    template <int Index, typename T>
    void set(const T& val) const {
        
        if (std::type_index(typeid(T)) != types[Index]) throw std::bad_cast();
        *((T*)block[Index]) = val;
        
        
    }

    ~ArrayAny() {
        for (unsigned int i = 0; i < numElems; ++i) {
            (*deallocators[i])(block[i]);
        }

        free(block);
        free(types);
        free(deallocators);
    }


private:

    void** block;
    std::type_index* types;
    unsigned int numElems;
    void (**deallocators)(void*);

    template <typename T> constexpr void alloc(void** mem, T&& arg,  int& index) noexcept {

        mem[index] = new T(std::move(arg));
        deallocators[index] = [](void* ptr) {
            delete static_cast<T*>(ptr);
        };
        types[index] = std::type_index(typeid(T));
        index++;

    }


};
