#ifndef MELINDA_MDBNET_SERIALIZATION_INCLUDED
#define MELINDA_MDBNET_SERIALIZATION_INCLUDED

#include <cstdint>
#include <span>
#include <string>

#include <flatbuffers/flatbuffers.h>

#include <mdbnet_wire_generated.h>

namespace melinda::mdbnet
{
using root_type = melinda::mdbnet::Message;

struct serialization final
{
public:
    static flatbuffers::FlatBufferBuilder query(std::string const& identity,
        std::string const& query);

    static flatbuffers::FlatBufferBuilder query_result(
        std::string const& identity,
        std::uint32_t length,
        std::span<uint32_t> const& offsets,
        std::span<std::byte> const& data);

public:
    serialization() = delete;

    serialization(serialization const&) = delete;

    serialization(serialization&&) noexcept = delete;

public:
    ~serialization() = delete;

public:
    serialization& operator=(serialization const&) = delete;

    serialization& operator=(serialization&&) noexcept = delete;
};

} // namespace melinda::mdbnet

#endif
