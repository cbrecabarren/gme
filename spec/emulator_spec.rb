require 'spec_helper'

include GME

describe Emulator, "when first created" do
  it "should do this" do 
    song = Emulator.open(File.expand_path(File.dirname(__FILE__) + '/fixtures/sample.spc'))

    song.should_not be_nil
  end
  
  it "should do something"
end
