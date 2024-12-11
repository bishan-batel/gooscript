#pragma once

#include <preamble.hpp>
#include <result.hpp>
#include <variant>
#include "Marshal.hpp"
#include "json/Number.hpp"
#include "json/utils.hpp"
#include "rpc.hpp"

namespace goos {
  namespace lsp {
    struct Request final : json::IMarshallable {
      rpc::Id id;
      WideString method;

      Request(decltype(id) id, decltype(method) method) : id{id}, method{std::move(method)} {}

      auto marshal() const -> Box<json::Value> override {
        Box<json::Object> obj = json::object();

        obj->put(L"id", std::visit(crab::cases{[](auto &i) -> Box<json::Value> { return json::to_json(i); }}, id));
        obj->put(L"method", method);

        return obj;
      }
    };
  } // namespace lsp

  template<>
  struct json::UnMarshal<lsp::Request> {
    auto operator()(const Value &val) -> Option<lsp::Request> {
      Option<Ref<Object>> obj_res = val.try_as<Object>();

      if (not obj_res) {
        return crab::none;
      }

      const Object &obj = obj_res.take_unchecked();

      lsp::Request request{0, L""};

      // request.id = obj.get_as<Number>(L"id")
      //              .map([&](Ref<Number> ref) { return static_cast<i64>(ref->get()); }) //
      //              .get_or([&] { return 0_i64; });

      if (auto num = obj.get_as<Number>(L"id")) {
        request.id = static_cast<i64>(num.take_unchecked()->get());
      } else if (auto num_str = obj.get_as<Text>(L"id")) {
        request.id = goos::str::convert((num_str.take_unchecked()->get_text()));
      } else {
        return crab::none;
      }

      if (auto method = obj.get_as<Text>(L"method")) {
        request.method = method.take_unchecked()->get_text();
      } else {
        return crab::none;
      }

      return crab::some(std::move(request));
    }
  };

  static_assert(json::full_marshallable<lsp::Request>, "lsp::Request is illformed");
} // namespace goos
