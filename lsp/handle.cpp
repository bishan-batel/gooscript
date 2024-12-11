#include "handle.hpp"
#include <cmath>
#include <functional>
#include "Marshal.hpp"
#include "Request.hpp"
#include "json/Object.hpp"
#include "logging.hpp"
#include "requests/Initialize.hpp"
#include "responses/InitializeResult.hpp"
#include "rpc.hpp"

namespace goos::lsp {
  auto initialize(rpc::Id id, const json::Object &params) -> bool {
    return json::unmarshal<request::Initialize>(params)
      .if_some([id](const request::Initialize &request) {
        log::info(
          "Initialized Request {}@v{}",
          request.client.name.get_or(L"Unknown Client Name"),
          request.client.version.get_or(L"Unknown Version")
        );

        rpc::send_result(response::Initialize{id});
      })
      .if_none([] { log::error("Incorrect 'initialize' params"); })
      .is_some();
  }

  auto handle_message(Request request, Box<json::Object> content) -> bool {
    Option<RefMut<json::Object>> params_opt = content->get_as<json::Object>(L"params");

    if (params_opt.is_none()) {
      log::error("No 'params'");
      return false;
    }

    static const Dictionary<WideStringView, std::function<bool(json::Object & params)>> handlers = {
      {L"initialize", [&](const auto &params) { return initialize(request.id, params); }},
    };

    if (not handlers.contains(request.method)) {
      log::error("No handler for method: '{}'", request.method);
      return false;
    }

    log::trace("Found handler for method");
    return handlers.at(request.method)(params_opt.take_unchecked());
  }
} // namespace goos::lsp
