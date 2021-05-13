#ifndef MELINDA_MDBNET_SERIALIZATION_INCLUDED
#define MELINDA_MDBNET_SERIALIZATION_INCLUDED

#include <cstdint>
#include <span>
#include <string>

#include <flatbuffers/flatbuffers.h>

#include <mblcxx_sops.h>

#include <mdbnet_wire_generated.h>

namespace melinda::mdbnet
{
    using root_type = melinda::mdbnet::Message;

    struct serialization final : public mblcxx::sops::none
    {
    public:
        static flatbuffers::FlatBufferBuilder query(std::string const& identity,
            std::string const& query);

        static flatbuffers::FlatBufferBuilder query_result(
            std::string const& identity,
            std::uint32_t length,
            std::span<uint32_t> const& offsets,
            std::span<std::byte> const& data);
    };

} // namespace melinda::mdbnet

#endif
