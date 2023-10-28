require "pluto/format/binding/lib_jpeg_turbo"
require "yaml"

require "./enums"

module PiCamera
  class ImageData
    include YAML::Serializable

    @[YAML::Field(ignore: true)]
    property bytes = Bytes.new(0)

    property width, height, stride, format

    property raw_file : String = ""

    def initialize(@bytes : Bytes, @width : UInt32, @height : UInt32, @stride : UInt32, @format : Cam::FourCC)
    end

    def to_jpeg : Bytes
      case format
      when Cam::FourCC::BGR888
        return compress_rgb
      when Cam::FourCC::YVYU
        return compress_yvyu
      else
        raise "Can't convert format: #{format}"
      end
    end

    private def jpeg_turbo_compress(data : Pointer(UInt8), width : Int, height : Int, stride : Int, quality : Int = 100)
      handle = LibJPEGTurbo.init_compress
      buffer = Pointer(UInt8).null
      LibJPEGTurbo.compress2(
        handle,
        data,
        width,
        stride,
        height,
        LibJPEGTurbo::PixelFormat::RGB,
        pointerof(buffer),
        out size,
        LibJPEGTurbo::Subsampling::S444,
        quality,
        0
      )
      LibJPEGTurbo.destroy(handle)

      bytes = Bytes.new(buffer, size).clone
      LibJPEGTurbo.free(buffer)

      return bytes
    end

    private def compress_rgb
      return jpeg_turbo_compress(bytes.to_unsafe, width, height, stride, 95)
    end

    private def to_rgb(y : UInt8, u : UInt8, v : UInt8) : {UInt8, UInt8, UInt8}
      cr = u.to_f
      cb = v.to_f
      r = y.to_f + 1.402 * (cr - 128)
      g = y.to_f - 0.344 * (cb - 128) - 0.714 * (cr - 128)
      b = y.to_f + 1.772 * (cb - 128)

      return r.clamp(0, 255).to_u8, g.clamp(0, 255).to_u8, b.clamp(0, 255).to_u8
    end

    private def compress_yvyu
      size = width * height

      red = Array(UInt8).new(size, 0)
      green = Array(UInt8).new(size, 0)
      blue = Array(UInt8).new(size, 0)
      alpha = Array(UInt8).new(size, 255)
      bytes.each_slice(4).with_index do |pixel, index|
        y1 = pixel[0]
        v = pixel[1]
        y2 = pixel[2]
        u = pixel[3]

        r1, g1, b1 = to_rgb(y1, u, v)
        r2, g2, b2 = to_rgb(y2, u, v)

        red[2 * index] = r1
        red[2 * index + 1] = r2
        green[2 * index] = g1
        green[2 * index + 1] = g2
        blue[2 * index] = b1
        blue[2 * index + 1] = b2
      end

      data = IO::Memory.new(size * 3)
      size.times do |index|
        data.write_byte(red.unsafe_fetch(index))
        data.write_byte(green.unsafe_fetch(index))
        data.write_byte(blue.unsafe_fetch(index))
      end

      return jpeg_turbo_compress(data.buffer, width, height, 0, 95)
    end
  end
end
