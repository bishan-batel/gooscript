#include <crab/debug.hpp>
#include <vector>
#include "token/Token.hpp"
namespace goos::ast {

  class TokenTrace {
    Rc<token::Token> beginning, end;

  public:
    TokenTrace(Rc<token::Token> beginning, Rc<token::Token> end)
        : beginning{std::move(beginning)}, end{std::move(end)} {}

    TokenTrace(Rc<token::Token> token) : TokenTrace{token, std::move(token)} {}

    template<typename... Args>
    static auto from_unordered(Vec<Rc<token::Token>> tokens) -> TokenTrace {
      debug_assert(not tokens.empty(), "Cannot create a token trace with no tokens.");

      usize min_index = 0, min_source_index = 0;
      usize max_index = 0, max_source_index = 0;

      for (usize i = 0; i < tokens.size(); i++) {
        const token::Token &tok = tokens[i];

        if (const usize lower_bound = tok.get_range().lower_bound(); lower_bound < min_source_index) {
          min_source_index = lower_bound;
          min_index = i;
        }

        if (const usize upper_bound = tok.get_range().upper_bound(); upper_bound > max_source_index) {
          max_source_index = upper_bound;
          max_index = i;
        }
      }

      return TokenTrace{tokens[min_index], tokens[max_index]};
    }

    template<std::same_as<TokenTrace>... Traces>
    [[nodiscard]] static auto
    merge_fold(const TokenTrace &first, const Traces &...rest) -> TokenTrace {
      if constexpr (sizeof...(rest) == 1) {
        return first.merge(rest...);
      } else {
        return first.merge_fold(rest...);
      }
    }

    [[nodiscard]] auto merge(const TokenTrace &trace) const -> TokenTrace {
      return TokenTrace{
          beginning->get_range().lower_bound() < trace.beginning->get_range().lower_bound() ? beginning
                                                                                            : trace.beginning,
          end->get_range().upper_bound() < trace.end->get_range().upper_bound() ? end : trace.end,
      };
    }

    [[nodiscard]] auto get_beginning() const -> Rc<token::Token> { return beginning; }

    [[nodiscard]] auto get_end() const -> Rc<token::Token> { return end; }
  };

} // namespace goos::ast
