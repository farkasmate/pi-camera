start = Time.monotonic
pic = File.read("/tmp/pic")
puts "Reading image data took: #{(Time.monotonic - start).total_milliseconds}"

spawn do
  start = Time.monotonic
  loop do
    puts "Display after sleeping: #{(Time.monotonic - start).total_milliseconds}"
    start = Time.monotonic
    sleep 200.milliseconds
  end
end

spawn do
  10.times do
    start = Time.monotonic
    File.write("/tmp/test-file", pic)
    #File.open("/tmp/test-file", mode: "wb") do |file|
    #  pic.to_slice.each_slice(10_000_000) do |arr|
    #    s = Time.monotonic
    #    file.write(Slice.new(arr.size) {|i| arr[i]})
    #    puts "Yielding after: #{(Time.monotonic - s).total_milliseconds}"
    #    Fiber.yield
    #  end
    #end
    puts "Writing image data took: #{(Time.monotonic - start).total_milliseconds}"
    Fiber.yield
  end
end

sleep 10.seconds
