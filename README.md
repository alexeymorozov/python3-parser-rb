# python3-parser-rb

A Python 3 parser for Ruby.

## Usage

Parse some Python 3 code and walk over the parse tree with a visitor:

```ruby
require 'python3-parser'

# from https://rosettacode.org/wiki/Roman_numerals/Decode#Python
python_code = <<~END
  _rdecode = dict(zip('MDCLXVI', (1000, 500, 100, 50, 10, 5, 1)))

  def decode(roman):
      result = 0
      for r, r1 in zip(roman, roman[1:]):
          rd, rd1 = _rdecode[r], _rdecode[r1]
          result += -rd if rd < rd1 else rd
      return result + _rdecode[roman[-1]]

  if __name__ == '__main__':
      for r in 'MCMXC MMVIII MDCLXVI'.split():
          print(r, decode(r))
END

class MyFuncVisitor < Python3Parser::Visitor
  def visit_funcdef(ctx)
    puts ctx.NAME.text
    visit_children(ctx)
  end
end

parser = Python3Parser::Parser.parse(python_code)
parser.visit(MyFuncVisitor.new)
```

## Caveats

See the caveats listed in [antlr4-native's README](https://github.com/camertron/antlr4-native-rb#caveats).

## System Requirements

See the system requirements listed in [antlr4-native's README](https://github.com/camertron/antlr4-native-rb#system-requirements).

## License

Licensed under the MIT license. See LICENSE.txt for details.

## Authors

* Cameron C. Dutro: http://github.com/camertron
