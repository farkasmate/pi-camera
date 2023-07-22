module PiCamera
  class Capture
    def initialize
      still_config = Cam::StreamConfig.new(four_cc: Cam::FourCC::BGR888)
      viewfinder_config = Cam::StreamConfig.new(four_cc: Cam::FourCC::YUV420, width: 162, height: 122)

      cam = Cam.new(still_config: still_config, viewfinder_config: viewfinder_config, transform: Cam::Transform::ROT180)
      cam.capture do |still, viewfinder, focus|
        puts "Drawing viewfinder..."
        pp! still, viewfinder, focus

        File.write("/tmp/image.jpg", still.to_jpeg)

        true
      end
    end
  end
end
