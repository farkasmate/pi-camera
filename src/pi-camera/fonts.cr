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
        bitmap = @@font.get_bitmap(string, wrap: 0)[0]

        frame = Frame.new(width: bitmap[0].size, height: bitmap.size)
        bitmap.each.with_index do |row, y|
          row.each.with_index { |color, x| frame.set(x, y, color ? Frame::Color::Black : Frame::Color::White) }
        end

        frame
      end
    end
  end
end
