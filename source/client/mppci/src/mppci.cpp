#include <array>
#include <chrono>
#include <flatbuffers/flatbuffers.h>
#include <iostream>
#include <thread>
#include <vector>

#include <unistd.h>

#include <zmq.hpp>
#include <zmq_addon.hpp>

#include "mppci.h"
#include "ncprot_client.h"
#include "ncprot_message.h"
#include "scope_exit.h"
#include "trace.h"
#include "unused.h"
#include "wire_generated.h"

void mel::mppci::old_main()
{
    zmq::context_t ctx;
    constexpr char const* const address = "tcp://localhost:22365";

    mel::cppex::result<zmq::socket_t> connect_result =
        mel::ncprot::client::connect(ctx, address);
    if (!connect_result)
    {
        MEL_TRACE_FATAL("Can't connect to {}", address);
        std::terminate();
    }

    // TODO-JK allow taking values out of result
    zmq::socket_t& socket = connect_result.ok();
    ON_SCOPE_EXIT(socket.disconnect(address));

    char identity[256];
    std::size_t l = 256;
    socket.getsockopt(ZMQ_ROUTING_ID, identity, &l);

    flatbuffers::FlatBufferBuilder const query =
        mel::ncprot::serialize_query(identity, "SELECT * FROM v$sql");

    while (true)
    {
        auto send_result = mel::ncprot::client::send(socket,
            {reinterpret_cast<std::byte*>(query.GetBufferPointer()),
                query.GetSize()});

        if (send_result.index() == 0)
        {
            if (!std::get<zmq::send_result_t>(send_result))
            {
                continue;
            }
        }

        auto recv_result = mel::ncprot::client::recv(
            socket); // TODO-JK: This is blocking indefinately
        if (recv_result.index() == 0)
        {
            auto const& result =
                std::get<std::pair<zmq::recv_result_t, zmq::message_t>>(
                    recv_result);
            if (result.first)
            {
                mel::network::Message const* message =
                    flatbuffers::GetRoot<mel::ncprot::root_type>(
                        result.second.data());

                if (message->content_type() ==
                    mel::network::MessageContent_result)
                {
                    mel::network::QueryResult const* query_result =
                        message->content_as_result();
                    MEL_TRACE_INFO("Returned {} rows.", query_result->length());
                }
            }
        }
    }
}
