/****************************************************************************

 Copyright (c) 2014-2016 SDKBOX Inc

 ****************************************************************************/

#ifndef _PLUGIN_SHARE_TWITTER_WRAPPER_H_
#define _PLUGIN_SHARE_TWITTER_WRAPPER_H_

#include "json98.h"
#include "ShareConnector.h"

namespace sdkbox {

    class ShareTwitterConnector : public ShareConnector {
    public:
        ShareTwitterConnector() {
            platform = sdkbox::SocialPlatform::Platform_Twitter;
        }

        bool init(const Json& config);
        void share(const sdkbox::SocialShareInfo& info);
        void shareDialog(const sdkbox::SocialShareInfo& info);
        void shareRequest(const sdkbox::SocialShareInfo& info);
    };

}

#endif
