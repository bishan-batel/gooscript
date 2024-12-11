//
// Created by bishan_ on 6/18/24.
//

#pragma once

#include "interfaces/ICallable.hpp"
#include "ast/expression/literal/Lambda.hpp"

namespace goos::runtime {
  class Closure final : public ICallable {
    Box<ast::expression::Lambda> lambda;
    RcMut<Environment> captured;

  public:
    explicit Closure(RcMut<Environment> captured, Box<ast::expression::Lambda> lambda);

    [[nodiscard]] auto call(Intepreter &runtime, const Vec<Any> &values) const -> Result<Any, Box<err::Error>> override;

    [[nodiscard]] auto get_arity_type() const -> ArityType override;

    [[nodiscard]] auto get_arity() const -> usize override;

    [[nodiscard]] auto clone() const -> Any override;

    [[nodiscard]] auto to_string() const -> WideString override;

    [[nodiscard]] auto get_type() const -> meta::VariantType override;

    [[nodiscard]] auto base_hash() const -> utils::hash_code override;
    [[nodiscard]] auto to_json() const -> Box<json::Value> override;
  };
} // goos
