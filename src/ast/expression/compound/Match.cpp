//
// Created by bishan_ on 6/19/24.
//

#include "Match.hpp"

#include <algorithm>
#include <ranges>

#include <range.hpp>
#include <sstream>

#include "json/Array.hpp"
#include "json/Object.hpp"

namespace goos::parser::pass::expr {
  Match::Match(Box<Expression> state, Vec<Case> cases, Box<Expression> default_case)
    : state{std::move(state)},
      cases{std::move(cases)},
      default_case{std::move(default_case)} {}

  auto Match::operator==(const Statement &statement) const -> bool {
    if (auto op = statement.try_as<Match>()) {
      auto &match = *op.take_unchecked();

      const auto case_eq = [](const Case &a, const Case &b) {
        return *a.state == b.state and *a.then == b.then;
      };

      if (cases.size() != match.cases.size()) {
        return true;
      }

      if (*state != match.state) {
        return true;
      }

      if (*default_case != match.default_case) {
        return true;
      }

      for (const usize i: crab::range(cases.size())) {
        if (not case_eq(cases[i], match.cases[i])) {
          return false;
        }
      }

      return true;
    }

    return false;
  }

  auto Match::to_string() const -> WideString {
    WideStringStream stream{};

    stream << "match {\n";
    for (const auto &[state, then]: cases) {
      stream << state->to_string() << " => " << then->to_string() << "\n";
    }
    stream << "default => " << default_case->to_string() << "\n";
    stream << "}";

    return stream.str();
  }

  auto Match::json() const -> Box<json::Value> {
    auto json = crab::make_box<json::Object>();

    json->put(L"type", L"match");
    json->put(L"state", state->json());

    auto arr = crab::make_box<json::Array>();
    for (const auto &[state, then]: cases) {
      auto obj = crab::make_box<json::Object>();
      obj->put(L"state", state->json());
      obj->put(L"then", then->json());
      arr->push(std::move(obj));
    }
    json->put(L"state", state->json());
    json->put(L"cases", std::move(arr));
    json->put(L"default", default_case->json());

    return json;
  }

  auto Match::clone_expr() const -> Box<Expression> {
    Vec<Case> cases{};
    for (const auto &[state, then]: cases) {
      cases.push_back(Case{state->clone_expr(), then->clone_expr()});
    }
    return crab::make_box<Match>(
      state->clone_expr(),
      std::move(cases),
      default_case->clone_expr()
    );
  }

  auto Match::accept_expr(IVisitor &visitor) const -> Result<std::any, Box<crab::Error>> {
    return visitor.visit_match(*this);
  }
}
