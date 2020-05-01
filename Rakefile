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

task :compile do
  Dir.chdir(File.join(%w(ext python3-parser))) do
    load 'extconf.rb'
    exec 'make -j 4'
  end
end
