module Pi::Camera
  class Frame
    enum Color : UInt8
      Black
      White
    end

    getter :width, :height

    def initialize(@width : UInt8, @height : UInt8)
      @buffer = Bytes.new(@width.to_i * @height.to_i, Color::White.value)
    end

    def get(x, y) : Color
      raise IndexError.new if x.negative? || y.negative? || x > @width || y > @height

      Color.from_value @buffer[x + @width * y]
    end

    def set(x : Int, y : Int, color : Color)
      raise IndexError.new if x.negative? || y.negative? || x > @width || y > @height

      @buffer[x + @width * y] = color.value
    end

    def fill(color : Color)
      @buffer.fill(color.value)
    end

    def draw(frame : Frame, x_offset : Int = 0, y_offset : Int = 0, scale : Int = 1, color : Color = Color::Black, transparent : Bool = false)
      (0..frame.width * scale - 1).each do |x|
        (0..frame.height * scale - 1).each do |y|
          if frame.get(x // scale, y // scale) == color
            set(x + x_offset, y + y_offset, color)
          elsif !transparent
            set(x + x_offset, y + y_offset, color.black? ? Color::White : Color::Black)
          end
        end
      end
    end

    def to_s
      String.build do |ret|
        @buffer.each.with_index do |color, index|
          ret << "\n" if index > 0 && index % width == 0
          ret << (Color.from_value(color).black? ? "#" : " ")
        end
      end
    end
  end
end
