export const STRING_LITERAL = 0;
export const NUMBER = 1;
export const BOOL = 2;
export const OPEN_CURLY = 3;
export const CLOSE_CURLY = 4;
export const OPEN_BRACKET = 5;
export const CLOSE_BRACKET = 6;
export const COLON = 7;
export const COMMA = 8;

export const TY_TO_STR = [
    "STRING_LITERAL",
    "NUMBER",
    "BOOL",
    "OPEN_CURLY",
    "CLOSE_CURLY",
    "OPEN_BRACKET",
    "CLOSE_BRACKET",
    "COLON",
    "COMMA"
];


export const new_token = (ty, data, to_string) => ({
    type: ty,
    data: data,
    to_string: () => TY_TO_STR[ty] + ": " + to_string()
});

const unit = undefined;

export const string = (data) => new_token(STRING_LITERAL, data, () => "'" + data + "'");

export const num = (data) => new_token(NUMBER, data, () => data);

export const bool = (data) => new_token(BOOL, data, () => data ? "true" : "false");

export const open_curly = () => new_token(OPEN_CURLY, unit, () => "{{"[0]);

export const close_curly = () => new_token(CLOSE_CURLY, unit, () => "}");

export const open_bracket = () => new_token(OPEN_BRACKET, unit, () => "[");

export const close_bracket = () => new_token(CLOSE_BRACKET, unit, () => "]")

export const colon = () => new_token(COLON, unit, () => ":")

export const comma = () => new_token(COMMA, unit, () => ",")
