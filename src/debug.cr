require "./fonts"

module PiCamera
  class Debug
    def initialize(ui : Ui)
      ui.mode Ui::Mode::Full

      hostname = shell "hostname"
      ssid = shell "iwgetid --raw"
      ip_address = shell "hostname --all-ip-addresses"
      loads = shell "uptime | grep -Po 'load average: \\K.*$'"
      battery = shell "echo 'get battery' | nc -q 0 127.0.0.1 8423 | grep -Po ' \\K\\d+'"

      frame = Frame.new(width: 250, height: 122)
      frame.draw(Fonts::Terminus.text("HOSTNAME: #{hostname}"), x_offset: Fonts::Terminus.height, y_offset: Fonts::Terminus.height * 1)
      frame.draw(Fonts::Terminus.text("SSID:     #{ssid}"), x_offset: Fonts::Terminus.height, y_offset: Fonts::Terminus.height * 2)
      frame.draw(Fonts::Terminus.text("IP:       #{ip_address}"), x_offset: Fonts::Terminus.height, y_offset: Fonts::Terminus.height * 3)
      frame.draw(Fonts::Terminus.text("LOAD:     #{loads}"), x_offset: Fonts::Terminus.height, y_offset: Fonts::Terminus.height * 4)
      frame.draw(Fonts::Terminus.text("BATTERY:  #{battery}%"), x_offset: Fonts::Terminus.height, y_offset: Fonts::Terminus.height * 5)

      ui.display frame
    end

    private def shell(command : String) : String
      output = IO::Memory.new
      Process.run(command, output: output, shell: true)
      output.to_s.chomp
    end
  end
end
