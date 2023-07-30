module PiCamera
  abstract class Ui
    abstract def display(frame : Frame)
  end

  class Ui::Stdout < Ui
    def display(frame : Frame)
      puts frame
    end
  end
end
