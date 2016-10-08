
#ifndef _PLUGIN_SHARE_CONNECTOR_H_
#define _PLUGIN_SHARE_CONNECTOR_H_

#include "json98.h"
#include "PluginShare.h"

namespace sdkbox {

    class ShareConnector {
    public:
        ShareConnector();
        virtual ~ShareConnector();
        virtual bool init(const Json& config);
        const std::string& getName();
        void setPlatform(const std::string& platform);
        void setPlatform(sdkbox::SocialPlatform platform);
        sdkbox::SocialPlatform getPlatform();

        virtual void share(const sdkbox::SocialShareInfo& info);
        virtual void shareDialog(const sdkbox::SocialShareInfo& info);

    protected:
        std::string platformName;
        sdkbox::SocialPlatform platform;
    };
}

#endif
