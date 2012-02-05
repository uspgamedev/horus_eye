
require "ugdk.math"

print "============ HEY FROM LUA"

function ls(t)
    for k,v in pairs(t) do
        print(k,v)
    end
end

v = ugdk_math.Vector2D(1,2)

--ls(ugdk_math)

print "============ BYE FROM LUA"

