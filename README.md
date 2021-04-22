# Another One draft lexer/parser in c

Just a attempt to implement a lexer, parser in pure C without a lexer parser generator.

**Date:** 2019-2020

## Input

```
main() {
	5+5;
}
```

## Output

```
- main(ident)
- ((operator)
- )(operator)
- {(operator)
- 5(number)
- +(operator)
- 5(number)
- ;(operator)
- }(operator)
```

## Using

```bash
$ gcc main.c -o one
$ ./one input.one
```

So let me remind again that the license is GPL 3.

© Copyright Max Base 2019
