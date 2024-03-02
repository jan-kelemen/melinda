#include <mdbxtr_execute.hpp>

#include <mblcxx_result.hpp>
#include <mqlast_from_parse_tree.hpp>
#include <mqlast_syntax_tree.hpp>
#include <mqlprs_parse.hpp>

#include <lexy/parse_tree.hpp>

#include <filesystem>
#include <variant>

// IWYU pragma: no_include <lexy/input/base.hpp>
// IWYU pragma: no_include <lexy/_detail/iterator.hpp>
// IWYU pragma: no_include <ranges>
// IWYU pragma: no_include <vector>

melinda::mblcxx::result<std::uint64_t> melinda::mdbxtr::execute(
    std::string_view sql_text)
{
    auto const parse_tree_result{mqlprs::parse(sql_text)};
    if (!parse_tree_result)
    {
        return mblcxx::error<std::error_code>{
            std::make_error_code(std::errc::invalid_argument)};
    }

    auto const ast_result{mqlast::from_parse_tree(*parse_tree_result)};
    if (!ast_result)
    {
        return mblcxx::error<std::error_code>{
            std::make_error_code(std::errc::invalid_argument)};
    }

    std::uint64_t executed_commands{};
    for (mqlast::command const& command : ast_result->commands)
    {
        if (auto const* create_schema{
                std::get_if<mqlast::create_schema_command>(&command)})
        {
            auto const schema_directory{
                std::filesystem::current_path() / create_schema->schema_name};

            std::error_code ec;
            if (!create_directory(schema_directory, ec))
            {
                if (ec)
                {
                    return mblcxx::error<std::error_code>{ec};
                }
                return mblcxx::error<std::error_code>(
                    std::make_error_code(std::errc::file_exists));
            }
            ++executed_commands;
        }
        else
        {
            return mblcxx::error<std::error_code>(
                std::make_error_code(std::errc::not_supported));
        }
    }

    return executed_commands;
}
