open Util;

module type Parsers = {
  type parser('a);
  type parse_error = ParseError.t;
  // type result(('a, Location.t), parse_error);
  let runParser:
    (parser('a), Location.t) => result(('a, Location.t), parse_error);
  let run: (parser('a), string) => result(('a, Location.t), parse_error);
  let get_exn: result(('a, Location.t), parse_error) => 'a;
  let biMapResult: ('a => 'b, 'c => 'b, result(('a, 'd), 'c)) => 'b;
  let get_error: result(('a, Location.t), parse_error) => parse_error;
  let string: string => parser(string);
  let orElse: (parser('a), Lazy.t(parser('a))) => parser('a);
  let flatMap: (parser('a), 'a => parser('b)) => parser('b);
  let unit: 'a => parser('a);
  let listOfN: (int, parser('a)) => parser(array('a));
  let many: parser('a) => parser(array('a));
  let many1: parser('a) => parser(array('a));
  let slice: parser('a) => parser(string);
  let regex: string => parser(array(string));
  let sepBy: (parser('b), parser('a)) => parser(array('a));

  let label: (string, parser('a)) => parser('a);
  let scope: (string, parser('a)) => parser('a);

  let attempt: parser('a) => parser(option('a));
  let fail: string => parser('a);
};

module DerivedParsers = (PS: Parsers) => {
  open PS;

  let map = (parser, fn) => flatMap(parser, v => unit(fn(v)));

  let map2 = (parser1, parser2, fn) =>
    flatMap(parser1, v1 => map(Lazy.force(parser2), v2 => fn(v1, v2)));

  let product = (parser1, parser2) =>
    map2(parser1, parser2, (v1, v2) => (v1, v2));

  let char = c => (char_to_string(c) |> string)->(map(s => s.[0]));
};

module InfixOps = (PS: Parsers) => {
  module Derived_Parsers = DerivedParsers(PS);
  open PS;
  open Derived_Parsers;

  let (>>=) = flatMap;
  let (<$>) = map;
  let (>>) = product;
  let (<|>) = orElse;
};
