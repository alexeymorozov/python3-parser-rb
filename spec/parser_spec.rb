require 'spec_helper'

class SnippetVisitor < Python3Parser::Visitor
  attr_reader :snippets

  def initialize
    @snippets = []
    super
  end
end

describe Python3Parser::Parser do
  let(:code) do
    # https://rosettacode.org/wiki/Remove_duplicate_elements#Python
    <<~END
      items = [1, 2, 3, 'a', 'b', 'c', 2, 3, 4, 'b', 'c', 'd']
      unique = []
      helperset = set()
      for x in items:
          if x not in helperset:
              unique.append(x)
              helperset.add(x)
    END
  end

  let(:parser) { Python3Parser::Parser.parse(code) }

  context '#visit' do
    before { parser.visit(visitor) }

    context 'atoms in a list' do
      subject { visitor.snippets }

      let(:visitor) do
        Class.new(SnippetVisitor) do
          def visit_testlist_comp(ctx)
            @snippets += ctx.test.map do |t|
              [:test, t.text]
            end

            visit_children(ctx)
          end
        end.new
      end

      it 'contains list atoms' do
        expect(subject).to eq([
          [:test, '1'],
          [:test, '2'],
          [:test, '3'],
          [:test, "'a'"],
          [:test, "'b'"],
          [:test, "'c'"],
          [:test, '2'],
          [:test, '3'],
          [:test, '4'],
          [:test, "'b'"],
          [:test, "'c'"],
          [:test, "'d'"],
        ])
      end
    end

    context 'list comprehensions' do
      let(:visitor) do
        Class.new(Python3Parser::Visitor) do
          attr_reader :exprlist, :testlist

          def visit_for_stmt(ctx)
            @exprlist = ctx.exprlist.text
            @testlist = ctx.testlist.text
            visit_children(ctx)
          end
        end.new
      end

      it 'ensures the test and expr lists are correct' do
        expect(visitor.exprlist).to eq('x')
        expect(visitor.testlist).to eq('items')
      end
    end

    context 'atoms and trailers' do
      subject { visitor.snippets }

      let(:visitor) do
        Class.new(SnippetVisitor) do
          def visit_atom_expr(ctx)
            if ctx.trailer.size > 0
              @snippets << [:call, ctx.atom.text, *ctx.trailer.map(&:text)]
            end
          end
        end.new
      end

      it 'identifies method calls' do
        expect(subject).to eq([
          [:call, 'set', '()'],
          [:call, 'unique', '.append', '(x)'],
          [:call, 'helperset', '.add', '(x)']
        ])
      end
    end
  end
end
