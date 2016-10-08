#ifndef __Share_EMailConnector_h__
#define __Share_EMailConnector_h__

#include "json98.h"
#include "ShareConnector.h"

namespace sdkbox {

    class ShareEMailConnector : public ShareConnector {
    public:
        ShareEMailConnector() {
            platformName = "email";
            platform = sdkbox::SocialPlatform::Platform_Mail;
        }

        bool init(const Json& config);
        void share(const sdkbox::SocialShareInfo& info);
        void shareDialog(const sdkbox::SocialShareInfo& info);
    };

}

#endif
