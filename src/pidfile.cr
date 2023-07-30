module Pidfile
  extend self

  class ProcessAlreadyRunning < Exception; end

  def lock(filename : String = "/var/run/#{File.basename PROGRAM_NAME}.pid")
    raise ProcessAlreadyRunning.new if File.exists?(filename) && Process.exists?(File.read(filename).to_i)

    File.write filename, Process.pid

    at_exit { File.delete filename }
  end
end
