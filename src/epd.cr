require "log"

require "./epd_2in13_v2"
require "./frame"

module PiCamera
  struct Frame
    def to_epd_payload : Bytes
      Slice.join(@buffer.map { |column| column.to_slice }).map { |byte| byte.bit_reverse }
    end
  end

  module Epd
    extend self

    Log = ::Log.for "PiCamera::Epd"

    def init
      Log.debug { "Init" }

      EPD_2in13_v2.module_init
      EPD_2in13_v2.init(EPD_2in13_v2::Mode::Full)

      at_exit do
        Log.debug { "Deinit" }
        EPD_2in13_v2.module_exit
        Log.debug { "DONE: Deinit" }
      end

      Log.debug { "DONE: Init" }
    end

    def display(frame : Frame)
      Log.debug { "Displaying full" }
      EPD_2in13_v2.display frame.to_epd_payload
      Log.debug { "DONE" }
    end
  end
end
