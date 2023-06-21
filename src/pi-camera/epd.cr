module Pi::Camera
  class Epd
    @[Link("bcm2835")]
    @[Link("epd_2in13_v2")]
    lib LibEPD
      EPD_2IN13_V2_FULL = 0
      EPD_2IN13_V2_PART = 1

      fun DEV_Module_Init : UInt8
      fun DEV_Module_Exit : Void

      fun EPD_2IN13_V2_Init(mode : UInt8)
      fun EPD_2IN13_V2_Display(image : Pointer(UInt8))
    end

    def initialize # FIXME: mode
      LibEPD.DEV_Module_Init
      LibEPD.EPD_2IN13_V2_Init LibEPD::EPD_2IN13_V2_FULL
    end

    def display(bytes : Bytes)
      LibEPD.EPD_2IN13_V2_Display bytes.to_unsafe
    end

    def finalize
      LibEPD.DEV_Module_Exit
    end
  end
end
