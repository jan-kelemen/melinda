#include <mdbsql_parser.h>

#include <cstdint>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include <fmt/core.h>
#include <fmt/format.h>

#include <lexy/action/parse.hpp>
#include <lexy/callback/container.hpp>
#include <lexy/callback/fold.hpp>
#include <lexy/callback/object.hpp>
#include <lexy/dsl/brackets.hpp>
#include <lexy/dsl/branch.hpp>
#include <lexy/dsl/case_folding.hpp>
#include <lexy/dsl/choice.hpp>
#include <lexy/dsl/eof.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/list.hpp>
#include <lexy/dsl/literal.hpp>
#include <lexy/dsl/option.hpp>
#include <lexy/dsl/production.hpp>
#include <lexy/dsl/punctuator.hpp>
#include <lexy/dsl/separator.hpp>
#include <lexy/dsl/sequence.hpp>
#include <lexy/dsl/symbol.hpp>
#include <lexy/encoding.hpp>
#include <lexy/grammar.hpp>
#include <lexy/input/string_input.hpp>
// IWYU pragma: no_include <lexy/_detail/invoke.hpp>

#include <mbltrc_trace.h>

#include <mdbsql_ast_column_definition.h>
#include <mdbsql_ast_delimited_identifier.h>
#include <mdbsql_ast_schema_definition.h> // IWYU pragma: keep
#include <mdbsql_ast_sql_executable_statement.h>
#include <mdbsql_ast_sql_schema_statement.h>
#include <mdbsql_ast_table_definition.h> // IWYU pragma: keep
#include <mdbsql_ast_table_elements.h>
#include <mdbsql_parser_common.h>
#include <mdbsql_parser_identifier.h> // IWYU pragma: keep
#include <mdbsql_parser_parse_error.h>
#include <mdbsql_parser_reserved_word.h>

namespace
{
    namespace ast = melinda::mdbsql::ast;
    namespace parser = melinda::mdbsql::parser;
    namespace dsl = lexy::dsl;

