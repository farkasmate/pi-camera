require "log"
require "option_parser"
require "system/user"

require "./drawille"
require "./epd"
require "./fonts"
require "./frame"

module PiCamera
  ::Log.setup_from_env

  lib LibC
    fun geteuid : ::LibC::UidT
  end

  user = System::User.find_by(id: LibC.geteuid.to_s)
  headless = !(user.username == "root" && File.exists? "/dev/mem")
  text = ""
  x_offset = 0
  y_offset = 0

  OptionParser.new do |parser|
    help = -> do
      STDERR.puts parser
      exit 1
    end

    parser.banner = "Usage: pi-camera [OPTIONS] [COMMAND]"
    parser.on("-h", "--help", "Show this help") { help.call }
    parser.missing_option { help.call }
    parser.invalid_option { help.call }

    parser.on("--headless", "Render UI on stdout") { headless = true }
    parser.on("--text TEXT", "Draw text on frame") { |t| text = t.gsub("\\n", "\n") }
    parser.on("-x X_OFFSET", "X offset") { |x| x_offset = x.to_i }
    parser.on("-y Y_OFFSET", "Y offset") { |y| y_offset = y.to_i }
  end.parse

puts "#{x_offset.class}"
  frame = Frame.new
  frame.set(x: 100, y: 5, color: Frame::Color::Black)
  frame.set(x: 102, y: 5, color: Frame::Color::Black)
  frame.set(x: 100, y: 7, color: Frame::Color::Black)
  frame.set(x: 101, y: 7, color: Frame::Color::Black)
  frame.set(x: 102, y: 7, color: Frame::Color::Black)

  frame.draw(Fonts::Terminus.text(text) , x_offset: x_offset, y_offset: y_offset) unless text.empty?

  if headless
    canvas = Drawille::Canvas.create_epd
    canvas.draw frame
    puts canvas.render_with_border
  else
    Epd.init
    Epd.display frame
  end
end
