module Pi::Camera
  class Menu
    def initialize(&renderer : Frame -> Nil)
      @renderer = renderer
    end

    def animate
      spawn do
        frame = Frame.new(width: 250, height: 122)
        frame.draw(Frame::Icon::Camera, x_offset: 10, y_offset: 10, scale: 3)
        loop do
          frame.shift_left!
          @renderer.call frame
          Fiber.yield
        end
      end
    end
  end
end
