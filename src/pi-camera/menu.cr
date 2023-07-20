module PiCamera
  class Menu
    @@options : Array(Array(Frame)) = [Icons::Camera, Icons::Cloud, Icons::Wrench]

    @frame = Frame.new(width: 250, height: 122)
    @is_next = false
    @is_selected = false
    @selection = :capture

    def initialize(@ui : Ui)
      Signal::USR1.trap do
        @is_next = true
      end

      Signal::USR2.trap do
        @is_selected = true
        # FIXME
        @selection = case @@options[0]
                     when Icons::Camera
                       :capture
                     when Icons::Cloud
                       :sync
                     when Icons::Wrench
                       :debug
                     else
                       :error
                     end
        puts "Selected option: #{@selection}"
      end

      spawn do
        loop do
          4.times do |index|
            @frame.clear
            @frame.draw(@@options[-1][0], x_offset: 18, y_offset: 37)     # FIXME
            @frame.draw(@@options[0][index], x_offset: 101, y_offset: 37) # FIXME
            @frame.draw(@@options[1][0], x_offset: 184, y_offset: 37)     # FIXME
            @ui.display @frame
            Fiber.yield

            break if @is_next
          end

          animate_next if @is_next
          break if @is_selected
        end

        case @selection
        when :debug
          Debug.new @ui
        end
      end
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
