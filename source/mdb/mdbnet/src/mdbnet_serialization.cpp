#include <mdbnet_serialization.h>

flatbuffers::FlatBufferBuilder melinda::mdbnet::serialization::query(
    std::string const& identity,
    std::string const& query)
{
    flatbuffers::FlatBufferBuilder raw_builder;

    auto const client_off = raw_builder.CreateString(identity);

    auto const query_off = CreateQueryDirect(raw_builder, query.c_str());

    MessageBuilder message_builder(raw_builder);

    message_builder.add_content_type(MessageContent_query);
    message_builder.add_content(query_off.Union());
    message_builder.add_client(client_off);

    raw_builder.Finish(message_builder.Finish());

    return raw_builder;
}

flatbuffers::FlatBufferBuilder melinda::mdbnet::serialization::query_result(
    std::string const& identity,
    std::uint32_t length,
    std::span<uint32_t> const& offsets,
    std::span<std::byte> const& data)
{
    flatbuffers::FlatBufferBuilder raw_builder;

    auto const client_off = raw_builder.CreateString(identity);
    auto const offsets_off =
        raw_builder.CreateVector(offsets.data(), offsets.size());
    static_assert(sizeof(uint8_t) == sizeof(std::byte),
        "flatbuffers use uint8_t as a byte representation");
    auto const raw_values_off =
        raw_builder.CreateVector(reinterpret_cast<uint8_t const*>(data.data()),
            data.size());

    QueryResultBuilder query_result_builder(raw_builder);

    query_result_builder.add_length(length);
    query_result_builder.add_offsets(offsets_off);
    query_result_builder.add_raw_values(raw_values_off);

    auto const query_off = query_result_builder.Finish();

    MessageBuilder message_builder(raw_builder);

    message_builder.add_content_type(MessageContent_result);
    message_builder.add_content(query_off.Union());
    message_builder.add_client(client_off);

    raw_builder.Finish(message_builder.Finish());

    return raw_builder;
}
