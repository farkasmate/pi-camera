require "drawille-cr"

require "./epd_2in13_v2"

module PiCamera
  abstract class Ui
    enum Mode
      Full
      Partial
    end

    abstract def display(frame : Frame, timeout : Time::Span)
    abstract def mode(mode : Mode)
  end

  class Ui::Stdout < Ui
    def initialize
      @canvas = Drawille::Canvas.new
    end

    def display(frame : Frame, timeout = 1.second)
      frame.width.times do |x|
        frame.height.times do |y|
          @canvas.set(x, y) if frame.get(x, y) == Frame::Color::Black
        end
      end

      puts @canvas.render
      @canvas.clear

      sleep 200.milliseconds
    end

    def mode(mode : Mode)
    end
  end
end
