require "option_parser"

module PiCamera
  class CLI
    def initialize
      pid_path = "./pi-camera.pid"
      is_remote_control = false

      begin
        Pidfile.lock pid_path
      rescue Pidfile::ProcessAlreadyRunning
        is_remote_control = true
      end

      Signal::USR1.ignore
      Signal::USR2.ignore

      command = nil
      is_headless = false

      parser = OptionParser.new do |parser|
        parser.banner = "Usage: pi-camera [OPTIONS] [COMMAND]"
        parser.on("-h", "--help", "Show this help") do
          puts parser
          exit
        end

        parser.on("--headless", "Render UI on stdout") { is_headless = true }

        parser.on("capture", "Capture a photo") { command = :capture }
        parser.on("debug", "Display debug information") { command = :debug }
        parser.on("menu", "Start menu or next menu item (or shutter)") { command = :menu }
        parser.on("select", "Select current menu item or start capture if menu is not active") { command = :select }
        parser.on("sync", "Sync cached images into Google Photos") { command = :sync }
      end

      parser.parse

      if is_remote_control
        pid = File.read(pid_path).to_i
        case command
        when :menu
          Process.signal(Signal::USR1, pid)
        when :select
          Process.signal(Signal::USR2, pid)
        else
          exit 2
        end
        exit 0
      end

      ui = is_headless ? Ui::Stdout.new : Ui::Epd.new Ui::Epd::Mode::Partial

      case command
      when :debug
        Debug.new ui
      when :menu
        Menu.new ui
        sleep 10.seconds
      else
        puts parser
        exit 1
      end
    end
  end
end
