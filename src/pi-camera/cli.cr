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
      else
        puts parser
        exit 1
      end
    end
  end
end
