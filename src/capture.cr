require "./camera"
require "./dcim"
require "./fonts"
require "./frame"

module PiCamera
  class Capture
    @@bayer = [
      [0, 32, 8, 40, 2, 34, 10, 42],
      [48, 16, 56, 24, 50, 18, 58, 26],
      [12, 44, 4, 36, 14, 46, 6, 38],
      [60, 28, 52, 20, 62, 30, 54, 22],
      [3, 35, 11, 43, 1, 33, 9, 41],
      [51, 19, 59, 27, 49, 17, 57, 25],
      [15, 47, 7, 39, 13, 45, 5, 37],
      [63, 31, 55, 23, 61, 29, 53, 21],
    ]

    @click = false
    @finished = false

    def initialize(ui : Ui)
      Signal::USR1.trap { @click = true }
      Signal::USR2.trap { @finished = true }

      ui.mode Ui::Mode::Partial

      frame = Frame.new(width: 250, height: 122)
      still_config = Cam::StreamConfig.new(four_cc: Cam::FourCC::BGR888)
      viewfinder_config = Cam::StreamConfig.new(four_cc: Cam::FourCC::YUV420, width: 162, height: 122)

      cam = Cam.new(still_config: still_config, viewfinder_config: viewfinder_config, orientation: Cam::Orientation::ROTATE180)
      cam.capture do |still, viewfinder, focus|
        frame.clear

        if viewfinder
          viewfinder.height.times do |y|
            viewfinder.width.times do |x|
              frame.set(x, y, Frame::Color::Black) if viewfinder.bytes[y*viewfinder.stride + x] < 4 * @@bayer[x % 8][y % 8]
            end
          end
        end

        frame.draw(Fonts::Terminus.text("focus: #{focus}"), x_offset: Fonts::Terminus.height, y_offset: Fonts::Terminus.height)
        ui.display(frame, timeout: 40.milliseconds)

        if @click
          spawn do
            start = Time.monotonic
            DCIM.save_raw(still)
            Log.debug { "Writing image data took: #{Time.monotonic - start}" }
          end
          @click = false
        end

        @finished
      end
    end
  end
end
