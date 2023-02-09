#include "mdbsql_ast.h"
#include <mdbsql_parser.h>

#include <algorithm>
#include <string_view>
#include <vector>

#include <lexy/action/parse.hpp>
#include <lexy/action/trace.hpp>
#include <lexy/callback.hpp>
#include <lexy/dsl.hpp>
#include <lexy/encoding.hpp>
#include <lexy/input/range_input.hpp>

#include <lexy_ext/report_error.hpp>

#include <mbltrc_trace.h>

namespace
{
    namespace ast = melinda::mdbsql::ast;
    namespace dsl = lexy::dsl;

    constexpr auto separator = LEXY_LIT("/*") >> dsl::until(LEXY_LIT("*/")) |
        LEXY_LIT("--") >> dsl::until(dsl::newline) | dsl::unicode::space;

    constexpr auto kw = dsl::identifier(dsl::ascii::alpha,
        dsl::ascii::alpha_underscore / LEXY_LIT("-"));

    constexpr auto kw_abs = dsl::ascii::case_folding(LEXY_KEYWORD(u8"abs", kw));
    constexpr auto kw_all = dsl::ascii::case_folding(LEXY_KEYWORD(u8"all", kw));
    constexpr auto kw_allocate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"allocate", kw));
    constexpr auto kw_alter =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"alter", kw));
    constexpr auto kw_and = dsl::ascii::case_folding(LEXY_KEYWORD(u8"and", kw));
    constexpr auto kw_any = dsl::ascii::case_folding(LEXY_KEYWORD(u8"any", kw));
    constexpr auto kw_are = dsl::ascii::case_folding(LEXY_KEYWORD(u8"are", kw));
    constexpr auto kw_array =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"array", kw));
    constexpr auto kw_array_agg =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"array_agg", kw));
    constexpr auto kw_array_max_cardinality =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"array_max_cardinality", kw));
    constexpr auto kw_as = dsl::ascii::case_folding(LEXY_KEYWORD(u8"as", kw));
    constexpr auto kw_asensitive =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"asensitive", kw));
    constexpr auto kw_asymmetric =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"asymmetric", kw));
    constexpr auto kw_at = dsl::ascii::case_folding(LEXY_KEYWORD(u8"at", kw));
    constexpr auto kw_atomic =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"atomic", kw));
    constexpr auto kw_authorization =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"authorization", kw));
    constexpr auto kw_avg = dsl::ascii::case_folding(LEXY_KEYWORD(u8"avg", kw));
    constexpr auto kw_begin =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"begin", kw));
    constexpr auto kw_begin_frame =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"begin_frame", kw));
    constexpr auto kw_begin_partition =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"begin_partition", kw));
    constexpr auto kw_between =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"between", kw));
    constexpr auto kw_bigint =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"bigint", kw));
    constexpr auto kw_binary =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"binary", kw));
    constexpr auto kw_blob =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"blob", kw));
    constexpr auto kw_boolean =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"boolean", kw));
    constexpr auto kw_both =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"both", kw));
    constexpr auto kw_by = dsl::ascii::case_folding(LEXY_KEYWORD(u8"by", kw));
    constexpr auto kw_call =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"call", kw));
    constexpr auto kw_called =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"called", kw));
    constexpr auto kw_cardinality =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cardinality", kw));
    constexpr auto kw_cascaded =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cascaded", kw));
    constexpr auto kw_case =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"case", kw));
    constexpr auto kw_cast =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cast", kw));
    constexpr auto kw_ceil =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ceil", kw));
    constexpr auto kw_ceiling =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ceiling", kw));
    constexpr auto kw_char =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"char", kw));
    constexpr auto kw_character =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"character", kw));
    constexpr auto kw_character_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"character_length", kw));
    constexpr auto kw_char_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"char_length", kw));
    constexpr auto kw_check =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"check", kw));
    constexpr auto kw_clob =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"clob", kw));
    constexpr auto kw_close =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"close", kw));
    constexpr auto kw_coalesce =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"coalesce", kw));
    constexpr auto kw_collate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collate", kw));
    constexpr auto kw_collect =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"collect", kw));
    constexpr auto kw_column =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"column", kw));
    constexpr auto kw_commit =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"commit", kw));
    constexpr auto kw_condition =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"condition", kw));
    constexpr auto kw_connect =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"connect", kw));
    constexpr auto kw_constraint =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"constraint", kw));
    constexpr auto kw_contains =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"contains", kw));
    constexpr auto kw_convert =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"convert", kw));
    constexpr auto kw_corr =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"corr", kw));
    constexpr auto kw_corresponding =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"corresponding", kw));
    constexpr auto kw_count =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"count", kw));
    constexpr auto kw_covar_pop =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"covar_pop", kw));
    constexpr auto kw_covar_samp =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"covar_samp", kw));
    constexpr auto kw_create =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"create", kw));
    constexpr auto kw_cross =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cross", kw));
    constexpr auto kw_cube =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cube", kw));
    constexpr auto kw_cume_dist =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cume_dist", kw));
    constexpr auto kw_current =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current", kw));
    constexpr auto kw_current_catalog =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_catalog", kw));
    constexpr auto kw_current_date =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_date", kw));
    constexpr auto kw_current_default_transform_group =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"current_default_transform_group", kw));
    constexpr auto kw_current_path =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_path", kw));
    constexpr auto kw_current_role =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_role", kw));
    constexpr auto kw_current_row =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_row", kw));
    constexpr auto kw_current_schema =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_schema", kw));
    constexpr auto kw_current_time =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_time", kw));
    constexpr auto kw_current_timestamp =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_timestamp", kw));
    constexpr auto kw_current_transform_group_for_type =
        dsl::ascii::case_folding(
            LEXY_KEYWORD(u8"current_transform_group_for_type", kw));
    constexpr auto kw_current_user =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"current_user", kw));
    constexpr auto kw_cursor =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cursor", kw));
    constexpr auto kw_cycle =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"cycle", kw));
    constexpr auto kw_date =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"date", kw));
    constexpr auto kw_day = dsl::ascii::case_folding(LEXY_KEYWORD(u8"day", kw));
    constexpr auto kw_deallocate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deallocate", kw));
    constexpr auto kw_dec = dsl::ascii::case_folding(LEXY_KEYWORD(u8"dec", kw));
    constexpr auto kw_decimal =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"decimal", kw));
    constexpr auto kw_declare =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"declare", kw));
    constexpr auto kw_default =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"default", kw));
    constexpr auto kw_delete =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"delete", kw));
    constexpr auto kw_dense_rank =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"dense_rank", kw));
    constexpr auto kw_deref =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deref", kw));
    constexpr auto kw_describe =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"describe", kw));
    constexpr auto kw_deterministic =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"deterministic", kw));
    constexpr auto kw_disconnect =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"disconnect", kw));
    constexpr auto kw_distinct =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"distinct", kw));
    constexpr auto kw_double =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"double", kw));
    constexpr auto kw_drop =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"drop", kw));
    constexpr auto kw_dynamic =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"dynamic", kw));
    constexpr auto kw_each =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"each", kw));
    constexpr auto kw_element =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"element", kw));
    constexpr auto kw_else =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"else", kw));
    constexpr auto kw_end = dsl::ascii::case_folding(LEXY_KEYWORD(u8"end", kw));
    constexpr auto kw_end_exec =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"end-exec", kw));
    constexpr auto kw_end_frame =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"end_frame", kw));
    constexpr auto kw_end_partition =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"end_partition", kw));
    constexpr auto kw_equals =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"equals", kw));
    constexpr auto kw_escape =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"escape", kw));
    constexpr auto kw_every =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"every", kw));
    constexpr auto kw_except =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"except", kw));
    constexpr auto kw_exec =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"exec", kw));
    constexpr auto kw_execute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"execute", kw));
    constexpr auto kw_exists =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"exists", kw));
    constexpr auto kw_exp = dsl::ascii::case_folding(LEXY_KEYWORD(u8"exp", kw));
    constexpr auto kw_external =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"external", kw));
    constexpr auto kw_extract =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"extract", kw));
    constexpr auto kw_false =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"false", kw));
    constexpr auto kw_fetch =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"fetch", kw));
    constexpr auto kw_filter =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"filter", kw));
    constexpr auto kw_first_value =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"first_value", kw));
    constexpr auto kw_float =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"float", kw));
    constexpr auto kw_floor =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"floor", kw));
    constexpr auto kw_for = dsl::ascii::case_folding(LEXY_KEYWORD(u8"for", kw));
    constexpr auto kw_foreign =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"foreign", kw));
    constexpr auto kw_frame_row =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"frame_row", kw));
    constexpr auto kw_free =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"free", kw));
    constexpr auto kw_from =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"from", kw));
    constexpr auto kw_full =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"full", kw));
    constexpr auto kw_function =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"function", kw));
    constexpr auto kw_fusion =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"fusion", kw));
    constexpr auto kw_get = dsl::ascii::case_folding(LEXY_KEYWORD(u8"get", kw));
    constexpr auto kw_global =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"global", kw));
    constexpr auto kw_grant =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"grant", kw));
    constexpr auto kw_group =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"group", kw));
    constexpr auto kw_grouping =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"grouping", kw));
    constexpr auto kw_groups =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"groups", kw));
    constexpr auto kw_having =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"having", kw));
    constexpr auto kw_hold =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"hold", kw));
    constexpr auto kw_hour =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"hour", kw));
    constexpr auto kw_identity =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"identity", kw));
    constexpr auto kw_in = dsl::ascii::case_folding(LEXY_KEYWORD(u8"in", kw));
    constexpr auto kw_indicator =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"indicator", kw));
    constexpr auto kw_inner =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"inner", kw));
    constexpr auto kw_inout =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"inout", kw));
    constexpr auto kw_insensitive =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"insensitive", kw));
    constexpr auto kw_insert =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"insert", kw));
    constexpr auto kw_int = dsl::ascii::case_folding(LEXY_KEYWORD(u8"int", kw));
    constexpr auto kw_integer =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"integer", kw));
    constexpr auto kw_intersect =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"intersect", kw));
    constexpr auto kw_intersection =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"intersection", kw));
    constexpr auto kw_interval =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"interval", kw));
    constexpr auto kw_into =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"into", kw));
    constexpr auto kw_is = dsl::ascii::case_folding(LEXY_KEYWORD(u8"is", kw));
    constexpr auto kw_join =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"join", kw));
    constexpr auto kw_lag = dsl::ascii::case_folding(LEXY_KEYWORD(u8"lag", kw));
    constexpr auto kw_language =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"language", kw));
    constexpr auto kw_large =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"large", kw));
    constexpr auto kw_last_value =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"last_value", kw));
    constexpr auto kw_lateral =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"lateral", kw));
    constexpr auto kw_lead =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"lead", kw));
    constexpr auto kw_leading =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"leading", kw));
    constexpr auto kw_left =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"left", kw));
    constexpr auto kw_like =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"like", kw));
    constexpr auto kw_like_regex =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"like_regex", kw));
    constexpr auto kw_ln = dsl::ascii::case_folding(LEXY_KEYWORD(u8"ln", kw));
    constexpr auto kw_local =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"local", kw));
    constexpr auto kw_localtime =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"localtime", kw));
    constexpr auto kw_localtimestamp =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"localtimestamp", kw));
    constexpr auto kw_lower =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"lower", kw));
    constexpr auto kw_match =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"match", kw));
    constexpr auto kw_max = dsl::ascii::case_folding(LEXY_KEYWORD(u8"max", kw));
    constexpr auto kw_member =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"member", kw));
    constexpr auto kw_merge =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"merge", kw));
    constexpr auto kw_method =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"method", kw));
    constexpr auto kw_min = dsl::ascii::case_folding(LEXY_KEYWORD(u8"min", kw));
    constexpr auto kw_minute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"minute", kw));
    constexpr auto kw_mod = dsl::ascii::case_folding(LEXY_KEYWORD(u8"mod", kw));
    constexpr auto kw_modifies =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"modifies", kw));
    constexpr auto kw_module =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"module", kw));
    constexpr auto kw_month =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"month", kw));
    constexpr auto kw_multiset =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"multiset", kw));
    constexpr auto kw_national =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"national", kw));
    constexpr auto kw_natural =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"natural", kw));
    constexpr auto kw_nchar =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nchar", kw));
    constexpr auto kw_nclob =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nclob", kw));
    constexpr auto kw_new = dsl::ascii::case_folding(LEXY_KEYWORD(u8"new", kw));
    constexpr auto kw_no = dsl::ascii::case_folding(LEXY_KEYWORD(u8"no", kw));
    constexpr auto kw_none =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"none", kw));
    constexpr auto kw_normalize =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"normalize", kw));
    constexpr auto kw_not = dsl::ascii::case_folding(LEXY_KEYWORD(u8"not", kw));
    constexpr auto kw_nth_value =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nth_value", kw));
    constexpr auto kw_ntile =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"ntile", kw));
    constexpr auto kw_null =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"null", kw));
    constexpr auto kw_nullif =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"nullif", kw));
    constexpr auto kw_numeric =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"numeric", kw));
    constexpr auto kw_occurrences_regex =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"occurrences_regex", kw));
    constexpr auto kw_octet_length =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"octet_length", kw));
    constexpr auto kw_of = dsl::ascii::case_folding(LEXY_KEYWORD(u8"of", kw));
    constexpr auto kw_offset =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"offset", kw));
    constexpr auto kw_old = dsl::ascii::case_folding(LEXY_KEYWORD(u8"old", kw));
    constexpr auto kw_on = dsl::ascii::case_folding(LEXY_KEYWORD(u8"on", kw));
    constexpr auto kw_only =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"only", kw));
    constexpr auto kw_open =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"open", kw));
    constexpr auto kw_or = dsl::ascii::case_folding(LEXY_KEYWORD(u8"or", kw));
    constexpr auto kw_order =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"order", kw));
    constexpr auto kw_out = dsl::ascii::case_folding(LEXY_KEYWORD(u8"out", kw));
    constexpr auto kw_outer =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"outer", kw));
    constexpr auto kw_over =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"over", kw));
    constexpr auto kw_overlaps =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"overlaps", kw));
    constexpr auto kw_overlay =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"overlay", kw));
    constexpr auto kw_parameter =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"parameter", kw));
    constexpr auto kw_partition =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"partition", kw));
    constexpr auto kw_percent =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"percent", kw));
    constexpr auto kw_percentile_cont =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"percentile_cont", kw));
    constexpr auto kw_percentile_disc =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"percentile_disc", kw));
    constexpr auto kw_percent_rank =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"percent_rank", kw));
    constexpr auto kw_period =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"period", kw));
    constexpr auto kw_portion =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"portion", kw));
    constexpr auto kw_position =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"position", kw));
    constexpr auto kw_position_regex =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"position_regex", kw));
    constexpr auto kw_power =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"power", kw));
    constexpr auto kw_precedes =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"precedes", kw));
    constexpr auto kw_precision =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"precision", kw));
    constexpr auto kw_prepare =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"prepare", kw));
    constexpr auto kw_primary =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"primary", kw));
    constexpr auto kw_procedure =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"procedure", kw));
    constexpr auto kw_range =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"range", kw));
    constexpr auto kw_rank =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"rank", kw));
    constexpr auto kw_reads =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"reads", kw));
    constexpr auto kw_real =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"real", kw));
    constexpr auto kw_recursive =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"recursive", kw));
    constexpr auto kw_ref = dsl::ascii::case_folding(LEXY_KEYWORD(u8"ref", kw));
    constexpr auto kw_references =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"references", kw));
    constexpr auto kw_referencing =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"referencing", kw));
    constexpr auto kw_regr_avgx =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_avgx", kw));
    constexpr auto kw_regr_avgy =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_avgy", kw));
    constexpr auto kw_regr_count =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_count", kw));
    constexpr auto kw_regr_intercept =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_intercept", kw));
    constexpr auto kw_regr_r2 =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_r2", kw));
    constexpr auto kw_regr_slope =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_slope", kw));
    constexpr auto kw_regr_sxx =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_sxx", kw));
    constexpr auto kw_regr_sxy =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_sxy", kw));
    constexpr auto kw_regr_syy =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"regr_syy", kw));
    constexpr auto kw_release =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"release", kw));
    constexpr auto kw_result =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"result", kw));
    constexpr auto kw_return =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"return", kw));
    constexpr auto kw_returns =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"returns", kw));
    constexpr auto kw_revoke =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"revoke", kw));
    constexpr auto kw_right =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"right", kw));
    constexpr auto kw_rollback =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"rollback", kw));
    constexpr auto kw_rollup =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"rollup", kw));
    constexpr auto kw_row = dsl::ascii::case_folding(LEXY_KEYWORD(u8"row", kw));
    constexpr auto kw_rows =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"rows", kw));
    constexpr auto kw_row_number =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"row_number", kw));
    constexpr auto kw_savepoint =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"savepoint", kw));
    constexpr auto kw_scope =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scope", kw));
    constexpr auto kw_scroll =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"scroll", kw));
    constexpr auto kw_search =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"search", kw));
    constexpr auto kw_second =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"second", kw));
    constexpr auto kw_select =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"select", kw));
    constexpr auto kw_sensitive =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sensitive", kw));
    constexpr auto kw_session_user =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"session_user", kw));
    constexpr auto kw_set = dsl::ascii::case_folding(LEXY_KEYWORD(u8"set", kw));
    constexpr auto kw_similar =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"similar", kw));
    constexpr auto kw_smallint =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"smallint", kw));
    constexpr auto kw_some =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"some", kw));
    constexpr auto kw_specific =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"specific", kw));
    constexpr auto kw_specifictype =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"specifictype", kw));
    constexpr auto kw_sql = dsl::ascii::case_folding(LEXY_KEYWORD(u8"sql", kw));
    constexpr auto kw_sqlexception =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqlexception", kw));
    constexpr auto kw_sqlstate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqlstate", kw));
    constexpr auto kw_sqlwarning =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqlwarning", kw));
    constexpr auto kw_sqrt =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"sqrt", kw));
    constexpr auto kw_start =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"start", kw));
    constexpr auto kw_static =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"static", kw));
    constexpr auto kw_stddev_pop =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"stddev_pop", kw));
    constexpr auto kw_stddev_samp =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"stddev_samp", kw));
    constexpr auto kw_submultiset =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"submultiset", kw));
    constexpr auto kw_substring =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"substring", kw));
    constexpr auto kw_substring_regex =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"substring_regex", kw));
    constexpr auto kw_succeeds =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"succeeds", kw));
    constexpr auto kw_sum = dsl::ascii::case_folding(LEXY_KEYWORD(u8"sum", kw));
    constexpr auto kw_symmetric =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"symmetric", kw));
    constexpr auto kw_system =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"system", kw));
    constexpr auto kw_system_time =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"system_time", kw));
    constexpr auto kw_system_user =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"system_user", kw));
    constexpr auto kw_table =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"table", kw));
    constexpr auto kw_tablesample =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"tablesample", kw));
    constexpr auto kw_then =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"then", kw));
    constexpr auto kw_time =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"time", kw));
    constexpr auto kw_timestamp =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"timestamp", kw));
    constexpr auto kw_timezone_hour =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"timezone_hour", kw));
    constexpr auto kw_timezone_minute =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"timezone_minute", kw));
    constexpr auto kw_to = dsl::ascii::case_folding(LEXY_KEYWORD(u8"to", kw));
    constexpr auto kw_trailing =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trailing", kw));
    constexpr auto kw_translate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"translate", kw));
    constexpr auto kw_translate_regex =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"translate_regex", kw));
    constexpr auto kw_translation =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"translation", kw));
    constexpr auto kw_treat =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"treat", kw));
    constexpr auto kw_trigger =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trigger", kw));
    constexpr auto kw_trim =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trim", kw));
    constexpr auto kw_trim_array =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"trim_array", kw));
    constexpr auto kw_true =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"true", kw));
    constexpr auto kw_truncate =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"truncate", kw));
    constexpr auto kw_uescape =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"uescape", kw));
    constexpr auto kw_union =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"union", kw));
    constexpr auto kw_unique =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unique", kw));
    constexpr auto kw_unknown =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unknown", kw));
    constexpr auto kw_unnest =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"unnest", kw));
    constexpr auto kw_update =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"update", kw));
    constexpr auto kw_upper =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"upper", kw));
    constexpr auto kw_user =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"user", kw));
    constexpr auto kw_using =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"using", kw));
    constexpr auto kw_value =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"value", kw));
    constexpr auto kw_values =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"values", kw));
    constexpr auto kw_value_of =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"value_of", kw));
    constexpr auto kw_varbinary =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"varbinary", kw));
    constexpr auto kw_varchar =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"varchar", kw));
    constexpr auto kw_varying =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"varying", kw));
    constexpr auto kw_var_pop =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"var_pop", kw));
    constexpr auto kw_var_samp =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"var_samp", kw));
    constexpr auto kw_versioning =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"versioning", kw));
    constexpr auto kw_when =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"when", kw));
    constexpr auto kw_whenever =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"whenever", kw));
    constexpr auto kw_where =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"where", kw));
    constexpr auto kw_width_bucket =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"width_bucket", kw));
    constexpr auto kw_window =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"window", kw));
    constexpr auto kw_with =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"with", kw));
    constexpr auto kw_within =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"within", kw));
    constexpr auto kw_without =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"without", kw));
    constexpr auto kw_year =
        dsl::ascii::case_folding(LEXY_KEYWORD(u8"year", kw));

    constexpr auto reserved_set1 = dsl::literal_set(kw_abs,
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

    constexpr auto reserved_set2 = dsl::literal_set(kw_lag,
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
            ast::identifier {},
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

            return (kw_authorization >> dsl::nullopt + dsl::p<identifier_part> |
                       dsl::else_ >>
                           dsl::p<identifier> + dsl::opt(authorization)) +
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
        static constexpr auto rule = dsl::p<sql_schema_statement>;
        static constexpr auto value =
            lexy::construct<ast::sql_executable_statement>;
    };
} // namespace

std::optional<melinda::mdbsql::ast::sql_executable_statement>
melinda::mdbsql::parse(std::string_view statement)
{
    lexy::range_input<lexy::utf8_char_encoding,
        std::string_view::const_iterator> const range {std::cbegin(statement),
        std::cend(statement)};

    auto result =
        lexy::parse<sql_executable_statement>(range, lexy_ext::report_error);
    if (!result || !result.has_value())
    {
        lexy::trace<sql_executable_statement>(stdout, range);
        MBLTRC_TRACE_ERROR("Parsing of statement '{}' failed", statement);
        return std::nullopt;
    }

    return {std::move(result).value()};
};
