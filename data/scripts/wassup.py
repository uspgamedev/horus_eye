print "starting wassup.py"

try:
    import ugdk
    print dir(ugdk)
except:
    print "could not import ugdk"

try:
    import _ugdk_math
    print dir(_ugdk_math)
except:
    print "could not import _ugdk_math"

import sys
print sys.modules