    [[maybe_unused]] constexpr auto nrkw_a =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"a", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_absolute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"absolute", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_action =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"action", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_ada =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ada", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_add =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"add", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_admin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"admin", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_after =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"after", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_always =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"always", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_asc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"asc", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_assertion =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"assertion", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_assignment =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"assignment", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_attribute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"attribute", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_attributes =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"attributes", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_before =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"before", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_bernoulli =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"bernoulli", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_breadth =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"breadth", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_c =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"c", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_cascade =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cascade", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_catalog_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"catalog_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_chain =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"chain", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_characteristics =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"characteristics", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_characters =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"characters", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_character_set_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"character_set_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_character_set_name =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"character_set_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_character_set_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"character_set_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_class_origin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"class_origin", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_cobol =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cobol", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_collation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collation", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_collation_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"collation_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_collation_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collation_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_collation_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"collation_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_column_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"column_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_command_function =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"command_function", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_command_function_code =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"command_function_code", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_committed =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"committed", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_condition_number =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"condition_number", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_connection =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"connection", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_connection_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"connection_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_constraints =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraints", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_constraint_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"constraint_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_constraint_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraint_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_constraint_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"constraint_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_constructor =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constructor", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_continue =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"continue", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_cursor_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cursor_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_data =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"data", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_datetime_interval_code =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"datetime_interval_code", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_datetime_interval_precision =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"datetime_interval_precision", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_defaults =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"defaults", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_deferrable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deferrable", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_deferred =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deferred", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_defined =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"defined", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_definer =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"definer", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_degree =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"degree", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_depth =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"depth", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_derived =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"derived", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_desc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"desc", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_descriptor =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"descriptor", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_diagnostics =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"diagnostics", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_dispatch =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"dispatch", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_domain =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"domain", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_dynamic_function =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"dynamic_function", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_dynamic_function_code =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"dynamic_function_code", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_enforced =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"enforced", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_exclude =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"exclude", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_excluding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"excluding", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_expression =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"expression", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_final =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"final", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_first =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"first", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_flag =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"flag", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_following =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"following", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_fortran =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"fortran", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_found =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"found", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_g =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"g", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_general =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"general", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_generated =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"generated", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_go =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"go", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_goto =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"goto", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_granted =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"granted", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_hierarchy =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"hierarchy", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_ignore =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ignore", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_immediate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"immediate", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_immediately =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"immediately", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_implementation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"implementation", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_including =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"including", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_increment =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"increment", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_initially =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"initially", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_input =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"input", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_instance =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"instance", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_instantiable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"instantiable", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_instead =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"instead", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_invoker =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"invoker", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_isolation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"isolation", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_k =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"k", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_key =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"key", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_key_member =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"key_member", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_key_type =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"key_type", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_last =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"last", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"length", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_level =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"level", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_locator =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"locator", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_m =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"m", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_map =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"map", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_matched =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"matched", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_maxvalue =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"maxvalue", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_message_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"message_length", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_message_octet_length =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"message_octet_length", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_message_text =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"message_text", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_minvalue =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"minvalue", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_more =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"more", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_mumps =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"mumps", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_names =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"names", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nesting =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nesting", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_next =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"next", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nfc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfc", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nfd =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfd", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nfkc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfkc", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nfkd =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfkd", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_normalized =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"normalized", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nullable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nullable", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_nulls =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nulls", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_number =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"number", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_object =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"object", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_octets =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"octets", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_option =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"option", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_options =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"options", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_ordering =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ordering", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_ordinality =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ordinality", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_others =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"others", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_output =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"output", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_overriding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"overriding", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_p =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"p", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_pad =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"pad", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_mode =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter_mode", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_ordinal_position =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_ordinal_position", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_specific_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_specific_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_specific_name =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_specific_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_specific_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_specific_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_partial =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"partial", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_pascal =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"pascal", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_path =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"path", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_placing =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"placing", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_pli =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"pli", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_preceding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"preceding", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_preserve =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"preserve", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_prior =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"prior", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_privileges =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"privileges", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_public =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"public", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_read =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"read", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_relative =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"relative", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_repeatable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"repeatable", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_respect =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"respect", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_restart =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"restart", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_restrict =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"restrict", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_returned_cardinality =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"returned_cardinality", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_returned_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"returned_length", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_returned_octet_length =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"returned_octet_length", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_returned_sqlstate =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"returned_sqlstate", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_role =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"role", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_routine =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_routine_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_routine_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_routine_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_row_count =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"row_count", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_scale =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scale", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_schema_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"schema_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_scope_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_scope_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_scope_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_section =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"section", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_security =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"security", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_self =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"self", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_sequence =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sequence", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_serializable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"serializable", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_server_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"server_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_session =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"session", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_sets =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sets", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_simple =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"simple", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_size =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"size", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_source =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"source", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_space =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"space", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_specific_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"specific_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_state =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"state", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_statement =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"statement", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_structure =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"structure", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_style =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"style", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_subclass_origin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"subclass_origin", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_t =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"t", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_table_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"table_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_temporary =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"temporary", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_ties =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ties", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_top_level_count =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"top_level_count", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_transaction =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transaction", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_transactions_committed =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"transactions_committed", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_transactions_rolled_back =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"transactions_rolled_back", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_transaction_active =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"transaction_active", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_transform =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transform", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_transforms =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transforms", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_trigger_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_trigger_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_trigger_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_type =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"type", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_unbounded =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unbounded", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_uncommitted =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"uncommitted", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_under =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"under", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_unnamed =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unnamed", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_usage =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"usage", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"user_defined_type_catalog", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_code =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"user_defined_type_code", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_name =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"user_defined_type_name", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"user_defined_type_schema", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_view =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"view", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_work =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"work", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_write =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"write", parser::kw));
    [[maybe_unused]] constexpr auto nrkw_zone =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"zone", parser::kw));

    constexpr auto table_scope = lexy::symbol_table<char const*>
                                         .map<LEXY_SYMBOL("global")>("global")
                                         .map<LEXY_SYMBOL("local")>("local")
                                         .case_folding(dsl::ascii::case_folding);

    struct column_definition
    {
        static constexpr auto rule = []
        { return dsl::p<parser::multipart_identifier> + parser::kw_integer; }();
        static constexpr auto value = lexy::construct<ast::column_definition>;
    };

    struct table_elements
    {
        static constexpr auto rule = []
        {
            return dsl::parenthesized.list(dsl::p<column_definition>,
                dsl::sep(dsl::comma));
        }();

        static constexpr auto value =
            lexy::fold_inplace<ast::table_elements>({},
                [](auto& rv, ast::column_definition v)
                { rv.elements.push_back(std::move(v)); });
    };

    struct table_definition
    {
        static constexpr auto rule = []
        {
            return dsl::opt(
                       dsl::symbol<table_scope>(parser::kw) >> nrkw_temporary) +
                parser::kw_table + dsl::p<parser::multipart_identifier> +
                dsl::p<table_elements>;
        }();
        static constexpr auto value = lexy::construct<ast::table_definition>;
    };

    struct path_specification
    {
        static constexpr auto rule = []
        {
            return dsl::list(dsl::p<parser::multipart_identifier>,
                dsl::sep(dsl::comma));
        }();

        static constexpr auto value =
            lexy::fold_inplace<std::vector<ast::multipart_identifier>>({},
                [](auto& rv, ast::multipart_identifier v)
                { rv.push_back(std::move(v)); });
    };

    struct schema_definition
    {
        static constexpr auto rule = []
        {
            auto authorization =
                parser::kw_authorization >> dsl::p<parser::identifier>;

            auto character_set = parser::kw_default >> parser::kw_character >>
                parser::kw_set >> dsl::p<parser::multipart_identifier>;
            auto path_spec = nrkw_path >> dsl::p<path_specification>;

            return (parser::kw_authorization >>
                           (dsl::nullopt + dsl::p<parser::identifier>) |
                       dsl::else_ >> (dsl::p<parser::multipart_identifier> +
                                         dsl::opt(authorization))) +
                dsl::opt(character_set | path_spec) +
                dsl::opt(character_set | path_spec) +
                dsl::opt(parser::kw_create >> dsl::p<table_definition>);
        }();

        static constexpr auto value = lexy::construct<ast::schema_definition>;
    };

    struct create_statement
    {
        static constexpr auto rule = parser::kw_create >>
            (nrkw_schema >> dsl::p<schema_definition> |
                dsl::else_ >> dsl::p<table_definition>);
        static constexpr auto value =
            lexy::construct<ast::sql_schema_statement>;
    };

    struct sql_schema_statement
    {
        static constexpr auto rule = dsl::p<create_statement>;
        static constexpr auto value =
            lexy::construct<ast::sql_schema_statement>;
    };

    struct sql_executable_statement
    {
        static constexpr auto whitespace = parser::separator;
        static constexpr auto rule = dsl::p<sql_schema_statement> + dsl::eof;
        static constexpr auto value =
            lexy::construct<ast::sql_executable_statement>;
    };
} // namespace

