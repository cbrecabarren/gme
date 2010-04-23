require 'rubygems'

begin
  require 'jeweler'
  
  Jeweler::Tasks.new do |gemspec|
    gemspec.name = "gme"
    gemspec.summary = "gme for Ruby"
    gemspec.description = "libgme interface for Ruby"

    gemspec.authors << "Carlos Beltrán-Recabarren"
    gemspec.email = "cbrecabarren@gmail.com"
    gemspec.homepage = "http://www.beltran-recabarren.com"

    gemspec.extensions << "ext/gme/extconf.rb"
    gemspec.platform = Gem::Platform::CURRENT
    
    gemspec.requirements = "libgme v0.5.5 (http://code.google.com/p/game-music-emu/) by Blargg"
    gemspec.files = FileList['lib/**/*', 'ext/**/*', '[A-Z]*'].to_a
  end
  
  Jeweler::GemcutterTasks.new

rescue LoadError
  puts "Jeweler not available. Install it with: gem install jeweler"
end

require 'rake'
require 'spec'
require 'spec/rake/spectask'

spec_files = Rake::FileList["spec/**/*_spec.rb"]

desc "Run specs"
Spec::Rake::SpecTask.new do |t|
  t.spec_files = spec_files
  t.spec_opts = ["-c"]
end

task :default => :spec
