
#ifndef __Share_FacebookConnector_h__
#define __Share_FacebookConnector_h__

#include "json98.h"
#include "ShareConnector.h"

namespace sdkbox {

    class ShareFacebookConnector : public ShareConnector {
    public:
        ShareTwitterConnector() {
            platformName = "facebook";
        }

        bool init(const Json& config);
        void share(const PluginShare::ShareInfo& info);
        void shareDialog(const PluginShare::ShareInfo& info);
        void shareRequest(const PluginShare::ShareInfo& info);
    };

}

#endif
