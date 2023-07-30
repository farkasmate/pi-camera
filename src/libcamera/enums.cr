module PiCamera
  class Cam
    enum FourCC : UInt32
      BGR888 = 875710274
      YUV420 = 842093913
      YVYU = 1448695129
    end

    enum Transform
      IDENTITY = 0
      ROT0 = IDENTITY
      HFLIP = 1
      VFLIP = 2
      HVFLIP = HFLIP | VFLIP
      ROT180 = HVFLIP
      TRANSPOSE = 4
      ROT270 = HFLIP | TRANSPOSE
      ROT90 = VFLIP | TRANSPOSE
      ROT180TRANSPOSE = HFLIP | VFLIP | TRANSPOSE
    end
  end
end
