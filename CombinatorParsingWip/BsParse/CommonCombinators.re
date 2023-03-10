open Combinators;

let eof = regex("$") |> slice <$> (_ => ()) |> label("Expected EOF");

let whitespace = regex("[\s]*") |> slice <$> (_ => ());

let spaceAround = bodyP =>
  whitespace >>= (_ => bodyP >>= (result => whitespace <$> (_ => result)));

let surround = (openP, bodyP, closeP) =>
  openP >>= (_ => spaceAround(bodyP) >>= (result => closeP <$> (_ => result)));

let number = regex("-?\d+(?:\.\d*)?(?:[eE][+\-]?\d+)?") |> slice;

let str = regex("\"([^\"]*)\"") <$> (matches => matches[1]);

let endLine = string("\n") <|> (lazy(string("\r\n")));
