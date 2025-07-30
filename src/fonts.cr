require "fontanyl"

require "./frame"
require "./resources"

module PiCamera
  module Fonts
    module Terminus
      extend self

      @@font = Fontanyl::BDF.new(Resources.get("fonts/ter-u16n.bdf"))

      def height
        @@font.size
      end

      def text(string : String) : Frame
        bitmaps = string.lines.map { |line| @@font.get_bitmap(line, wrap: 0)[0] }
        width = bitmaps.map(&.[0].size).max
        height = bitmaps.size * bitmaps[0].size

        frame = Frame.new(width, height)

        bitmaps.each_with_index do |bitmap, line|
          bitmap.each.with_index do |pixel_row, y|
            pixel_row.each.with_index do |color, x|
              frame.set(x, line * self.height + y, color ? Frame::Color::Black : Frame::Color::White)
            end
          end
        end

        frame
      end
    end
  end
end
