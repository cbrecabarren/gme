# Generated by jeweler
# DO NOT EDIT THIS FILE DIRECTLY
# Instead, edit Jeweler::Tasks in Rakefile, and run the gemspec command
# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{gme}
  s.version = "0.0.9"
  s.platform = %q{x86-linux}

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Carlos Beltr\303\241n-Recabarren"]
  s.date = %q{2010-04-17}
  s.description = %q{libgme interface for Ruby}
  s.email = %q{cbrecabarren@gmail.com}
  s.extensions = ["ext/gme/extconf.rb", "ext/gme/extconf.rb"]
  s.extra_rdoc_files = [
    "README.rdoc"
  ]
  s.files = [
    "README.rdoc",
     "Rakefile",
     "VERSION",
     "ext/gme/extconf.rb",
     "ext/gme/gme.c",
     "ext/gme/gme_funcs.c",
     "ext/gme/gme_funcs.h",
     "ext/gme/util.c",
     "ext/gme/util.h",
     "lib/gme.rb",
     "lib/gme/exceptions.rb",
     "lib/gme/gme.rb"
  ]
  s.homepage = %q{http://www.beltran-recabarren.com}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib"]
  s.requirements = ["libgme v0.5.5 (http://code.google.com/p/game-music-emu/) by Blargg"]
  s.rubygems_version = %q{1.3.6}
  s.summary = %q{gme for Ruby}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 3

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end

