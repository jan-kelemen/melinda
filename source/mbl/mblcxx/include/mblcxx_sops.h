#ifndef MELINDA_MBLCXX_SOPS_INCLUDED
#define MELINDA_MBLCXX_SOPS_INCLUDED

namespace melinda::mblcxx::detail
{
    struct nondefconstructible
    {
    public:
        nondefconstructible() = delete;
    };

    struct nondestructible
    {
    public:
        ~nondestructible() = delete;
    };

    struct noncopyable
    {
    public:
        noncopyable(noncopyable const&) = delete;

    public:
        noncopyable& operator=(noncopyable const&) = delete;
    };

    struct none
        : nondefconstructible
        , nondestructible
        , noncopyable
    {
    };
} // namespace melinda::mblcxx::detail
namespace melinda::mblcxx
{
    struct sops final : detail::none
    {
    public:
        using nondefconstructible = detail::nondestructible;
        using nondestructible = detail::nondestructible;
        using noncopyable = detail::noncopyable;
        using none = detail::none;
    };
} // namespace melinda::mblcxx

#endif
