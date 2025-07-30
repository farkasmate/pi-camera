require "drawille-cr"

require "./frame"

module Drawille
  class Canvas
    def self.create_epd : self
      new(terminal_lines: 31, terminal_columns: 126)
    end

    def draw(frame : PiCamera::Frame)
      frame.width.times do |x|
        frame.height.times do |y|
          set(x, y) if frame.get(x, y) == PiCamera::Frame::Color::Black
        end
      end
    end

    def render_with_border : String
      Colorize.enabled = false

      String.build do |str|
        str << "+#{"-" * @terminal_columns}+\n"
        render.chars.each_slice(@terminal_columns) { |line| str << "|#{line.join}|\n" }
        str << "+#{"-" * @terminal_columns}+\n"
      end
    end
  end
end
