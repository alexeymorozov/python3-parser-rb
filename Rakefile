require 'bundler'
require 'antlr4-native'

Bundler::GemHelper.install_tasks

task :generate do
  generator = Antlr4Native::Generator.new(
    grammar_files:      ['Python3Lexer.g4', 'Python3Parser.g4'],
    output_dir:         'ext',
    parser_root_method: 'file_input'
  )

  generator.generate
end
