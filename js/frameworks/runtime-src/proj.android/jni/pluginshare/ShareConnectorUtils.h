
#ifndef _PLUGIN_SHARE_CONNECTOR_UTILS_H_
#define _PLUGIN_SHARE_CONNECTOR_UTILS_H_

#include "PluginShare.h"
#include "ShareConnector.h"


namespace sdkbox {
    class ShareConnectorUtils {
    public:
        static ShareConnector* createConnector(const std::string& platform, const Json& config);
        static bool shareInfo2Map(const sdkbox::SocialShareInfo& shareInfo, std::map<std::string, std::string>& map);
        static std::string platformType2String(sdkbox::SocialPlatform platform);
        static sdkbox::SocialPlatform platformString2Type(std::string platform);
        static std::string shareInfo2String(const sdkbox::SocialShareInfo& info);
    };
}

#endif
