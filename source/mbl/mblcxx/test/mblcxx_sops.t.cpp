#include <mblcxx_sops.h>

#include <type_traits>

using namespace melinda;
using namespace mblcxx;

namespace
{
    struct inh_from_nondefconstructible final : sops::nondefconstructible
    {
    };
    static_assert(
        !std::is_default_constructible_v<inh_from_nondefconstructible>);

    struct inh_from_nondestructible final : sops::nondestructible
    {
    };
    static_assert(!std::is_destructible_v<inh_from_nondestructible>);

    struct inh_from_noncopyable final : sops::noncopyable
    {
    };
    static_assert(!std::is_copy_constructible_v<inh_from_noncopyable>);
    static_assert(!std::is_copy_assignable_v<inh_from_noncopyable>);

    struct inh_from_none final : sops::none
    {
    };
    static_assert(!std::is_default_constructible_v<inh_from_none>);
    static_assert(!std::is_destructible_v<inh_from_none>);
    static_assert(!std::is_copy_constructible_v<inh_from_none>);
    static_assert(!std::is_copy_assignable_v<inh_from_none>);
    static_assert(!std::is_move_constructible_v<inh_from_none>);
    static_assert(!std::is_move_assignable_v<inh_from_none>);
} // namespace
