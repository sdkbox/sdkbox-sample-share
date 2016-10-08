
#ifndef _PLUGIN_SHARE_BASE_CONNECTOR_H_
#define _PLUGIN_SHARE_BASE_CONNECTOR_H_

#include "json98.h"
#include "ShareConnector.h"

namespace sdkbox {

    class ShareBaseConnector : public ShareConnector {
    public:
        ShareBaseConnector();
        virtual bool init(const Json& config);

        virtual void share(const sdkbox::SocialShareInfo& info);
        virtual void shareDialog(const sdkbox::SocialShareInfo& info);

    protected:
        bool initNative(const Json& config);
        void shareNative(const sdkbox::SocialShareInfo& info);
        void shareDialogNative(const sdkbox::SocialShareInfo& info);

        bool createShareUnit();
    };
}

#endif
