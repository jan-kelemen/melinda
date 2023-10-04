#ifndef MELINDA_MDBSQL_PARSER_RESERVED_WORD_INCLUDED
#define MELINDA_MDBSQL_PARSER_RESERVED_WORD_INCLUDED

#include <lexy/dsl/ascii.hpp>
#include <lexy/dsl/identifier.hpp>
#include <lexy/dsl/case_folding.hpp>

namespace melinda::mdbsql::parser
{
    constexpr auto kw = lexy::dsl::identifier(lexy::dsl::ascii::alpha,
        lexy::dsl::ascii::alpha_underscore / LEXY_LIT("-"));

    constexpr auto kw_abs = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"abs", kw));
    constexpr auto kw_all = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"all", kw));
    constexpr auto kw_allocate =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"allocate", kw));
    constexpr auto kw_alter =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"alter", kw));
    constexpr auto kw_and = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"and", kw));
    constexpr auto kw_any = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"any", kw));
    constexpr auto kw_are = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"are", kw));
    constexpr auto kw_array =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"array", kw));
    constexpr auto kw_array_agg =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"array_agg", kw));
    constexpr auto kw_array_max_cardinality =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"array_max_cardinality", kw));
    constexpr auto kw_as = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"as", kw));
    constexpr auto kw_asensitive =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"asensitive", kw));
    constexpr auto kw_asymmetric =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"asymmetric", kw));
    constexpr auto kw_at = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"at", kw));
    constexpr auto kw_atomic =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"atomic", kw));
    constexpr auto kw_authorization =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"authorization", kw));
    constexpr auto kw_avg = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"avg", kw));
    constexpr auto kw_begin =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"begin", kw));
    constexpr auto kw_begin_frame =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"begin_frame", kw));
    constexpr auto kw_begin_partition =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"begin_partition", kw));
    constexpr auto kw_between =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"between", kw));
    constexpr auto kw_bigint =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"bigint", kw));
    constexpr auto kw_binary =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"binary", kw));
    constexpr auto kw_blob =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"blob", kw));
    constexpr auto kw_boolean =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"boolean", kw));
    constexpr auto kw_both =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"both", kw));
    constexpr auto kw_by = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"by", kw));
    constexpr auto kw_call =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"call", kw));
    constexpr auto kw_called =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"called", kw));
    constexpr auto kw_cardinality =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cardinality", kw));
    constexpr auto kw_cascaded =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cascaded", kw));
    constexpr auto kw_case =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"case", kw));
    constexpr auto kw_cast =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cast", kw));
    constexpr auto kw_ceil =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"ceil", kw));
    constexpr auto kw_ceiling =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"ceiling", kw));
    constexpr auto kw_char =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"char", kw));
    constexpr auto kw_character =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"character", kw));
    constexpr auto kw_character_length =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"character_length", kw));
    constexpr auto kw_char_length =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"char_length", kw));
    constexpr auto kw_check =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"check", kw));
    constexpr auto kw_clob =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"clob", kw));
    constexpr auto kw_close =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"close", kw));
    constexpr auto kw_coalesce =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"coalesce", kw));
    constexpr auto kw_collate =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"collate", kw));
    constexpr auto kw_collect =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"collect", kw));
    constexpr auto kw_column =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"column", kw));
    constexpr auto kw_commit =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"commit", kw));
    constexpr auto kw_condition =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"condition", kw));
    constexpr auto kw_connect =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"connect", kw));
    constexpr auto kw_constraint =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraint", kw));
    constexpr auto kw_contains =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"contains", kw));
    constexpr auto kw_convert =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"convert", kw));
    constexpr auto kw_corr =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"corr", kw));
    constexpr auto kw_corresponding =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"corresponding", kw));
    constexpr auto kw_count =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"count", kw));
    constexpr auto kw_covar_pop =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"covar_pop", kw));
    constexpr auto kw_covar_samp =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"covar_samp", kw));
    constexpr auto kw_create =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"create", kw));
    constexpr auto kw_cross =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cross", kw));
    constexpr auto kw_cube =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cube", kw));
    constexpr auto kw_cume_dist =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cume_dist", kw));
    constexpr auto kw_current =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current", kw));
    constexpr auto kw_current_catalog =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_catalog", kw));
    constexpr auto kw_current_date =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_date", kw));
    constexpr auto kw_current_default_transform_group =
        lexy::dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"current_default_transform_group", kw));
    constexpr auto kw_current_path =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_path", kw));
    constexpr auto kw_current_role =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_role", kw));
    constexpr auto kw_current_row =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_row", kw));
    constexpr auto kw_current_schema =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_schema", kw));
    constexpr auto kw_current_time =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_time", kw));
    constexpr auto kw_current_timestamp =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_timestamp", kw));
    constexpr auto kw_current_transform_group_for_type =
        lexy::dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"current_transform_group_for_type", kw));
    constexpr auto kw_current_user =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_user", kw));
    constexpr auto kw_cursor =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cursor", kw));
    constexpr auto kw_cycle =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"cycle", kw));
    constexpr auto kw_date =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"date", kw));
    constexpr auto kw_day = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"day", kw));
    constexpr auto kw_deallocate =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"deallocate", kw));
    constexpr auto kw_dec = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"dec", kw));
    constexpr auto kw_decimal =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"decimal", kw));
    constexpr auto kw_declare =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"declare", kw));
    constexpr auto kw_default =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"default", kw));
    constexpr auto kw_delete =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"delete", kw));
    constexpr auto kw_dense_rank =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"dense_rank", kw));
    constexpr auto kw_deref =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"deref", kw));
    constexpr auto kw_describe =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"describe", kw));
    constexpr auto kw_deterministic =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"deterministic", kw));
    constexpr auto kw_disconnect =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"disconnect", kw));
    constexpr auto kw_distinct =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"distinct", kw));
    constexpr auto kw_double =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"double", kw));
    constexpr auto kw_drop =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"drop", kw));
    constexpr auto kw_dynamic =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"dynamic", kw));
    constexpr auto kw_each =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"each", kw));
    constexpr auto kw_element =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"element", kw));
    constexpr auto kw_else =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"else", kw));
    constexpr auto kw_end = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"end", kw));
    constexpr auto kw_end_exec =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"end-exec", kw));
    constexpr auto kw_end_frame =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"end_frame", kw));
    constexpr auto kw_end_partition =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"end_partition", kw));
    constexpr auto kw_equals =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"equals", kw));
    constexpr auto kw_escape =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"escape", kw));
    constexpr auto kw_every =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"every", kw));
    constexpr auto kw_except =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"except", kw));
    constexpr auto kw_exec =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"exec", kw));
    constexpr auto kw_execute =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"execute", kw));
    constexpr auto kw_exists =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"exists", kw));
    constexpr auto kw_exp = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"exp", kw));
    constexpr auto kw_external =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"external", kw));
    constexpr auto kw_extract =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"extract", kw));
    constexpr auto kw_false =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"false", kw));
    constexpr auto kw_fetch =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"fetch", kw));
    constexpr auto kw_filter =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"filter", kw));
    constexpr auto kw_first_value =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"first_value", kw));
    constexpr auto kw_float =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"float", kw));
    constexpr auto kw_floor =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"floor", kw));
    constexpr auto kw_for = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"for", kw));
    constexpr auto kw_foreign =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"foreign", kw));
    constexpr auto kw_frame_row =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"frame_row", kw));
    constexpr auto kw_free =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"free", kw));
    constexpr auto kw_from =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"from", kw));
    constexpr auto kw_full =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"full", kw));
    constexpr auto kw_function =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"function", kw));
    constexpr auto kw_fusion =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"fusion", kw));
    constexpr auto kw_get = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"get", kw));
    constexpr auto kw_global =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"global", kw));
    constexpr auto kw_grant =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"grant", kw));
    constexpr auto kw_group =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"group", kw));
    constexpr auto kw_grouping =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"grouping", kw));
    constexpr auto kw_groups =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"groups", kw));
    constexpr auto kw_having =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"having", kw));
    constexpr auto kw_hold =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"hold", kw));
    constexpr auto kw_hour =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"hour", kw));
    constexpr auto kw_identity =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"identity", kw));
    constexpr auto kw_in = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"in", kw));
    constexpr auto kw_indicator =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"indicator", kw));
    constexpr auto kw_inner =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"inner", kw));
    constexpr auto kw_inout =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"inout", kw));
    constexpr auto kw_insensitive =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"insensitive", kw));
    constexpr auto kw_insert =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"insert", kw));
    constexpr auto kw_int = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"int", kw));
    constexpr auto kw_integer =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"integer", kw));
    constexpr auto kw_intersect =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"intersect", kw));
    constexpr auto kw_intersection =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"intersection", kw));
    constexpr auto kw_interval =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"interval", kw));
    constexpr auto kw_into =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"into", kw));
    constexpr auto kw_is = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"is", kw));
    constexpr auto kw_join =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"join", kw));
    constexpr auto kw_lag = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"lag", kw));
    constexpr auto kw_language =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"language", kw));
    constexpr auto kw_large =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"large", kw));
    constexpr auto kw_last_value =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"last_value", kw));
    constexpr auto kw_lateral =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"lateral", kw));
    constexpr auto kw_lead =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"lead", kw));
    constexpr auto kw_leading =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"leading", kw));
    constexpr auto kw_left =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"left", kw));
    constexpr auto kw_like =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"like", kw));
    constexpr auto kw_like_regex =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"like_regex", kw));
    constexpr auto kw_ln = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"ln", kw));
    constexpr auto kw_local =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"local", kw));
    constexpr auto kw_localtime =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"localtime", kw));
    constexpr auto kw_localtimestamp =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"localtimestamp", kw));
    constexpr auto kw_lower =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"lower", kw));
    constexpr auto kw_match =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"match", kw));
    constexpr auto kw_max = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"max", kw));
    constexpr auto kw_member =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"member", kw));
    constexpr auto kw_merge =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"merge", kw));
    constexpr auto kw_method =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"method", kw));
    constexpr auto kw_min = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"min", kw));
    constexpr auto kw_minute =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"minute", kw));
    constexpr auto kw_mod = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"mod", kw));
    constexpr auto kw_modifies =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"modifies", kw));
    constexpr auto kw_module =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"module", kw));
    constexpr auto kw_month =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"month", kw));
    constexpr auto kw_multiset =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"multiset", kw));
    constexpr auto kw_national =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"national", kw));
    constexpr auto kw_natural =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"natural", kw));
    constexpr auto kw_nchar =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"nchar", kw));
    constexpr auto kw_nclob =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"nclob", kw));
    constexpr auto kw_new = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"new", kw));
    constexpr auto kw_no = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"no", kw));
    constexpr auto kw_none =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"none", kw));
    constexpr auto kw_normalize =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"normalize", kw));
    constexpr auto kw_not = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"not", kw));
    constexpr auto kw_nth_value =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"nth_value", kw));
    constexpr auto kw_ntile =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"ntile", kw));
    constexpr auto kw_null =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"null", kw));
    constexpr auto kw_nullif =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"nullif", kw));
    constexpr auto kw_numeric =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"numeric", kw));
    constexpr auto kw_occurrences_regex =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"occurrences_regex", kw));
    constexpr auto kw_octet_length =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"octet_length", kw));
    constexpr auto kw_of = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"of", kw));
    constexpr auto kw_offset =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"offset", kw));
    constexpr auto kw_old = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"old", kw));
    constexpr auto kw_on = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"on", kw));
    constexpr auto kw_only =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"only", kw));
    constexpr auto kw_open =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"open", kw));
    constexpr auto kw_or = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"or", kw));
    constexpr auto kw_order =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"order", kw));
    constexpr auto kw_out = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"out", kw));
    constexpr auto kw_outer =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"outer", kw));
    constexpr auto kw_over =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"over", kw));
    constexpr auto kw_overlaps =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"overlaps", kw));
    constexpr auto kw_overlay =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"overlay", kw));
    constexpr auto kw_parameter =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter", kw));
    constexpr auto kw_partition =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"partition", kw));
    constexpr auto kw_percent =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"percent", kw));
    constexpr auto kw_percentile_cont =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"percentile_cont", kw));
    constexpr auto kw_percentile_disc =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"percentile_disc", kw));
    constexpr auto kw_percent_rank =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"percent_rank", kw));
    constexpr auto kw_period =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"period", kw));
    constexpr auto kw_portion =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"portion", kw));
    constexpr auto kw_position =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"position", kw));
    constexpr auto kw_position_regex =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"position_regex", kw));
    constexpr auto kw_power =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"power", kw));
    constexpr auto kw_precedes =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"precedes", kw));
    constexpr auto kw_precision =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"precision", kw));
    constexpr auto kw_prepare =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"prepare", kw));
    constexpr auto kw_primary =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"primary", kw));
    constexpr auto kw_procedure =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"procedure", kw));
    constexpr auto kw_range =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"range", kw));
    constexpr auto kw_rank =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"rank", kw));
    constexpr auto kw_reads =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"reads", kw));
    constexpr auto kw_real =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"real", kw));
    constexpr auto kw_recursive =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"recursive", kw));
    constexpr auto kw_ref = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"ref", kw));
    constexpr auto kw_references =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"references", kw));
    constexpr auto kw_referencing =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"referencing", kw));
    constexpr auto kw_regr_avgx =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_avgx", kw));
    constexpr auto kw_regr_avgy =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_avgy", kw));
    constexpr auto kw_regr_count =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_count", kw));
    constexpr auto kw_regr_intercept =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_intercept", kw));
    constexpr auto kw_regr_r2 =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_r2", kw));
    constexpr auto kw_regr_slope =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_slope", kw));
    constexpr auto kw_regr_sxx =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_sxx", kw));
    constexpr auto kw_regr_sxy =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_sxy", kw));
    constexpr auto kw_regr_syy =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_syy", kw));
    constexpr auto kw_release =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"release", kw));
    constexpr auto kw_result =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"result", kw));
    constexpr auto kw_return =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"return", kw));
    constexpr auto kw_returns =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"returns", kw));
    constexpr auto kw_revoke =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"revoke", kw));
    constexpr auto kw_right =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"right", kw));
    constexpr auto kw_rollback =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"rollback", kw));
    constexpr auto kw_rollup =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"rollup", kw));
    constexpr auto kw_row = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"row", kw));
    constexpr auto kw_rows =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"rows", kw));
    constexpr auto kw_row_number =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"row_number", kw));
    constexpr auto kw_savepoint =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"savepoint", kw));
    constexpr auto kw_scope =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope", kw));
    constexpr auto kw_scroll =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"scroll", kw));
    constexpr auto kw_search =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"search", kw));
    constexpr auto kw_second =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"second", kw));
    constexpr auto kw_select =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"select", kw));
    constexpr auto kw_sensitive =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sensitive", kw));
    constexpr auto kw_session_user =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"session_user", kw));
    constexpr auto kw_set = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"set", kw));
    constexpr auto kw_similar =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"similar", kw));
    constexpr auto kw_smallint =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"smallint", kw));
    constexpr auto kw_some =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"some", kw));
    constexpr auto kw_specific =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"specific", kw));
    constexpr auto kw_specifictype =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"specifictype", kw));
    constexpr auto kw_sql = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sql", kw));
    constexpr auto kw_sqlexception =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqlexception", kw));
    constexpr auto kw_sqlstate =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqlstate", kw));
    constexpr auto kw_sqlwarning =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqlwarning", kw));
    constexpr auto kw_sqrt =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqrt", kw));
    constexpr auto kw_start =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"start", kw));
    constexpr auto kw_static =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"static", kw));
    constexpr auto kw_stddev_pop =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"stddev_pop", kw));
    constexpr auto kw_stddev_samp =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"stddev_samp", kw));
    constexpr auto kw_submultiset =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"submultiset", kw));
    constexpr auto kw_substring =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"substring", kw));
    constexpr auto kw_substring_regex =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"substring_regex", kw));
    constexpr auto kw_succeeds =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"succeeds", kw));
    constexpr auto kw_sum = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"sum", kw));
    constexpr auto kw_symmetric =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"symmetric", kw));
    constexpr auto kw_system =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"system", kw));
    constexpr auto kw_system_time =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"system_time", kw));
    constexpr auto kw_system_user =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"system_user", kw));
    constexpr auto kw_table =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"table", kw));
    constexpr auto kw_tablesample =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"tablesample", kw));
    constexpr auto kw_then =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"then", kw));
    constexpr auto kw_time =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"time", kw));
    constexpr auto kw_timestamp =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"timestamp", kw));
    constexpr auto kw_timezone_hour =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"timezone_hour", kw));
    constexpr auto kw_timezone_minute =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"timezone_minute", kw));
    constexpr auto kw_to = lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"to", kw));
    constexpr auto kw_trailing =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"trailing", kw));
    constexpr auto kw_translate =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"translate", kw));
    constexpr auto kw_translate_regex =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"translate_regex", kw));
    constexpr auto kw_translation =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"translation", kw));
    constexpr auto kw_treat =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"treat", kw));
    constexpr auto kw_trigger =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger", kw));
    constexpr auto kw_trim =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"trim", kw));
    constexpr auto kw_trim_array =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"trim_array", kw));
    constexpr auto kw_true =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"true", kw));
    constexpr auto kw_truncate =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"truncate", kw));
    constexpr auto kw_uescape =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"uescape", kw));
    constexpr auto kw_union =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"union", kw));
    constexpr auto kw_unique =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"unique", kw));
    constexpr auto kw_unknown =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"unknown", kw));
    constexpr auto kw_unnest =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"unnest", kw));
    constexpr auto kw_update =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"update", kw));
    constexpr auto kw_upper =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"upper", kw));
    constexpr auto kw_user =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"user", kw));
    constexpr auto kw_using =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"using", kw));
    constexpr auto kw_value =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"value", kw));
    constexpr auto kw_values =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"values", kw));
    constexpr auto kw_value_of =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"value_of", kw));
    constexpr auto kw_varbinary =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"varbinary", kw));
    constexpr auto kw_varchar =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"varchar", kw));
    constexpr auto kw_varying =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"varying", kw));
    constexpr auto kw_var_pop =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"var_pop", kw));
    constexpr auto kw_var_samp =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"var_samp", kw));
    constexpr auto kw_versioning =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"versioning", kw));
    constexpr auto kw_when =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"when", kw));
    constexpr auto kw_whenever =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"whenever", kw));
    constexpr auto kw_where =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"where", kw));
    constexpr auto kw_width_bucket =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"width_bucket", kw));
    constexpr auto kw_window =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"window", kw));
    constexpr auto kw_with =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"with", kw));
    constexpr auto kw_within =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"within", kw));
    constexpr auto kw_without =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"without", kw));
    constexpr auto kw_year =
        lexy::dsl::ascii::case_folding(LEXY_KEYWORD(u8"year", kw));

    constexpr auto reserved_set1 = lexy::dsl::literal_set(kw_abs,
        kw_all,
        kw_allocate,
        kw_alter,
        kw_and,
        kw_any,
        kw_are,
        kw_array,
        kw_array_agg,
        kw_array_max_cardinality,
        kw_as,
        kw_asensitive,
        kw_asymmetric,
        kw_at,
        kw_atomic,
        kw_authorization,
        kw_avg,
        kw_begin,
        kw_begin_frame,
        kw_begin_partition,
        kw_between,
        kw_bigint,
        kw_binary,
        kw_blob,
        kw_boolean,
        kw_both,
        kw_by,
        kw_call,
        kw_called,
        kw_cardinality,
        kw_cascaded,
        kw_case,
        kw_cast,
        kw_ceil,
        kw_ceiling,
        kw_char,
        kw_character,
        kw_character_length,
        kw_char_length,
        kw_check,
        kw_clob,
        kw_close,
        kw_coalesce,
        kw_collate,
        kw_collect,
        kw_column,
        kw_commit,
        kw_condition,
        kw_connect,
        kw_constraint,
        kw_contains,
        kw_convert,
        kw_corr,
        kw_corresponding,
        kw_count,
        kw_covar_pop,
        kw_covar_samp,
        kw_create,
        kw_cross,
        kw_cube,
        kw_cume_dist,
        kw_current,
        kw_current_catalog,
        kw_current_date,
        kw_current_default_transform_group,
        kw_current_path,
        kw_current_role,
        kw_current_row,
        kw_current_schema,
        kw_current_time,
        kw_current_timestamp,
        kw_current_transform_group_for_type,
        kw_current_user,
        kw_cursor,
        kw_cycle,
        kw_date,
        kw_day,
        kw_deallocate,
        kw_dec,
        kw_decimal,
        kw_declare,
        kw_default,
        kw_delete,
        kw_dense_rank,
        kw_deref,
        kw_describe,
        kw_deterministic,
        kw_disconnect,
        kw_distinct,
        kw_double,
        kw_drop,
        kw_dynamic,
        kw_each,
        kw_element,
        kw_else,
        kw_end,
        kw_end_exec,
        kw_end_frame,
        kw_end_partition,
        kw_equals,
        kw_escape,
        kw_every,
        kw_except,
        kw_exec,
        kw_execute,
        kw_exists,
        kw_exp,
        kw_external,
        kw_extract,
        kw_false,
        kw_fetch,
        kw_filter,
        kw_first_value,
        kw_float,
        kw_floor,
        kw_for,
        kw_foreign,
        kw_frame_row,
        kw_free,
        kw_from,
        kw_full,
        kw_function,
        kw_fusion,
        kw_get,
        kw_global,
        kw_grant,
        kw_group,
        kw_grouping,
        kw_groups,
        kw_having,
        kw_hold,
        kw_hour,
        kw_identity,
        kw_in,
        kw_indicator,
        kw_inner,
        kw_inout,
        kw_insensitive,
        kw_insert,
        kw_int,
        kw_integer,
        kw_intersect,
        kw_intersection,
        kw_interval,
        kw_into,
        kw_is,
        kw_join);

    constexpr auto reserved_set2 = lexy::dsl::literal_set(kw_lag,
        kw_language,
        kw_large,
        kw_last_value,
        kw_lateral,
        kw_lead,
        kw_leading,
        kw_left,
        kw_like,
        kw_like_regex,
        kw_ln,
        kw_local,
        kw_localtime,
        kw_localtimestamp,
        kw_lower,
        kw_match,
        kw_max,
        kw_member,
        kw_merge,
        kw_method,
        kw_min,
        kw_minute,
        kw_mod,
        kw_modifies,
        kw_module,
        kw_month,
        kw_multiset,
        kw_national,
        kw_natural,
        kw_nchar,
        kw_nclob,
        kw_new,
        kw_no,
        kw_none,
        kw_normalize,
        kw_not,
        kw_nth_value,
        kw_ntile,
        kw_null,
        kw_nullif,
        kw_numeric,
        kw_occurrences_regex,
        kw_octet_length,
        kw_of,
        kw_offset,
        kw_old,
        kw_on,
        kw_only,
        kw_open,
        kw_or,
        kw_order,
        kw_out,
        kw_outer,
        kw_over,
        kw_overlaps,
        kw_overlay,
        kw_parameter,
        kw_partition,
        kw_percent,
        kw_percentile_cont,
        kw_percentile_disc,
        kw_percent_rank,
        kw_period,
        kw_portion,
        kw_position,
        kw_position_regex,
        kw_power,
        kw_precedes,
        kw_precision,
        kw_prepare,
        kw_primary,
        kw_procedure,
        kw_range,
        kw_rank,
        kw_reads,
        kw_real,
        kw_recursive,
        kw_ref,
        kw_references,
        kw_referencing,
        kw_regr_avgx,
        kw_regr_avgy,
        kw_regr_count,
        kw_regr_intercept,
        kw_regr_r2,
        kw_regr_slope,
        kw_regr_sxx,
        kw_regr_sxy,
        kw_regr_syy,
        kw_release,
        kw_result,
        kw_return,
        kw_returns,
        kw_revoke,
        kw_right,
        kw_rollback,
        kw_rollup,
        kw_row,
        kw_rows,
        kw_row_number,
        kw_savepoint,
        kw_scope,
        kw_scroll,
        kw_search,
        kw_second,
        kw_select,
        kw_sensitive,
        kw_session_user,
        kw_set,
        kw_similar,
        kw_smallint,
        kw_some,
        kw_specific,
        kw_specifictype,
        kw_sql,
        kw_sqlexception,
        kw_sqlstate,
        kw_sqlwarning,
        kw_sqrt,
        kw_start,
        kw_static,
        kw_stddev_pop,
        kw_stddev_samp,
        kw_submultiset,
        kw_substring,
        kw_substring_regex,
        kw_succeeds,
        kw_sum,
        kw_symmetric,
        kw_system,
        kw_system_time,
        kw_system_user,
        kw_table,
        kw_tablesample,
        kw_then,
        kw_time,
        kw_timestamp,
        kw_timezone_hour,
        kw_timezone_minute,
        kw_to,
        kw_trailing,
        kw_translate,
        kw_translate_regex,
        kw_translation,
        kw_treat,
        kw_trigger,
        kw_trim,
        kw_trim_array,
        kw_true,
        kw_truncate,
        kw_uescape,
        kw_union,
        kw_unique,
        kw_unknown,
        kw_unnest,
        kw_update,
        kw_upper,
        kw_user,
        kw_using,
        kw_value,
        kw_values,
        kw_value_of,
        kw_varbinary,
        kw_varchar,
        kw_varying,
        kw_var_pop,
        kw_var_samp,
        kw_versioning,
        kw_when,
        kw_whenever,
        kw_where,
        kw_width_bucket,
        kw_window,
        kw_with,
        kw_within,
        kw_without,
        kw_year);
}
#endif
