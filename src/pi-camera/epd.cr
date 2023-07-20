require "./ui"

module PiCamera
  class Frame
    def to_epd_payload : Bytes
      Slice.join(@buffer.map { |column| column.to_slice }).map { |byte| byte.bit_reverse }
    end
  end

  class Ui::Epd < Ui
    class FrameError < ArgumentError; end

    enum Mode : UInt8
      Full
      Partial
    end

    @@width = 250
    @@height = 122

    @[Link("bcm2835")]
    @[Link("epd_2in13_v2")]
    lib LibEPD
      fun DEV_Module_Init : UInt8
      fun DEV_Module_Exit : Void

      fun EPD_2IN13_V2_Init(mode : UInt8)
      fun EPD_2IN13_V2_Display(image : Pointer(UInt8))
      fun EPD_2IN13_V2_DisplayPart(image : Pointer(UInt8))
      fun EPD_2IN13_V2_DisplayPartBaseImage(image : Pointer(UInt8))
    end

    def initialize(@mode : Mode = Mode::Full)
      LibEPD.DEV_Module_Init
      LibEPD.EPD_2IN13_V2_Init Mode::Full

      return if @mode.full?

      @frame = Frame.new(width: @@width, height: @@height)
      LibEPD.EPD_2IN13_V2_DisplayPartBaseImage @frame.to_epd_payload.to_unsafe
      LibEPD.EPD_2IN13_V2_Init Mode::Partial

      at_exit { finalize }
    end

    def display(frame : Frame)
      raise FrameError.new "Dimensions must match: #{@@width}x#{@@height}" if frame.width != @@width || frame.height != @@height

      @frame = frame

      case @mode
      when Mode::Full
        LibEPD.EPD_2IN13_V2_Display @frame.to_epd_payload.to_unsafe
      when Mode::Partial
        LibEPD.EPD_2IN13_V2_DisplayPart @frame.to_epd_payload.to_unsafe
      end
    end

    def finalize
      if @mode.partial?
        @mode = Mode::Full
        LibEPD.EPD_2IN13_V2_Init Mode::Full
        display @frame
      end

      LibEPD.DEV_Module_Exit
    end
  end
end
