module Pi::Camera
  class Menu
    def initialize(@ui : Ui)
    end

    def animate
      spawn do
        frame = Frame.new(width: 250, height: 122)
        loop do
          4.times do |index|
            frame.draw(Frame::Icon::Camera[index], x_offset: 101, y_offset: 37) # FIXME
            @ui.display frame
            Fiber.yield
          end
        end
      end
    end
  end
end
