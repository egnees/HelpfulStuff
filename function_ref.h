#pragma once

#include <utility>
#include <functional>
#include <type_traits>
#include <memory>

template <typename Signature>
class FunctionRef;

template <typename Return, typename... Args>
class FunctionRef<Return(Args...)> {
    using Storage = std::aligned_union_t<sizeof(void*), Return (*)(Args...)>;
    using Callback = Return (*)(const void*, Args...);

    Storage storage_;
    Callback cb_;

    void* GetMemory() noexcept {
        return &storage_;
    }

    const void* GetMemory() const noexcept {
        return &storage_;
    }

public:
    template <class ReturnTp>
    FunctionRef(ReturnTp (*fptr)(Args...)) {
        using PointerType = Return (*)(Args...);
        ::new (GetMemory()) PointerType(reinterpret_cast<PointerType>(*fptr));

        cb_ = [](const void* memory, Args... args) {
            auto func = *static_cast<const PointerType*>(memory);
            return static_cast<Return>(func(std::forward<Args>(args)...));
        };
    }

    template <typename Functor>
    FunctionRef(Functor&& f)
        : cb_([](const void* memory, Args... args) {
              using PtrT = void*;
              auto ptr = *static_cast<const PtrT*>(memory);
              auto& func = *static_cast<Functor*>(ptr);
              return static_cast<Return>(func(std::forward<Args>(args)...));
          }) {
        ::new (GetMemory()) void*(&f);
    }

    template <typename Functor>
    FunctionRef(Functor& f)
        : cb_([](const void* memory, Args... args) {
              using PtrT = void*;
              auto ptr = *static_cast<const PtrT*>(memory);
              auto& func = *reinterpret_cast<Functor*>(ptr);
              return static_cast<Return>(func(std::forward<Args>(args)...));
          }) {
        ::new (GetMemory()) void*(&f);
    }

    FunctionRef& operator=(Return (*fptr)(Args...)) {
        using PointerType = Return (*)(Args...);
        ::new (GetMemory()) PointerType(fptr);

        cb_ = [](const void* memory, Args... args) {
            auto func = *static_cast<const PointerType*>(memory);
            return func(std::forward<Args>(args)...);
        };

        return *this;
    }

    template <typename Functor>
    FunctionRef& operator=(Functor&& f) {
        cb_ = [](const void* memory, Args... args) {
            using PtrT = void*;
            auto ptr = *static_cast<const PtrT*>(memory);
            auto& func = *static_cast<Functor*>(ptr);
            return static_cast<Return>(func(std::forward<Args>(args)...));
        };

        ::new (GetMemory()) void*(&f);

        return *this;
    }

    template <typename Functor>
    FunctionRef& operator=(Functor& f) {
        cb_ = [](const void* memory, Args... args) {
            using PtrT = void*;
            auto ptr = *static_cast<const PtrT*>(memory);
            auto& func = *static_cast<Functor*>(ptr);
            return static_cast<Return>(func(std::forward<Args>(args)...));
        };

        ::new (GetMemory()) void*(&f);

        return *this;
    }

    FunctionRef(Return (*fptr)(Args... args)) {
        using PointerType = Return (*)(Args... args);
        ::new (GetMemory()) PointerType(fptr);

        cb_ = [](const void* memory, Args... args) {
            auto func = *static_cast<const PointerType*>(memory);
            return func(std::forward<Args>(args)...);
        };
    }

    Return operator()(Args... args) const {
        return cb_(GetMemory(), std::forward<Args>(args)...);
    }
};
