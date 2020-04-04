require 'bundler'
require 'antlr4-native'

Bundler::GemHelper.install_tasks

task :generate do
  generator = Antlr4Native::Generator.new(
    grammar_files:      ['Python3.g4'],
    output_dir:         'ext/python3-parser/',
    parser_root_method: 'file_input'
  )

  generator.generate
end
