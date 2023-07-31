# Based on: https://github.com/waveshareteam/e-Paper/blob/8af38f2c89c236f8f9ebd353f69f044fd3d81cc3/RaspberryPi_JetsonNano/c/lib/e-Paper/EPD_2in13_V2.c
# Waveshare's original license is included verbatim below.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

require "bcm2835"
require "log"

module EPD_2in13_v2
  extend self

  ::Log.setup_from_env
  Log = ::Log.for "EPD_in13_v2"

  # NOTE: Width and height are swapped here (landscape)
  WIDTH     =  250
  HEIGHT    =  122
  BYTE_SIZE = 4000 # (HEIGHT / 8).round(:to_positive).to_i * WIDTH

  LUT_FULL_UPDATE = [
    0x80_u8, 0x60_u8, 0x40_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT0: BB:   VS 0 ~7
    0x10_u8, 0x60_u8, 0x20_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT1: BW:   VS 0 ~7
    0x80_u8, 0x60_u8, 0x40_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT2: WB:   VS 0 ~7
    0x10_u8, 0x60_u8, 0x20_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT3: WW:   VS 0 ~7
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT4: VCOM: VS 0 ~7

    0x03_u8, 0x03_u8, 0x00_u8, 0x00_u8, 0x02_u8, # TP0 A~D RP0
    0x09_u8, 0x09_u8, 0x00_u8, 0x00_u8, 0x02_u8, # TP1 A~D RP1
    0x03_u8, 0x03_u8, 0x00_u8, 0x00_u8, 0x02_u8, # TP2 A~D RP2
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP3 A~D RP3
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP4 A~D RP4
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP5 A~D RP5
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP6 A~D RP6

    0x15_u8, 0x41_u8, 0xA8_u8, 0x32_u8, 0x30_u8, 0x0A_u8,
  ]

  LUT_PARTIAL_UPDATE = [
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT0: BB:   VS 0 ~7
    0x80_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT1: BW:   VS 0 ~7
    0x40_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT2: WB:   VS 0 ~7
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT3: WW:   VS 0 ~7
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # LUT4: VCOM: VS 0 ~7

    0x0A_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP0 A~D RP0
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP1 A~D RP1
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP2 A~D RP2
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP3 A~D RP3
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP4 A~D RP4
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP5 A~D RP5
    0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, 0x00_u8, # TP6 A~D RP6

    0x15_u8, 0x41_u8, 0xA8_u8, 0x32_u8, 0x30_u8, 0x0A_u8,
  ]

  enum Mode : UInt8
    Full    = 0
    Partial = 1
  end

  enum Pin : UInt16
    CS   =  8
    RST  = 17
    BUSY = 24
    DC   = 25
  end

  def dev_module_init
    raise "bcm2835 init failed" if LibBcm2835.init == 0

    LibBcm2835.gpio_fsel(Pin::RST, LibBcm2835::GPIO_FSEL_OUTP)
    LibBcm2835.gpio_fsel(Pin::DC, LibBcm2835::GPIO_FSEL_OUTP)
    LibBcm2835.gpio_fsel(Pin::CS, LibBcm2835::GPIO_FSEL_OUTP)
    LibBcm2835.gpio_fsel(Pin::BUSY, LibBcm2835::GPIO_FSEL_INPT)

    dev_digital_write(Pin::CS, 1)

    LibBcm2835.spi_begin                                                       # Start spi interface, set spi pin for the reuse function
    LibBcm2835.spi_setBitOrder(LibBcm2835::SPI_BIT_ORDER_MSBFIRST)             # High first transmission
    LibBcm2835.spi_setDataMode(LibBcm2835::SPI_MODE0)                          # spi mode 0
    LibBcm2835.spi_setClockDivider(LibBcm2835::SPI_CLOCK_DIVIDER_128)          # Frequency
    LibBcm2835.spi_chipSelect(LibBcm2835::SPI_CS0)                             # set CE0
    LibBcm2835.spi_setChipSelectPolarity(LibBcm2835::SPI_CS0, LibBcm2835::LOW) # enable cs0
  end

  def dev_module_exit
    dev_digital_write(Pin::CS, 0)
    dev_digital_write(Pin::DC, 0)
    dev_digital_write(Pin::RST, 0)

    LibBcm2835.spi_end
    LibBcm2835.close
  end

  # FIXME: sleep millis.milliseconds
  def dev_delay_ms(millis : UInt32)
    LibBcm2835.delay(millis)
  end

  def dev_digital_read(pin : Pin) : UInt8
    return LibBcm2835.gpio_lev(pin)
  end

  def dev_digital_write(pin : Pin, value : UInt8)
    LibBcm2835.gpio_write(pin, value)
  end

  def dev_spi_writebyte(value : UInt8)
    LibBcm2835.spi_transfer(value)
  end

  def reset
    Log.debug { "reset" }
    dev_digital_write(Pin::RST, 1)
    dev_delay_ms(200)
    dev_digital_write(Pin::RST, 0)
    dev_delay_ms(2)
    dev_digital_write(Pin::RST, 1)
    dev_delay_ms(200)
  end

  def send_command(reg : UInt8)
    dev_digital_write(Pin::DC, 0)
    dev_digital_write(Pin::CS, 0)
    dev_spi_writebyte(reg)
    dev_digital_write(Pin::CS, 1)
  end

  def send_data(data : UInt8)
    dev_digital_write(Pin::DC, 1)
    dev_digital_write(Pin::CS, 0)
    dev_spi_writebyte(data)
    dev_digital_write(Pin::CS, 1)
  end

  def read_busy
    Log.debug { "e-Paper busy" }
    while dev_digital_read(Pin::BUSY) == 1
      # FIXME: sleep 100.milliseconds
      dev_delay_ms(100)
    end
    Log.debug { "e-Paper busy release" }
  end

  def turn_on_display
    Log.debug { "turn_on_display" }
    send_command(0x22)
    send_data(0xC7)
    send_command(0x20)
    read_busy
  end

  def turn_on_display_partial
    Log.debug { "turn_on_display_partial" }
    send_command(0x22)
    send_data(0x0C)
    send_command(0x20)
    read_busy
  end

  def init(mode : Mode)
    Log.debug { "init" }
    reset

    case mode
    when Mode::Full
      init_full
    when Mode::Partial
      init_partial
    end
    # Debug("error, the Mode is EPD_2IN13_FULL or EPD_2IN13_PART");
  end

  private def init_full
    read_busy
    send_command(0x12) # soft reset
    read_busy

    send_command(0x74) # set analog block control
    send_data(0x54)
    send_command(0x7E) # set digital block control
    send_data(0x3B)

    send_command(0x01) # Driver output control
    send_data(0xF9)
    send_data(0x00)
    send_data(0x00)

    send_command(0x11) # data entry mode
    send_data(0x01)

    send_command(0x44) # set Ram-X address start/end position
    send_data(0x00)
    send_data(0x0F) # 0x0C-->(15+1)*8=128

    send_command(0x45) # set Ram-Y address start/end position
    send_data(0xF9)    # 0xF9-->(249+1)=250
    send_data(0x00)
    send_data(0x00)
    send_data(0x00)

    send_command(0x3C) # BorderWavefrom
    send_data(0x03)

    send_command(0x2C) # VCOM Voltage
    send_data(0x55)

    send_command(0x03)
    send_data(LUT_FULL_UPDATE[70])

    send_command(0x04)
    send_data(LUT_FULL_UPDATE[71])
    send_data(LUT_FULL_UPDATE[72])
    send_data(LUT_FULL_UPDATE[73])

    send_command(0x3A) # Dummy Line
    send_data(LUT_FULL_UPDATE[74])
    send_command(0x3B) # Gate time
    send_data(LUT_FULL_UPDATE[75])

    send_command(0x32)
    70.times { |index| send_data(LUT_FULL_UPDATE[index]) }

    send_command(0x4E) # set RAM x address count to 0;
    send_data(0x00)
    send_command(0x4F) # set RAM y address count to 0X127;
    send_data(0xF9)
    send_data(0x00)
    read_busy
  end

  private def init_partial
    send_command(0x2C) # VCOM Voltage
    send_data(0x26)

    read_busy

    send_command(0x32)
    70.times { |index| send_data(LUT_PARTIAL_UPDATE[index]) }

    send_command(0x37)
    send_data(0x00)
    send_data(0x00)
    send_data(0x00)
    send_data(0x00)
    send_data(0x40)
    send_data(0x00)
    send_data(0x00)

    send_command(0x22)
    send_data(0xC0)

    send_command(0x20)
    read_busy

    send_command(0x3C) # BorderWavefrom
    send_data(0x01)
  end

  def clear
    Log.debug { "clear" }
    send_command(0x24)
    BYTE_SIZE.times { send_data(0xFF) }
    turn_on_display
  end

  def display(bytes : Bytes)
    Log.debug { "display" }

    raise "Image size must be #{BYTE_SIZE} bytes" unless bytes.bytesize == BYTE_SIZE

    send_command(0x24)
    bytes.each { |byte| send_data(byte) }
    turn_on_display
  end

  def display_partial_base_image(bytes : Bytes)
    Log.debug { "display_partial_base_image" }

    raise "Image size must be #{BYTE_SIZE} bytes" unless bytes.bytesize == BYTE_SIZE

    # NOTE: Write RAM (BW)
    send_command(0x24)
    bytes.each { |byte| send_data(byte) }

    # NOTE: Write RAM (RED) ?
    send_command(0x26)
    bytes.each { |byte| send_data(byte) }
    turn_on_display
  end

  def display_partial(bytes : Bytes)
    send_command(0x24)
    bytes.each { |byte| send_data(byte) }
    turn_on_display_partial
  end

  def sleep
    send_command(0x22) # POWER OFF
    send_data(0xC3)
    send_command(0x20)

    send_command(0x10) # enter deep sleep
    send_data(0x01)
    dev_delay_ms(100)
  end
end
