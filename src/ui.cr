module PiCamera
  abstract struct Ui
    enum Mode
      Full
      Partial
    end

    abstract def display(frame : Frame, timeout : Time::Span)
    abstract def mode(mode : Mode)
  end
end
