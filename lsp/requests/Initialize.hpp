#pragma once

#include "Marshal.hpp"
#include "json/utils.hpp"

namespace goos {
  namespace lsp::request {
    struct Initialize final {
      struct {
        Option<WideString> name;
        Option<WideString> version;
      } client;

      // auto marshal() const -> Box<json::Value> override { return json::object(); }
    };
  } // namespace lsp::request

  namespace json {
    template<>
    struct UnMarshal<lsp::request::Initialize> {
      auto operator()(const json::Value &value) -> Option<lsp::request::Initialize> {
        return value.try_as<Object>().flat_map([](const json::Object &obj) {
          return obj
            .get_as<Object>(L"clientInfo")

            // Process 'client' obj
            .map([](const Object &client) {
              return lsp::request::Initialize{
                client.get_as<Text>(L"name").map([](const auto &txt) { return txt->get_text(); }),
                client.get_as<Text>(L"version").map([](const auto &txt) { return txt->get_text(); })
              };
            });
        });
      }
    };
  } // namespace json

  static_assert(json::unmarshallable<lsp::request::Initialize>, "Ilformed Struct");

} // namespace goos