template<>
struct fmt::formatter<melinda::mdbsql::parser::parse_error_detail>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::parser::parse_error_detail const& value,
        FormatContext& ctx) const
    {
        return format_to(ctx.out(),
            "Line number: {}, Column number: {}, Production: {}, Reason: {}",
            value.line_number,
            value.column_number,
            value.production,
            static_cast<uint8_t>(value.kind));
    }
};

template<>
struct fmt::formatter<melinda::mdbsql::parser::parse_error>
{
    constexpr auto parse(format_parse_context& ctx) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(melinda::mdbsql::parser::parse_error const& value,
        FormatContext& ctx) const
    {
        if (!value.details.empty())
        {
            return format_to(ctx.out(),
                "Number of errors {}. Details:\n{}",
                value.details.size(),
                fmt::join(value.details, "\n\t"));
        }
        else
        {
            return format_to(ctx.out(), "No errors reported.");
        }
    }
};

melinda::mdbsql::parse_result melinda::mdbsql::parse(std::string_view statement)
{
    lexy::string_input<lexy::utf8_char_encoding> const range{statement};

    if (auto result{lexy::parse<sql_executable_statement>(range,
            lexy::collect<std::vector<parser::parse_error_detail>>(
                parser::error_callback{}))};
        result.has_value())
    {
        return {std::in_place_index<0>, std::move(result).value()};
    }
    else
    {
        parser::parse_error error{std::move(result).errors()};
        MBLTRC_TRACE_ERROR("Parsing of statement '{}' failed. {}",
            statement,
            error);
        return {std::in_place_index<1>, std::move(error)};
    }
}
