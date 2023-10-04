#include <mdbsql_parser.h>

#include <algorithm>
#include <string_view>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/action/trace.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/string_input.hpp>

#include <mbltrc_trace.h>

#include <mdbsql_parser_reserved_word.h>

using namespace melinda::mdbsql::parser;

namespace
{
    namespace ast = melinda::mdbsql::ast;
    namespace dsl = lexy::dsl;

    constexpr auto separator = LEXY_LIT("/*") >> dsl::until(LEXY_LIT("*/")) |
        LEXY_LIT("--") >> dsl::until(dsl::newline) | dsl::unicode::space;

    [[maybe_unused]] constexpr auto nrkw_a =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"a", kw));
    [[maybe_unused]] constexpr auto nrkw_absolute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"absolute", kw));
    [[maybe_unused]] constexpr auto nrkw_action =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"action", kw));
    [[maybe_unused]] constexpr auto nrkw_ada =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ada", kw));
    [[maybe_unused]] constexpr auto nrkw_add =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"add", kw));
    [[maybe_unused]] constexpr auto nrkw_admin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"admin", kw));
    [[maybe_unused]] constexpr auto nrkw_after =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"after", kw));
    [[maybe_unused]] constexpr auto nrkw_always =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"always", kw));
    [[maybe_unused]] constexpr auto nrkw_asc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"asc", kw));
    [[maybe_unused]] constexpr auto nrkw_assertion =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"assertion", kw));
    [[maybe_unused]] constexpr auto nrkw_assignment =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"assignment", kw));
    [[maybe_unused]] constexpr auto nrkw_attribute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"attribute", kw));
    [[maybe_unused]] constexpr auto nrkw_attributes =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"attributes", kw));
    [[maybe_unused]] constexpr auto nrkw_before =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"before", kw));
    [[maybe_unused]] constexpr auto nrkw_bernoulli =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"bernoulli", kw));
    [[maybe_unused]] constexpr auto nrkw_breadth =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"breadth", kw));
    [[maybe_unused]] constexpr auto nrkw_c =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"c", kw));
    [[maybe_unused]] constexpr auto nrkw_cascade =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cascade", kw));
    [[maybe_unused]] constexpr auto nrkw_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_catalog_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"catalog_name", kw));
    [[maybe_unused]] constexpr auto nrkw_chain =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"chain", kw));
    [[maybe_unused]] constexpr auto nrkw_characteristics =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"characteristics", kw));
    [[maybe_unused]] constexpr auto nrkw_characters =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"characters", kw));
    [[maybe_unused]] constexpr auto nrkw_character_set_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"character_set_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_character_set_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"character_set_name", kw));
    [[maybe_unused]] constexpr auto nrkw_character_set_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"character_set_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_class_origin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"class_origin", kw));
    [[maybe_unused]] constexpr auto nrkw_cobol =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cobol", kw));
    [[maybe_unused]] constexpr auto nrkw_collation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collation", kw));
    [[maybe_unused]] constexpr auto nrkw_collation_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collation_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_collation_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collation_name", kw));
    [[maybe_unused]] constexpr auto nrkw_collation_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collation_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_column_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"column_name", kw));
    [[maybe_unused]] constexpr auto nrkw_command_function =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"command_function", kw));
    [[maybe_unused]] constexpr auto nrkw_command_function_code =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"command_function_code", kw));
    [[maybe_unused]] constexpr auto nrkw_committed =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"committed", kw));
    [[maybe_unused]] constexpr auto nrkw_condition_number =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"condition_number", kw));
    [[maybe_unused]] constexpr auto nrkw_connection =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"connection", kw));
    [[maybe_unused]] constexpr auto nrkw_connection_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"connection_name", kw));
    [[maybe_unused]] constexpr auto nrkw_constraints =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraints", kw));
    [[maybe_unused]] constexpr auto nrkw_constraint_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraint_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_constraint_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraint_name", kw));
    [[maybe_unused]] constexpr auto nrkw_constraint_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraint_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_constructor =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constructor", kw));
    [[maybe_unused]] constexpr auto nrkw_continue =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"continue", kw));
    [[maybe_unused]] constexpr auto nrkw_cursor_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cursor_name", kw));
    [[maybe_unused]] constexpr auto nrkw_data =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"data", kw));
    [[maybe_unused]] constexpr auto nrkw_datetime_interval_code =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"datetime_interval_code", kw));
    [[maybe_unused]] constexpr auto nrkw_datetime_interval_precision =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"datetime_interval_precision", kw));
    [[maybe_unused]] constexpr auto nrkw_defaults =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"defaults", kw));
    [[maybe_unused]] constexpr auto nrkw_deferrable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deferrable", kw));
    [[maybe_unused]] constexpr auto nrkw_deferred =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deferred", kw));
    [[maybe_unused]] constexpr auto nrkw_defined =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"defined", kw));
    [[maybe_unused]] constexpr auto nrkw_definer =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"definer", kw));
    [[maybe_unused]] constexpr auto nrkw_degree =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"degree", kw));
    [[maybe_unused]] constexpr auto nrkw_depth =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"depth", kw));
    [[maybe_unused]] constexpr auto nrkw_derived =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"derived", kw));
    [[maybe_unused]] constexpr auto nrkw_desc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"desc", kw));
    [[maybe_unused]] constexpr auto nrkw_descriptor =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"descriptor", kw));
    [[maybe_unused]] constexpr auto nrkw_diagnostics =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"diagnostics", kw));
    [[maybe_unused]] constexpr auto nrkw_dispatch =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"dispatch", kw));
    [[maybe_unused]] constexpr auto nrkw_domain =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"domain", kw));
    [[maybe_unused]] constexpr auto nrkw_dynamic_function =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"dynamic_function", kw));
    [[maybe_unused]] constexpr auto nrkw_dynamic_function_code =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"dynamic_function_code", kw));
    [[maybe_unused]] constexpr auto nrkw_enforced =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"enforced", kw));
    [[maybe_unused]] constexpr auto nrkw_exclude =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"exclude", kw));
    [[maybe_unused]] constexpr auto nrkw_excluding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"excluding", kw));
    [[maybe_unused]] constexpr auto nrkw_expression =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"expression", kw));
    [[maybe_unused]] constexpr auto nrkw_final =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"final", kw));
    [[maybe_unused]] constexpr auto nrkw_first =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"first", kw));
    [[maybe_unused]] constexpr auto nrkw_flag =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"flag", kw));
    [[maybe_unused]] constexpr auto nrkw_following =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"following", kw));
    [[maybe_unused]] constexpr auto nrkw_fortran =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"fortran", kw));
    [[maybe_unused]] constexpr auto nrkw_found =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"found", kw));
    [[maybe_unused]] constexpr auto nrkw_g =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"g", kw));
    [[maybe_unused]] constexpr auto nrkw_general =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"general", kw));
    [[maybe_unused]] constexpr auto nrkw_generated =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"generated", kw));
    [[maybe_unused]] constexpr auto nrkw_go =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"go", kw));
    [[maybe_unused]] constexpr auto nrkw_goto =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"goto", kw));
    [[maybe_unused]] constexpr auto nrkw_granted =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"granted", kw));
    [[maybe_unused]] constexpr auto nrkw_hierarchy =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"hierarchy", kw));
    [[maybe_unused]] constexpr auto nrkw_ignore =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ignore", kw));
    [[maybe_unused]] constexpr auto nrkw_immediate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"immediate", kw));
    [[maybe_unused]] constexpr auto nrkw_immediately =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"immediately", kw));
    [[maybe_unused]] constexpr auto nrkw_implementation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"implementation", kw));
    [[maybe_unused]] constexpr auto nrkw_including =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"including", kw));
    [[maybe_unused]] constexpr auto nrkw_increment =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"increment", kw));
    [[maybe_unused]] constexpr auto nrkw_initially =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"initially", kw));
    [[maybe_unused]] constexpr auto nrkw_input =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"input", kw));
    [[maybe_unused]] constexpr auto nrkw_instance =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"instance", kw));
    [[maybe_unused]] constexpr auto nrkw_instantiable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"instantiable", kw));
    [[maybe_unused]] constexpr auto nrkw_instead =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"instead", kw));
    [[maybe_unused]] constexpr auto nrkw_invoker =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"invoker", kw));
    [[maybe_unused]] constexpr auto nrkw_isolation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"isolation", kw));
    [[maybe_unused]] constexpr auto nrkw_k =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"k", kw));
    [[maybe_unused]] constexpr auto nrkw_key =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"key", kw));
    [[maybe_unused]] constexpr auto nrkw_key_member =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"key_member", kw));
    [[maybe_unused]] constexpr auto nrkw_key_type =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"key_type", kw));
    [[maybe_unused]] constexpr auto nrkw_last =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"last", kw));
    [[maybe_unused]] constexpr auto nrkw_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"length", kw));
    [[maybe_unused]] constexpr auto nrkw_level =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"level", kw));
    [[maybe_unused]] constexpr auto nrkw_locator =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"locator", kw));
    [[maybe_unused]] constexpr auto nrkw_m =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"m", kw));
    [[maybe_unused]] constexpr auto nrkw_map =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"map", kw));
    [[maybe_unused]] constexpr auto nrkw_matched =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"matched", kw));
    [[maybe_unused]] constexpr auto nrkw_maxvalue =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"maxvalue", kw));
    [[maybe_unused]] constexpr auto nrkw_message_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"message_length", kw));
    [[maybe_unused]] constexpr auto nrkw_message_octet_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"message_octet_length", kw));
    [[maybe_unused]] constexpr auto nrkw_message_text =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"message_text", kw));
    [[maybe_unused]] constexpr auto nrkw_minvalue =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"minvalue", kw));
    [[maybe_unused]] constexpr auto nrkw_more =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"more", kw));
    [[maybe_unused]] constexpr auto nrkw_mumps =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"mumps", kw));
    [[maybe_unused]] constexpr auto nrkw_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"name", kw));
    [[maybe_unused]] constexpr auto nrkw_names =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"names", kw));
    [[maybe_unused]] constexpr auto nrkw_nesting =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nesting", kw));
    [[maybe_unused]] constexpr auto nrkw_next =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"next", kw));
    [[maybe_unused]] constexpr auto nrkw_nfc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfc", kw));
    [[maybe_unused]] constexpr auto nrkw_nfd =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfd", kw));
    [[maybe_unused]] constexpr auto nrkw_nfkc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfkc", kw));
    [[maybe_unused]] constexpr auto nrkw_nfkd =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nfkd", kw));
    [[maybe_unused]] constexpr auto nrkw_normalized =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"normalized", kw));
    [[maybe_unused]] constexpr auto nrkw_nullable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nullable", kw));
    [[maybe_unused]] constexpr auto nrkw_nulls =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nulls", kw));
    [[maybe_unused]] constexpr auto nrkw_number =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"number", kw));
    [[maybe_unused]] constexpr auto nrkw_object =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"object", kw));
    [[maybe_unused]] constexpr auto nrkw_octets =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"octets", kw));
    [[maybe_unused]] constexpr auto nrkw_option =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"option", kw));
    [[maybe_unused]] constexpr auto nrkw_options =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"options", kw));
    [[maybe_unused]] constexpr auto nrkw_ordering =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ordering", kw));
    [[maybe_unused]] constexpr auto nrkw_ordinality =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ordinality", kw));
    [[maybe_unused]] constexpr auto nrkw_others =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"others", kw));
    [[maybe_unused]] constexpr auto nrkw_output =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"output", kw));
    [[maybe_unused]] constexpr auto nrkw_overriding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"overriding", kw));
    [[maybe_unused]] constexpr auto nrkw_p =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"p", kw));
    [[maybe_unused]] constexpr auto nrkw_pad =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"pad", kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_mode =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter_mode", kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter_name", kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_ordinal_position =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_ordinal_position", kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_specific_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_specific_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_specific_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter_specific_name", kw));
    [[maybe_unused]] constexpr auto nrkw_parameter_specific_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"parameter_specific_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_partial =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"partial", kw));
    [[maybe_unused]] constexpr auto nrkw_pascal =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"pascal", kw));
    [[maybe_unused]] constexpr auto nrkw_path =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"path", kw));
    [[maybe_unused]] constexpr auto nrkw_placing =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"placing", kw));
    [[maybe_unused]] constexpr auto nrkw_pli =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"pli", kw));
    [[maybe_unused]] constexpr auto nrkw_preceding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"preceding", kw));
    [[maybe_unused]] constexpr auto nrkw_preserve =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"preserve", kw));
    [[maybe_unused]] constexpr auto nrkw_prior =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"prior", kw));
    [[maybe_unused]] constexpr auto nrkw_privileges =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"privileges", kw));
    [[maybe_unused]] constexpr auto nrkw_public =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"public", kw));
    [[maybe_unused]] constexpr auto nrkw_read =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"read", kw));
    [[maybe_unused]] constexpr auto nrkw_relative =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"relative", kw));
    [[maybe_unused]] constexpr auto nrkw_repeatable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"repeatable", kw));
    [[maybe_unused]] constexpr auto nrkw_respect =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"respect", kw));
    [[maybe_unused]] constexpr auto nrkw_restart =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"restart", kw));
    [[maybe_unused]] constexpr auto nrkw_restrict =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"restrict", kw));
    [[maybe_unused]] constexpr auto nrkw_returned_cardinality =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"returned_cardinality", kw));
    [[maybe_unused]] constexpr auto nrkw_returned_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"returned_length", kw));
    [[maybe_unused]] constexpr auto nrkw_returned_octet_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"returned_octet_length", kw));
    [[maybe_unused]] constexpr auto nrkw_returned_sqlstate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"returned_sqlstate", kw));
    [[maybe_unused]] constexpr auto nrkw_role =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"role", kw));
    [[maybe_unused]] constexpr auto nrkw_routine =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine", kw));
    [[maybe_unused]] constexpr auto nrkw_routine_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_routine_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine_name", kw));
    [[maybe_unused]] constexpr auto nrkw_routine_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"routine_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_row_count =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"row_count", kw));
    [[maybe_unused]] constexpr auto nrkw_scale =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scale", kw));
    [[maybe_unused]] constexpr auto nrkw_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"schema", kw));
    [[maybe_unused]] constexpr auto nrkw_schema_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"schema_name", kw));
    [[maybe_unused]] constexpr auto nrkw_scope_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_scope_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope_name", kw));
    [[maybe_unused]] constexpr auto nrkw_scope_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_section =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"section", kw));
    [[maybe_unused]] constexpr auto nrkw_security =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"security", kw));
    [[maybe_unused]] constexpr auto nrkw_self =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"self", kw));
    [[maybe_unused]] constexpr auto nrkw_sequence =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sequence", kw));
    [[maybe_unused]] constexpr auto nrkw_serializable =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"serializable", kw));
    [[maybe_unused]] constexpr auto nrkw_server_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"server_name", kw));
    [[maybe_unused]] constexpr auto nrkw_session =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"session", kw));
    [[maybe_unused]] constexpr auto nrkw_sets =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sets", kw));
    [[maybe_unused]] constexpr auto nrkw_simple =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"simple", kw));
    [[maybe_unused]] constexpr auto nrkw_size =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"size", kw));
    [[maybe_unused]] constexpr auto nrkw_source =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"source", kw));
    [[maybe_unused]] constexpr auto nrkw_space =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"space", kw));
    [[maybe_unused]] constexpr auto nrkw_specific_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"specific_name", kw));
    [[maybe_unused]] constexpr auto nrkw_state =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"state", kw));
    [[maybe_unused]] constexpr auto nrkw_statement =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"statement", kw));
    [[maybe_unused]] constexpr auto nrkw_structure =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"structure", kw));
    [[maybe_unused]] constexpr auto nrkw_style =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"style", kw));
    [[maybe_unused]] constexpr auto nrkw_subclass_origin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"subclass_origin", kw));
    [[maybe_unused]] constexpr auto nrkw_t =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"t", kw));
    [[maybe_unused]] constexpr auto nrkw_table_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"table_name", kw));
    [[maybe_unused]] constexpr auto nrkw_temporary =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"temporary", kw));
    [[maybe_unused]] constexpr auto nrkw_ties =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ties", kw));
    [[maybe_unused]] constexpr auto nrkw_top_level_count =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"top_level_count", kw));
    [[maybe_unused]] constexpr auto nrkw_transaction =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transaction", kw));
    [[maybe_unused]] constexpr auto nrkw_transactions_committed =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transactions_committed", kw));
    [[maybe_unused]] constexpr auto nrkw_transactions_rolled_back =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"transactions_rolled_back", kw));
    [[maybe_unused]] constexpr auto nrkw_transaction_active =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transaction_active", kw));
    [[maybe_unused]] constexpr auto nrkw_transform =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transform", kw));
    [[maybe_unused]] constexpr auto nrkw_transforms =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"transforms", kw));
    [[maybe_unused]] constexpr auto nrkw_trigger_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_trigger_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger_name", kw));
    [[maybe_unused]] constexpr auto nrkw_trigger_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_type =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"type", kw));
    [[maybe_unused]] constexpr auto nrkw_unbounded =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unbounded", kw));
    [[maybe_unused]] constexpr auto nrkw_uncommitted =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"uncommitted", kw));
    [[maybe_unused]] constexpr auto nrkw_under =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"under", kw));
    [[maybe_unused]] constexpr auto nrkw_unnamed =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unnamed", kw));
    [[maybe_unused]] constexpr auto nrkw_usage =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"usage", kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_catalog =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"user_defined_type_catalog", kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_code =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"user_defined_type_code", kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_name =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"user_defined_type_name", kw));
    [[maybe_unused]] constexpr auto nrkw_user_defined_type_schema =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"user_defined_type_schema", kw));
    [[maybe_unused]] constexpr auto nrkw_view =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"view", kw));
    [[maybe_unused]] constexpr auto nrkw_work =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"work", kw));
    [[maybe_unused]] constexpr auto nrkw_write =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"write", kw));
    [[maybe_unused]] constexpr auto nrkw_zone =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"zone", kw));

    constexpr auto table_scope = lexy::symbol_table<char const*>
                                         .map<LEXY_SYMBOL("global")>("global")
                                         .map<LEXY_SYMBOL("local")>("local")
                                         .case_folding(dsl::ascii::case_folding);

    struct identifier_part
    {
        static constexpr auto rule = []
        {
            auto lead_char = dsl::ascii::alpha;
            auto trailing_char =
                dsl::ascii::alpha_digit_underscore / LEXY_LIT("-");
            auto id = dsl::identifier(lead_char, trailing_char);

            return id.reserve(reserved_set1).reserve(reserved_set2);
        }();

        static constexpr auto value =
            lexy::as_string<std::string, lexy::utf8_encoding>;
    };

    struct identifier
    {
        static constexpr auto rule = []
        { return dsl::list(dsl::p<identifier_part>, dsl::sep(dsl::period)); }();

        static constexpr auto value = lexy::fold_inplace<ast::identifier>(
            ast::identifier{},
            [](auto& rv, std::string i) { rv.parts.push_back(std::move(i)); });
    };

    struct column_definition
    {
        static constexpr auto rule = []
        { return dsl::p<identifier> + kw_integer; }();
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
            return dsl::opt(dsl::symbol<table_scope>(kw) >> nrkw_temporary) +
                kw_table + dsl::p<identifier> + dsl::p<table_elements>;
        }();
        static constexpr auto value = lexy::construct<ast::table_definition>;
    };

    struct path_specification
    {
        static constexpr auto rule = []
        { return dsl::list(dsl::p<identifier>, dsl::sep(dsl::comma)); }();

        static constexpr auto value =
            lexy::fold_inplace<std::vector<ast::identifier>>({},
                [](auto& rv, ast::identifier v)
                { rv.push_back(std::move(v)); });
    };

    struct schema_definition
    {
        static constexpr auto rule = []
        {
            auto authorization = kw_authorization >> dsl::p<identifier_part>;

            auto character_set =
                kw_default >> kw_character >> kw_set >> dsl::p<identifier>;
            auto path_spec = nrkw_path >> dsl::p<path_specification>;

            return (kw_authorization >>
                           (dsl::nullopt + dsl::p<identifier_part>) |
                       dsl::else_ >>
                           (dsl::p<identifier> + dsl::opt(authorization))) +
                dsl::opt(character_set | path_spec) +
                dsl::opt(character_set | path_spec) +
                dsl::opt(kw_create >> dsl::p<table_definition>);
        }();

        static constexpr auto value = lexy::construct<ast::schema_definition>;
    };

    struct create_statement
    {
        static constexpr auto rule = kw_create >>
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
        static constexpr auto whitespace = separator;
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
