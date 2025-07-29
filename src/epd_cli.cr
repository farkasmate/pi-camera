require "log"

require "./epd"
require "./frame"

module PiCamera
  ::Log.setup_from_env

  frame = Frame.new(width: 250, height: 122) # FIXME
  frame.set(x: 100, y: 5, color: Frame::Color::Black)
  frame.set(x: 102, y: 5, color: Frame::Color::Black)
  frame.set(x: 100, y: 7, color: Frame::Color::Black)
  frame.set(x: 101, y: 7, color: Frame::Color::Black)
  frame.set(x: 102, y: 7, color: Frame::Color::Black)

  Epd.init
  Epd.display frame
end
