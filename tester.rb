require 'python3-parser'

class MyVisitor < Python3Parser::Visitor
  def visit_atom_expr(ctx)
    puts ctx.text
    visit_children(ctx)
  end
end

visitor = MyVisitor.new
parser = Python3Parser::Parser.parse_file('/Users/cameron/workspace/pyrb-runtime/spec/integration/source/allergies.py')

parser.visit(visitor)
