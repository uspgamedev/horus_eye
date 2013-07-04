import ugdk_math

def get_name(s):
    return "resources/images/switch/%04d.png" % s

file = [get_name(i+1) for i in range(120)]
frames = [[0, 0, 106, 106, ugdk_math.Vector2D(53, 93), i ] for i in range(120)]

