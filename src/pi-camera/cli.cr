require "option_parser"

module Pi::Camera
  class CLI
    def initialize
      command = nil

      parser = OptionParser.new do |parser|
        parser.banner = "Usage: pi-camera [OPTIONS] [COMMAND]"
        parser.on("-h", "--help", "Show this help") do
          puts parser
          exit
        end

        parser.on("capture", "Capture a photo") { command = :capture }
        parser.on("debug", "Display debug information") { command = :debug }
        parser.on("menu", "Start menu or next menu item") { command = :menu }
        parser.on("select", "Select current menu item or start capture if menu is not active") { command = :select }
        parser.on("sync", "Sync cached images into Google Photos") { command = :sync }
      end

      parser.parse

      case command
      when :debug
        puts "Hello World!"
        epd = Epd.new
        frame = Frame.new(width: 250, height: 122)
        frame.set(10, 10, Frame::Color::Black)
        frame.set(20, 10, Frame::Color::Black)
        frame.set(10, 20, Frame::Color::Black)
        frame.set(20, 20, Frame::Color::Black)
        epd.display frame.to_epd_payload
      when :menu
        epd = Epd.new
        menu = Menu.new { |frame| epd.display frame.to_epd_payload }
        menu.animate
        sleep 10.seconds
      else
        puts parser
        exit 1
      end
    end
  end
end
