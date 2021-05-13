#ifndef MELINDA_MBLCXX_SCOPE_EXIT_INCLUDED
#define MELINDA_MBLCXX_SCOPE_EXIT_INCLUDED

// https://www.youtube.com/watch?v=lKG1m2NkANM

// Provides a utility for execution actions when exiting the scope.

namespace melinda::mblcxx
{
    template<typename Action>
    class on_scope_exit
    {
    public: // Types
        using class_name = on_scope_exit;

    public: // Construction
        on_scope_exit() = delete;

        on_scope_exit(Action& action) noexcept : action(action) { }

        on_scope_exit(on_scope_exit const&) = delete;

        on_scope_exit(on_scope_exit&&) noexcept = delete;

    public: // Operators
        on_scope_exit& operator=(on_scope_exit const&) = delete;

        on_scope_exit& operator=(on_scope_exit&&) noexcept = delete;

    public: // Destruction
        ~on_scope_exit() { action(); }

    private: // Data
        Action& action;
    };

} // namespace melinda::mblcxx

#define MBLCXX_TOKEN_PASTEx(x, y) x##y
#define MBLCXX_TOKEN_PASTE(x, y) MBLCXX_TOKEN_PASTEx(x, y)

#define MBLCXX_ON_SCOPE_EXIT_INTERNAL1(lname, aname, ...) \
    auto lname = [&]() { __VA_ARGS__; }; \
    melinda::mblcxx::on_scope_exit<decltype(lname)> aname(lname);

#define MBLCXX_ON_SCOPE_EXIT_INTERNAL2(ctr, ...) \
    MBLCXX_ON_SCOPE_EXIT_INTERNAL1( \
        MBLCXX_TOKEN_PASTE(ON_SCOPE_EXIT_func_, ctr), \
        MBLCXX_TOKEN_PASTE(ON_SCOPE_EXIT_instance_, ctr), \
        __VA_ARGS__)

#define MBLCXX_ON_SCOPE_EXIT(...) \
    MBLCXX_ON_SCOPE_EXIT_INTERNAL2(__COUNTER__, __VA_ARGS__)

#endif
