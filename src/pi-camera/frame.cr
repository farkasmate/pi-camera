require "bit_array"

module Pi::Camera
  class Frame
    enum Color
      Black # false
      White # true
    end

    getter :width, :height

    @buffer : Slice(BitArray)

    def initialize(@width : Int32, @height : Int32)
      @buffer = Bytes.new(@width).map { |slice| BitArray.new(@height, true) }
    end

    def initialize(@width : Int32, @height : Int32, &block : Int32, Int32 -> Color)
      initialize(@width, @height)
      (0..width - 1).each do |x|
        (0..height - 1).each do |y|
          set(x, y, yield(x, y))
        end
      end
    end

    def get(x, y) : Color
      raise IndexError.new if x.negative? || y.negative? || x > @width || y > @height

      @buffer[x][y] ? Color::White : Color::Black
    end

    def set(x : Int, y : Int, color : Color)
      raise IndexError.new if x.negative? || y.negative? || x > @width || y > @height

      @buffer[x][y] = color.white?
    end

    def fill(color : Color)
      @buffer.each { |column| column.fill(color.white?) }
    end

    def draw(frame : Frame, x_offset : Int = 0, y_offset : Int = 0, scale : Int = 1, color : Color = Color::Black, transparent : Bool = false)
      (0..frame.width * scale - 1).each do |x|
        (0..frame.height * scale - 1).each do |y|
          begin
            if frame.get(x // scale, y // scale) == color
              set(x + x_offset, y + y_offset, color)
            elsif !transparent
              set(x + x_offset, y + y_offset, color.black? ? Color::White : Color::Black)
            end
          rescue IndexError
            # NOTE: Clipping `frame`
          end
        end
      end
    end

    def shift_left!(count : Int = 1)
      @buffer.rotate! count
    end

    def shift_right!(count : Int = 1)
      shift_left! -count
    end

    def shift_up!(count : Int = 1)
      @buffer.map! { |column| column.rotate! count }
    end

    def shift_down!(count : Int = 1)
      shift_up! -count
    end

    def to_s
      @buffer.map { |column| column.to_a }.to_a.transpose.map do |row|
        row.map { |color| color ? " " : "#" }.join
      end.join("\n")
    end

    def to_epd_payload : Bytes
      Slice.join(@buffer.map { |column| column.to_slice }).map { |byte| byte.bit_reverse }
    end
  end
end
