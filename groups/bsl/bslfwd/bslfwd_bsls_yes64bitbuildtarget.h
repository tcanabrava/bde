#ifndef INCLUDED_BSLFWD_BSLS_YES64BITBUILDTARGET
#define INCLUDED_BSLFWD_BSLS_YES64BITBUILDTARGET

#ifndef BSLFWD_BUILDTARGET
#include <bslfwd_buildtarget.h>
#endif

#ifdef BDE_BUILD_TARGET_BSL_NAMESPACES

namespace BloombergLP {
    namespace bsls {
        class Yes64BitBuildTarget;
    }

    typedef bsls::Yes64BitBuildTarget bsls_Yes64BitBuildTarget;
}

#else

namespace BloombergLP {
    class bsls_Yes64BitBuildTarget;
}

#endif

#endif
