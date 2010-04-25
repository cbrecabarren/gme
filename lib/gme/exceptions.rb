module GME
  # describes a generic exception, related to this gem
  class GenericException < Exception
  end

  # thrown when a path supplied doesn't point to a valid file for the library
  class InvalidFile < GenericException
  end

  # thrown when one tries to play a file without starting a track first
  class TrackNotStarted < GenericException
  end
end
