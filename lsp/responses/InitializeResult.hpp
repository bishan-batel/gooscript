#pragma once

#include "Marshal.hpp"
#include "json/utils.hpp"
#include "rpc.hpp"

namespace goos {
  namespace lsp::response {
    struct Initialize final : json::IMarshallable {
      rpc::Id id;

      Initialize(rpc::Id id) : id{std::move(id)} {}

      auto marshal() const -> Box<json::Value> override {
        Box<json::Object> obj = json::object(), result = json::object();

        {
          auto capabilities = json::object();
          result->put(L"hoverProvider", json::to_json(true));

          result->put(L"capabilities", std::move(capabilities));
        }

        obj->put(L"result", std::move(result));

        std::visit(crab::cases{[&](auto x) { obj->put(L"id", json::to_json(x)); }}, id);

        return obj;
      }
    };
  } // namespace lsp::response

  static_assert(json::marshallable<lsp::response::Initialize>, "Ilformed Struct");

} // namespace goos
