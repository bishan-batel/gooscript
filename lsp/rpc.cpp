#include "rpc.hpp"
#include <box.hpp>
#include <cmath>
#include <sstream>
#include "json/Object.hpp"
#include "lexer/Lexer.hpp"
#include "logging.hpp"
#include "parser/TokenStream.hpp"
#include "parser/pass/statement/block.hpp"
#include "runtime/Intepreter.hpp"

namespace goos::lsp::rpc {
  // auto log() -> std::wofstream & {
  //   static std::wofstream log{"gooscript-lsp.log"};
  //   return log;
  // }

  auto index_of_seperator(Span<u8> bytes) -> Option<usize> {
    for (usize i = 0; i + SEPERATOR.length() < bytes.size(); i += SEPERATOR.length()) {
      bool found = true;

      for (usize j = 0; j < SEPERATOR.size(); j++) {
        if (bytes[i + j] != std::bit_cast<u8>(SEPERATOR[j])) {
          found = false;
          break;
        }
      }

      if (found) {
        return crab::some(i);
      }
    }

    return crab::none;
  }

  auto is_msg_ready(const Span<u8> bytes) -> Result<bool> {
    Option<usize> seperator_index = index_of_seperator(bytes);

    if (seperator_index.is_none()) {
      return false;
    }

    String content_length_str{};

    for (auto i = HEADER.size(); i < seperator_index.get_unchecked(); i++) {
      content_length_str.push_back(static_cast<char>(bytes[i]));
    }

    usize content_length = 0;
    std::istringstream int_stream{content_length_str};
    int_stream >> content_length;

    if (int_stream.fail()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::INVALID_CONTENT_LENGTH);
    }

    // return bytes.size() - seperator_index - SEPERATOR.size() == content_length;
    // return content_length + seperator_index + SEPERATOR.size() == bytes.size();

    // if (bytes.size() - seperator_index < content_length + SEPERATOR.size()) {
    //   return false;
    // }

    return bytes.size() >= content_length + seperator_index.get_unchecked() + SEPERATOR.size();
  }

  auto decode(const Span<u8> bytes) -> Result<Box<json::Object>> {
    log::trace("Start");

    Option<usize> seperator_index = index_of_seperator(bytes);

    if (seperator_index.is_none()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::FAILED_TO_FIND_SEPERATOR);
    }

    log::trace("Getting Content Length");
    String content_length_str{};

    for (auto i = HEADER.size(); i < seperator_index.get_unchecked(); i++) {
      content_length_str.push_back(static_cast<char>(bytes[i]));
    }

    usize content_length = 0;
    std::istringstream int_stream{content_length_str};
    int_stream >> content_length;

    if (int_stream.fail()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::INVALID_CONTENT_LENGTH);
    }

    WideString content;
    content.reserve(content_length);

    // for (const u8 byte: bytes.subspan(seperator_index.get_unchecked() + SEPERATOR.length())) {
    //   content.push_back(static_cast<widechar>(byte));
    // }

    for (usize i = 0; i < content_length; i++) {
      content.push_back(static_cast<widechar>(bytes[i + SEPERATOR.length() + seperator_index.get_unchecked()]));
    }

    log::trace("DECODE: Tokening");

    lexer::Result<lexer::TokenList> tokens = lexer::Lexer::tokenize(SourceFile::create("", content));
    if (tokens.is_err()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::MALFORMED_JSON);
    }

    log::trace("DECODE: Parsing");

    parser::TokenStream stream{tokens.take_unchecked()};
    parser::MustEvalResult<ast::expression::ScopeBlock> ast = parser::pass::block_top_level(stream);

    if (ast.is_err()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::MALFORMED_JSON);
    }

    log::trace("DECODE: Intepreter");

    runtime::Intepreter intepreter{};
    ::Result<runtime::Any, Box<runtime::err::Error>> evaluated_expr = intepreter.evaluate(ast.take_unchecked());

    if (evaluated_expr.is_err()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::MALFORMED_JSON);
    }

    log::trace("DECODE: Coercing to Object");
    Box<json::Value> val = evaluated_expr.take_unchecked()->to_json();

    if (val->try_as<json::Object>().is_none()) {
      return err::make<err::MalformedDecode>(err::MalformedDecode::Type::MALFORMED_JSON);
    }

    log::trace("DECODE: Success");

    return Box<json::Object>::wrap_unchecked(dynamic_cast<json::Object *>(Box<json::Value>::unwrap(std::move(val))));
  }

} // namespace goos::lsp::rpc
