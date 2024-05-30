require "pluto/format/binding/lib_jpeg_turbo"


def jpeg_turbo_compress(data : Pointer(UInt8), width : Int, height : Int, stride : Int, quality : Int = 100)
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

pic = File.read("/tmp/pic")

channel = Channel(Nil).new(10)

10.times do |i|
  spawn do
    start = Time.monotonic
    jpeg_data = jpeg_turbo_compress(pic.to_unsafe, 4056, 3040, 12192, 95)
    puts "JPEG conversion took: #{(Time.monotonic - start).total_milliseconds}"

    start_write = Time.monotonic
    File.write("/tmp/test_#{i}.jpeg", jpeg_data)
    puts "Writing JPEG took: #{(Time.monotonic - start_write).total_milliseconds}"

    puts "TOTAL: #{(Time.monotonic - start).total_milliseconds}"
    channel.send(nil)
  end
end

10.times { channel.receive }
