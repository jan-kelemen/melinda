#ifndef MELINDA_MBLCXX_SOPS_INCLUDED
#define MELINDA_MBLCXX_SOPS_INCLUDED

namespace melinda::mblcxx::detail
{
    struct noncopyable
    {
    public:
        noncopyable(noncopyable const&) = delete;
        noncopyable& operator=(noncopyable const&) = delete;

    protected:
        constexpr noncopyable() noexcept = default;
        ~noncopyable() noexcept = default;
    };

    struct none : public noncopyable
    {
    public:
        constexpr none() = delete;
        ~none() noexcept = delete;
    };
} // namespace melinda::mblcxx::detail
namespace melinda::mblcxx
{
    struct sops final : detail::none
    {
    public:
        using noncopyable = detail::noncopyable;
        using none = detail::none;
    };
} // namespace melinda::mblcxx

#endif
