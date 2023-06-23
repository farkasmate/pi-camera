module Pi::Camera
  class Menu
    def initialize(&renderer : Frame -> Nil)
      @renderer = renderer
    end

    def animate
      spawn do
        frame = Frame.new(width: 250, height: 122)
        rectangle = Frame.new(width: 10, height: 10)
        rectangle.fill(Frame::Color::Black)
        frame.draw(rectangle, x_offset: 10, y_offset: 10, scale: 3)
        loop do
          frame.shift_left!
          @renderer.call frame
          sleep 1.seconds
        end
      end
    end
  end
end
