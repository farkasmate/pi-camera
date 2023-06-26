require "option_parser"

module Pi::Camera
  class CLI
    def initialize
      Pidfile.lock "./pi-camera.pid"
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
        parser.on("menu", "Start menu or next menu item") { command = :menu }
        parser.on("select", "Select current menu item or start capture if menu is not active") { command = :select }
        parser.on("sync", "Sync cached images into Google Photos") { command = :sync }
      end

      parser.parse

      ui = is_headless ? Ui::Stdout.new : Ui::Epd.new Ui::Epd::Mode::Partial

      case command
      when :debug
        frame = Frame.new(width: 250, height: 122)
        frame.draw(Fonts::Terminus.text("Hello World!"))
        ui.display frame
      when :menu
        menu = Menu.new ui
        menu.animate
        sleep 10.seconds
      else
        puts parser
        exit 1
      end
    end
  end
end
