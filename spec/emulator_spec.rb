require 'spec_helper'

include GME

describe Emulator, "when first created" do
  before(:all) do
    path = File.expand_path(File.dirname(__FILE__) + '/fixtures/sample.spc')
    @song = Emulator.open(path)
  end
  it "should not be nil" do 
    @song.should_not be_nil
  end
  
  it "should contain 1 track" do 
    @song.track_count.should == 1
  end

  it "should load the metadata in a container and respond to the message 'info'" do 
    @song.info.should_not be_empty
  end

  it "should indicate that no track has been started" do 
    @song.track_started?.should be_false
  end
end
