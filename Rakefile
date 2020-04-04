require 'antlr4-native'
require 'pry-byebug'

task :generate do
  generator = Antlr4Native::Generator.new(
    grammar_files:      ['Python3.g4'],
    output_dir:         'ext/python-parser/',
    parser_root_method: 'file_input'
  )

  generator.generate
end
