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
    class FrameError < ArgumentError; end

    alias Mode = EPD_2in13_v2::Mode

    def initialize(@mode : Mode = Mode::Full)
      EPD_2in13_v2.dev_module_init
      EPD_2in13_v2.init Mode::Full
      @display_partial = false

      @frame = Frame.new(width: EPD_2in13_v2::WIDTH, height: EPD_2in13_v2::HEIGHT)

      at_exit do
        if @mode.partial?
          @display_partial = false
          @mode = Mode::Full
          EPD_2in13_v2.init @mode
          display @frame
        end

        EPD_2in13_v2.dev_module_exit
      end

      return if @mode.full?

      EPD_2in13_v2.display_partial_base_image @frame.to_epd_payload
      EPD_2in13_v2.init Mode::Partial
      @display_partial = true

      spawn do
        while @display_partial
          puts "Partial display..."
          EPD_2in13_v2.display_partial @frame.to_epd_payload
          Fiber.yield
        end
      end
    end

    def display(frame : Frame)
      raise FrameError.new "Dimensions must match: #{@frame.width}x#{@frame.height}" if frame.width != @frame.width || frame.height != @frame.height

      @frame = frame

      case @mode
      when Mode::Full
        EPD_2in13_v2.display @frame.to_epd_payload
      when Mode::Partial
        # FIXME: Drawing continuously on fiber
      end
    end
  end
end
