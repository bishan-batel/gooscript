import * as Token from "./parser_token.js";
import fs from "fs";

console.time();

// let original = import("fizzbuzz.goo.json")

function error(fmt_str) {
    console.log("[ERROR] ", fmt_str);
    process.exit(42);
}

let tokenize = (source) => {
    const length = source.length;


    const tokens = [];

    let i = 0;


    const WHITESPACE = /\s/;


    const not_eof = () => i < length;

    const curr = () => not_eof() ? `${source.charAt(i)}` : "";

    const next = () => {
        i = i + 1;
        return curr();
    }


    const DIGIT_REGEX = /[0-9]/;

    let digit = () => {
        if (!DIGIT_REGEX.test(curr())) {
            return false;
        }

        let num = curr()


        while (DIGIT_REGEX.test(next())) {
            num = num + curr()
        }


        if (curr() == ".") {
            num = num + "."
            while (DIGIT_REGEX.test(next())) {
                num = num + curr()
            }
        }

        tokens.push(Token.num(parseFloat(num)));

        return true;
    }


    let string = () => {
        if (curr() !== '"') return false;


        let literal = "";
        while (next() != '"') {
            literal = literal + curr();
        }
        next();

        tokens.push(Token.string(literal));

        return true;
    };

    while (not_eof()) {
        if (WHITESPACE.test(curr())) {
            next()
        } else if (curr() == "{") {
            tokens.push(Token.open_curly())
            next()
        } else if (curr() == "}") {
            tokens.push(Token.close_curly());
            next()
        } else if (curr() == "[") {
            tokens.push(Token.open_bracket());
            next();
        } else if (curr() == "]") {
            tokens.push(Token.close_bracket());
            next();
        } else if (curr() == ":") {
            tokens.push(Token.colon());
            next();
        } else if (curr() == ",") {
            tokens.push(Token.comma());
            next();
        } else if (digit()) {
        } else if (string()) {
        } else {
            console.log(`${i} / ${length}`);
            error("Invalid character: '" + curr() + "'");
        }
    }

    return tokens;
}


let file = fs.readFileSync("fizzbuzz.goo.json");

// console.log(JSON.parse(file.toString()));
// process.exit(0);

const toks = tokenize(file.toString())
const tok_len = toks.length;
let pos = 0;

let curr = () => toks[pos];

let is_curr = (ty) => curr().type == ty;

let next = () => pos = pos + 1;

let any = () => {
    if (is_curr(Token.OPEN_CURLY)) {
        return obj();
    } else if (is_curr(Token.OPEN_BRACKET)) {
        return array();
    } else {
        let data = curr().data
        next()
        return data;
    }
};


let obj = () => {
    next();
    let object = {};

    while (!is_curr(Token.CLOSE_CURLY)) {
        let key = any()
        next();

        object[key] = any();

        if (is_curr(Token.COMMA)) next();
    }
    next()

    return object
}

let array = () => {
    next();
    let arr = [];

    while (!is_curr(Token.CLOSE_BRACKET)) {
        arr.push(any())

        if (is_curr(Token.COMMA)) next();
    }
    next()

    return arr
}


let parsed = any();

console.log(JSON.stringify(parsed))

console.log(console.timeEnd());
