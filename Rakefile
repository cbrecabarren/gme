# -*- encoding: utf-8 -*-
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

namespace :gme do 
  desc "Builds gme_ext and deploys locally on lib/gme_ext.so"
  task :extbuild do
    # cd to ext/gme/build
    # run extconf.rb
    # run make
    # cp gme_ext.so to lib/
    puts ">> Compiling gme_ext.so ..."
    path = File.expand_path(File.dirname(__FILE__) + '/ext/gme/build')
    `cd #{path}; ruby ../extconf.rb; make; cp gme_ext.so ../../../lib/gme_ext.so`
  end
  
  desc "Runs clean task in build directory and deletes lib/gme_ext.so"
  task :extclean do
    path = File.expand_path(File.dirname(__FILE__) + '/ext/gme/build')
    `cd #{path}; make clean; rm ../../../lib/gme_ext.so`
  end
end

task :spec => "gme:extbuild"

task :default => :spec
