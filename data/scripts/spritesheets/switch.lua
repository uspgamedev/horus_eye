require 'ugdk.math'

file = {}
frames = {}

for i = 1,120 do
  table.insert(file, "resources/images/switch/" .. string.format("%.4d", i) .. ".png")
  table.insert(frames, {0, 0, 106, 106, ugdk_math.Vector2D(53, 93), i - 1 })
end
