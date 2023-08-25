module PiCamera
  abstract class Ui
    enum Mode
      Full
      Partial
    end

    abstract def display(frame : Frame, timeout : Time::Span)
    abstract def mode(mode : Mode)
  end

  class Ui::Stdout < Ui
    def display(frame : Frame, timeout = 1.second)
      puts frame
    end

    def mode(mode : Mode)
    end
  end
end
