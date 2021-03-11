#ifndef MELINDA_NETWQRK_NCPROT_NCPROT_SERIALIZATION_INCLUDED
#define MELINDA_NETWQRK_NCPROT_NCPROT_SERIALIZATION_INCLUDED

#include <cstdint>
#include <span>
#include <string>

#include <flatbuffers/flatbuffers.h>

#include "wire_generated.h"

namespace mel::ncprot
{
    using root_type = mel::network::Message;

    flatbuffers::FlatBufferBuilder serialize_query(std::string const& identity,
        std::string const& query);

    flatbuffers::FlatBufferBuilder serialize_query_result(
        std::string const& identity,
        std::uint32_t length,
        std::span<uint32_t> const& offsets,
        std::span<std::byte> const& data);
} // namespace mel::ncprot

#endif
