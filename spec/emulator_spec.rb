require 'spec_helper'
require 'digest/sha1'

include GME

describe Emulator, "when first created" do
  
  before(:each) do
    path = File.expand_path(File.dirname(__FILE__) + '/fixtures/sample.spc')
    @song = Emulator.open(path)
  end
  
  it "should not be nil" do 
    @song.should_not be_nil
  end
  
  it "should acknowledge 1 track" do 
    @song.track_count.should == 1
  end

  it "should respond to the message 'info' and return a non-empty container" do 
    @song.info.should_not be_empty
  end

  it "should correctly return the metadata of track 0 in a hash" do 
    @song.info[:game].should == "Super Castlevania 4"
    @song.info[:song].should == "Stage Clear"
    @song.info[:length].should == 4000
    @song.info[:copyright].should == "1991 Konami"
    @song.info[:author].should == "Masanori Adachi, Taro Kudou"
    @song.info[:play_length].should == 4000
    @song.info[:comment].should == ""
    @song.info[:dumper].should == "Datschge"
    @song.info[:system].should == "Super Nintendo"
    @song.info[:loop_length].should == -1
  end

  it "should indicate that no track has been started" do 
    @song.track_started?.should be_false
  end

  it "should raise an exception when asked if the current track has ended" do 
    lambda { @song.track_ended? }.should raise_exception(TrackNotStarted) 
  end

  it "should return 0 when asked for the time played since start" do 
    @song.tell.should == 0
  end

  it "should raise and exception if get_samples is called" do 
    lambda { @song.get_samples }.should raise_exception(TrackNotStarted)
  end

  context "when play_to_file is called" do
    
    context "when given an invalid file" do

      it "should raise an exception if the file argument is nil" do
        @file = nil
        lambda { @song.play_to_file(@file) }.should raise_exception(GenericException)
      end

      it "should raise an exception if the file argument is not of File class" do 
        @file = StringIO.new
        lambda { @song.play_to_file(@file) }.should raise_exception(GenericException)
      end
      
    end

    context "when given a valid file" do
      
      before(:each) do 
        @file = File.new("temp.out", "w+")
      end
      
      it "should play track 0 and write the samples to the specified file" do 
        lambda { @song.play_to_file(@file) }.should_not raise_exception
        @song.close
        File.size("temp.out").should == 706560
        Digest::SHA1.hexdigest(File.read("temp.out")).should == "5a557efdb64cb71baa953b7129663b1503f4a7d9"
      end

      after(:each) do 
        @file.close unless @file.closed?
        File.delete("temp.out") if File.exists?("temp.out")
      end
      
    end
  end

  context "when starting a track" do
    
    it "should start the specified track and return that number when successful" do 
      @song.start_track(0).should == 0
    end

    it "should start track 0 when no track number is specified" do 
      @song.start_track.should == 0
    end

    it "should raise an exception when the specified track number is invalid" do 
      lambda { @song.start_track(1) }.should raise_exception(GenericException)
    end
  end

  context "when a track has been started" do 
    before(:each) do 
      @song.start_track(0)
    end

    it "should indicate that a track has been started when asked" do 
      @song.track_started?.should be_true
    end
    
    it "should indicate that the track has not ended" do 
      @song.track_ended?.should be_false
    end
    
    it "should return 0 as the time played since the start of the track" do 
      @song.tell.should == 0
    end

    # TODO: get_samples, open with options, etc.
    
  end
end
