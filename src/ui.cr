require "log"
require "drawille-cr"

require "./epd_2in13_v2"

module PiCamera
  abstract class Ui
    enum Mode
      Full
      Partial
    end

    class FrameError < ArgumentError; end

    @display_channel = Channel(Frame).new
    @display_fiber : Fiber
    @frame = Frame.new(width: EPD_2in13_v2::WIDTH, height: EPD_2in13_v2::HEIGHT)
    @mode = Mode::Full

    private abstract def display_full(frame : Frame)
    private abstract def display_partial(frame : Frame)

    def initialize
      @display_fiber = spawn do
        loop do
          Log.debug { "WAITING for payload" }
          select
          when payload = @display_channel.receive?
            Log.debug { "payload received" }

            break if payload.nil?

            case @mode
            when Mode::Full
              Log.debug { "displaying full" }
              display_full(payload)
              Log.debug { "DONE" }
            when Mode::Partial
              Log.debug { "displaying partial" }
              display_partial(payload)
              Log.debug { "DONE" }
            end
            Log.debug { "DONE-DONE" }
          end
        end
      end
    end

    def display(frame : Frame, timeout after = 1.minute)
      raise FrameError.new "Dimensions must match: #{@frame.width}x#{@frame.height}" if frame.width != @frame.width || frame.height != @frame.height

      @frame = frame

      select
      when @display_channel.send(frame)
        Log.debug { "Data sent successfully" }
      when timeout after
        Log.debug { "Data dropped after timeout" }
      end
    end

    def mode(mode : Mode)
      return if mode == @mode

      @mode = mode

    end
  end

  class Ui::Stdout < Ui
    def initialize
      super

      @canvas = Drawille::Canvas.new
    end

    # TODO: Can I use @frame here?
    def display_full(frame : Frame)
      display_partial(frame)

      sleep 1.second
    end

    def display_partial(frame : Frame)
      frame.width.times do |x|
        frame.height.times do |y|
          @canvas.set(x, y) if frame.get(x, y) == Frame::Color::Black
        end
      end

      STDERR.puts @canvas.render
      @canvas.clear

      sleep 200.milliseconds
    end
  end
end
