module PiCamera
  class Cam
    enum FourCC : UInt32
      BGR888 =  875710274
      YUV420 =  842093913
      YVYU   = 1448695129
    end

    enum Orientation
      ROTATE0         = 1
      ROTATE0MIRROR
      ROTATE180
      ROTATE180MIRROR
      ROTATE90MIRROR
      ROTATE270
      ROTATE270MIRROR
      ROTATE90
    end
  end
end
