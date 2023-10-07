#include <mdbsrv_options.h>

#include <iostream>
#include <string>
#include <utility>

#include <boost/program_options.hpp>

std::optional<melinda::mdbsrv::options> melinda::mdbsrv::parse_options(int argc,
    char** argv)
{
    namespace bpo = boost::program_options;

    // https://stackoverflow.com/a/14940678
    try
    {
        bpo::options_description cmd_options{"Allowed options"};
        cmd_options.add_options()("help", "produce help message")(
            "data-directory,D",
            bpo::value<std::string>()->required(),
            "data directory of the database server");

        bpo::variables_map vm;
        bpo::store(bpo::parse_command_line(argc, argv, cmd_options), vm);

        if (vm.count("help"))
        {
            std::cout << cmd_options << '\n';
            return std::nullopt;
        }

        bpo::notify(vm);

        std::filesystem::path data_directory;
        if (vm.count("data-directory"))
        {
            data_directory = vm["data-directory"].as<std::string>();
        }

        return {{std::move(data_directory)}};
    }
    catch (bpo::error const& ex)
    {
        std::cerr << "Failed start with given command line arguments: "
                  << ex.what() << '\n';
        return std::nullopt;
    }
}
