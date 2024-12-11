#include <fmt/xchar.h>
#include <fstream>
#include <iostream>
#include <preamble.hpp>
#include "Marshal.hpp"
#include "Request.hpp"
#include "handle.hpp"
#include "json/Object.hpp"
#include "logging.hpp"
#include "rpc.hpp"
#include "utils/str.hpp"

int main() {
  using namespace goos;
  using namespace goos::lsp;

  log::init_logfile();

  log::info("Started LSP");

  Vec<u8> bytes;

  while (true) {
    // Read byte
    {
      char byte = 0;
      std::cin.get(byte);
      bytes.push_back(std::bit_cast<u8>(byte));
    }

    {
      auto is_ready = rpc::is_msg_ready(bytes);
      if (is_ready.is_err()) {
        log::error(str::convert(is_ready.take_err_unchecked()->what()));
        bytes.clear();
        continue;
      }

      if (not is_ready.take_unchecked()) {
        // rpc::log() << "Not Ready [" << bytes.size() << "]: " << std::endl;
        continue;
      }
    }

    log::trace("Decoding Message:");

    lsp::Result<Box<json::Object>> res = rpc::decode(bytes);

    if (res.is_err()) {
      log::trace("Failed to Decode: {}", res.take_err_unchecked()->what());
      bytes.clear();
      continue;
    }

    Box<json::Object> obj = res.take_unchecked();
    auto cloned = obj->clone();

    if (not json::unmarshal<Request>(obj)
              .map([&](auto req) { return handle_message(req, std::move(obj)); }) //
              .get_or(false)) {
      log::error("Malformed Request key-value pair.");

      // log::error("Invalid Request:\n\n{}\n\n", );
      // cloned->write(rpc::log());
    }

    bytes.clear();
  }
}
