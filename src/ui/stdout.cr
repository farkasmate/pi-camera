require "drawille-cr"

require "../frame"
require "../ui"

module PiCamera
  struct Ui::Stdout < Ui
    def initialize
      @rows = 32
      @cols = 126
      @canvas = Drawille::Canvas.new(@rows, @cols)
    end

    def display(frame : Frame, timeout = 1.second)
      frame.width.times do |x|
        frame.height.times do |y|
          @canvas.set(x, y) if frame.get(x, y) == Frame::Color::Black
        end
      end

      Colorize.enabled = false
      frame_string = String.build do |str|
        str << "+#{"-" * @cols}+\n"
        @canvas.render.chars.each_slice(@cols) { |line| str << "|#{line.join}|\n" }
        str << "+#{"-" * @cols}+\n"
      end

      puts frame_string
      @canvas.clear

      sleep 200.milliseconds
    end

    def mode(mode : Mode)
    end
  end
end
