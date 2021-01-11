#include "ncprot_message.h"

std::byte const* mel::ncprot::message::data() const noexcept
{
    return data_.data();
}

std::size_t mel::ncprot::message::size() const noexcept { return data_.size(); }
