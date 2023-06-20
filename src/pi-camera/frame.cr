module Pi::Camera
  class Frame
    enum Color : UInt8
      Black
      White
    end

    getter :width, :height

    @buffer : Slice(Bytes)

    def initialize(@width : UInt8, @height : UInt8)
      @buffer = Bytes.new(@height.to_i).map { |slice| Bytes.new(@width.to_i, Color::White.value) }
    end

    def get(x, y) : Color
      raise IndexError.new if x.negative? || y.negative? || x > @width || y > @height

      Color.from_value @buffer[y][x]
    end

    def set(x : Int, y : Int, color : Color)
      raise IndexError.new if x.negative? || y.negative? || x > @width || y > @height

      @buffer[y][x] = color.value
    end

    def fill(color : Color)
      @buffer.each { |row| row.fill(color.value) }
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

    def shift(count : Int = 1)
      @buffer.each { |row| row.rotate! count }
    end

    def to_s
      String.build do |ret|
        @buffer.each do |row|
          ret << row.map { |color| Color.from_value(color).black? ? "#" : " " }.to_a.join
          ret << "\n"
        end
      end.chomp
    end
  end
end
