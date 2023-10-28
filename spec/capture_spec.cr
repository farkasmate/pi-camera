require "./spec_helper"
require "../src/capture"
require "../src/ui"

module PiCamera
  describe Capture do
    it "takes two pictures on two shutter presses" do
      # Emulate shutter button presses
      spawn do
        sleep 2.seconds
        Process.signal(Signal::USR1, Process.pid)
        sleep 2.seconds
        Process.signal(Signal::USR1, Process.pid)
        sleep 2.seconds
        Process.signal(Signal::USR2, Process.pid)
      end

      Capture.new Ui::Stdout.new
    end
  end
end
