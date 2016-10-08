
#ifndef __Share_SMSConnector_h__
#define __Share_SMSConnector_h__

#include "json98.h"
#include "ShareConnector.h"

namespace sdkbox {

    class ShareSMSConnector : public ShareConnector {
    public:
        ShareSMSConnector() {
            platformName = "sms";
            platform = sdkbox::SocialPlatform::Platform_SMS;
        }

        bool init(const Json& config);
        void share(const sdkbox::SocialShareInfo& info);
        void shareDialog(const sdkbox::SocialShareInfo& info);
    };

}

#endif
