require "./epd_2in13_v2"
require "./frame"
require "./ui"

module PiCamera
  class Frame
    def to_epd_payload : Bytes
      Slice.join(@buffer.map { |column| column.to_slice }).map { |byte| byte.bit_reverse }
    end
  end

  class Ui::Epd < Ui
    def initialize
      super

      EPD_2in13_v2.module_init
      mode Mode::Full

      at_exit do
        mode = @mode
        if mode && mode.partial?
          mode Mode::Full
          display @frame
        end

        @display_channel.close
        while !@display_fiber.dead?
          Fiber.yield
        end

        EPD_2in13_v2.module_exit
      end
    end

    def display_full(frame : Frame)
      EPD_2in13_v2.display frame.to_epd_payload
    end

    def display_partial(frame : Frame)
      EPD_2in13_v2.display_partial frame.to_epd_payload
    end

    def mode(mode : Mode)
      super

      EPD_2in13_v2.display_partial_base_image @frame.to_epd_payload if mode == Mode::Partial

      EPD_2in13_v2.init(mode.full? ? EPD_2in13_v2::Mode::Full : EPD_2in13_v2::Mode::Partial)
    end
  end
end
