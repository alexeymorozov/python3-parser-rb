require 'python-parser'
require 'benchmark/ips'

class MyVisitor < Python3Parser::Visitor
  def visit_atom_expr(ctx)
    visit_children(ctx)
  end
end

visitor = MyVisitor.new
parser = Python3Parser::Parser.parse("print('hello, world')\n")

Benchmark.ips do |x|
  x.report do
    parser.visit(visitor)
  end
end
