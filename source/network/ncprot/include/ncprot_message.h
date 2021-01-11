#ifndef MELINDA_NETWORK_NCPROT_NCPROT_MESSAGE_INCLUDED
#define MELINDA_NETWORK_NCPROT_NCPROT_MESSAGE_INCLUDED

#include <cstddef>
#include <vector>

#include "iterator_traits.h"

namespace mel::ncprot
{
    struct [[nodiscard]] message final
    {
    public: // Construction
        message() = default;

        template<typename InputIterator>
        message(InputIterator begin, InputIterator end)
            requires(mel::cppex::input_iterator<InputIterator>)
            : data_(begin, end)
        {
        }

        message(message const&) = default;
        message(message&&) noexcept = default;

    public: // Interface
        [[nodiscard]] std::byte const* data() const noexcept;

        [[nodiscard]] std::size_t size() const noexcept;

    public: // Operators
        message& operator=(message const&) = default;
        message& operator=(message&&) noexcept = default;

    public: // Destruction
        ~message() noexcept = default;

    private: // Data
        std::vector<std::byte> data_;
    };
} // namespace mel::ncprot

#endif
