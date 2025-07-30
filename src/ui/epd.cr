require "log"

require "../epd"
require "../epd_2in13_v2"
require "../frame"
require "../ui"

module PiCamera
  class Ui::Epd < Ui
    class FrameError < ArgumentError; end

    @display_channel = Channel(Bytes).new

    def initialize
      @frame = Frame.new(width: EPD_2in13_v2::WIDTH, height: EPD_2in13_v2::HEIGHT)
      @mode = Mode::Full

      EPD_2in13_v2.module_init
      EPD_2in13_v2.init EPD_2in13_v2::Mode::Full

      display_fiber = spawn do
        loop do
          Log.debug { "WAITING for payload" }
          select
          when payload = @display_channel.receive?
            Log.debug { "payload received" }

            if payload.nil?
              Log.debug { "payload is missing" }
              break
            end

            case @mode
            when Mode::Full
              Log.debug { "displaying full" }
              EPD_2in13_v2.display payload
              Log.debug { "DONE" }
            when Mode::Partial
              Log.debug { "displaying partial" }
              EPD_2in13_v2.display_partial payload
              Log.debug { "DONE" }
            else
              Log.debug { "invalid mode: #{@mode}" }
            end
            Log.debug { "DONE-DONE" }
          end
        end
      end

      at_exit do
        mode = @mode
        if mode && mode.partial?
          mode Mode::Full
          display @frame
        end

        @display_channel.close
        while !display_fiber.dead?
          Fiber.yield
        end

        EPD_2in13_v2.module_exit
      end
    end

    def display(frame : Frame, timeout after = 1.minute)
      raise FrameError.new "Dimensions must match: #{@frame.width}x#{@frame.height}" if frame.width != @frame.width || frame.height != @frame.height

      @frame = frame

      select
      when @display_channel.send(frame.to_epd_payload)
        Log.debug { "Data sent successfully" }
      when timeout after
        Log.debug { "Data dropped after timeout" }
      end
    end

    def mode(mode : Mode)
      return if mode == @mode

      Log.debug { "changing mode to #{mode}" }

      EPD_2in13_v2.display_partial_base_image @frame.to_epd_payload if mode == Mode::Partial

      EPD_2in13_v2.init(mode.full? ? EPD_2in13_v2::Mode::Full : EPD_2in13_v2::Mode::Partial)
      @mode = mode
    end
  end
end
