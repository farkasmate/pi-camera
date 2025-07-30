require "log"

require "./epd"
require "./frame"
require "./ui/stdout"

module PiCamera
  ::Log.setup_from_env

  ui = Ui::Stdout.new

  frame = Frame.new
  frame.set(x: 100, y: 5, color: Frame::Color::Black)
  frame.set(x: 102, y: 5, color: Frame::Color::Black)
  frame.set(x: 100, y: 7, color: Frame::Color::Black)
  frame.set(x: 101, y: 7, color: Frame::Color::Black)
  frame.set(x: 102, y: 7, color: Frame::Color::Black)

  # Epd.init
  # Epd.display frame
  ui.display frame
end
