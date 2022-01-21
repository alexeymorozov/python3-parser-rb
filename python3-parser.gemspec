$:.unshift File.join(File.dirname(__FILE__), 'lib')
require 'python3-parser/version'

Gem::Specification.new do |s|
  s.name     = 'python3-parser'
  s.version  = ::Python3Parser::VERSION
  s.authors  = ['Cameron Dutro']
  s.email    = ['camertron@gmail.com']
  s.homepage = 'http://github.com/camertron/python3-parser-rb'

  s.description = s.summary = 'A Python 3 parser for Ruby.'

  s.platform = Gem::Platform::RUBY
  s.has_rdoc = true

  s.add_dependency 'rice', '~> 4.0'

  s.extensions = File.join(*%w(ext python3-parser extconf.rb))

  s.require_path = 'lib'
  s.files = Dir[
    '{lib,spec}/**/*',
    'ext/python3-parser/*.{rb,cpp,h}',
    'ext/python3-parser/antlrgen/*',
    'ext/python3-parser/antlr4-upstream/runtime/Cpp/runtime/src/**/*.{cpp,h}',
    'Gemfile',
    'README.md',
    'Rakefile',
    'python3-parser.gemspec'
  ]
end
