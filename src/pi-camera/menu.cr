module Pi::Camera
  class Menu
    @@options : Array(Array(Frame)) = [Icons::Wrench, Icons::Camera, Icons::Cloud]

    @frame = Frame.new(width: 250, height: 122)
    @is_next = false

    def initialize(@ui : Ui)
    end

    def animate
      Signal::USR1.trap do
        @is_next = true
      end

      spawn do
        loop do
          4.times do |index|
            @frame.draw(@@options[0][0], x_offset: 18, y_offset: 37)      # FIXME
            @frame.draw(@@options[1][index], x_offset: 101, y_offset: 37) # FIXME
            @frame.draw(@@options[2][0], x_offset: 184, y_offset: 37)     # FIXME
            @ui.display @frame
            Fiber.yield
            break if @is_next
          end

          animate_next if @is_next
        end
      end

      Signal::USR1.reset
    end

    private def animate_next
      3.times do |index|
        @frame.shift_left! 21 # FIXME
        @ui.display @frame
        Fiber.yield
      end
      @@options.rotate!
      @is_next = false
    end
  end
end
