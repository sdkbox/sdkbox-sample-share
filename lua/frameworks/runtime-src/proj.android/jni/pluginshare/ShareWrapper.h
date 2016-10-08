/****************************************************************************

 Copyright (c) 2014-2016 SDKBOX Inc

 ****************************************************************************/

#ifndef _PLUGIN_SHARE_WRAPPER_H_
#define _PLUGIN_SHARE_WRAPPER_H_

#include "PluginShare.h"
#include "json98.h"
#include "ShareConnector.h"

#define TAG "Share"
#define VERSION "1.0"

namespace sdkbox {

    class ShareWrapper {
    public:
        static ShareWrapper* getInstance();

        virtual bool init(const char* jsonConfig = 0)=0;
        virtual void setListener(ShareListener* listener)=0;
        virtual ShareListener* getListener()=0;
        virtual void removeListener()=0;
        virtual void setSharePanelTitle(const std::string& s)=0;
        virtual void setSharePanelCancel(const std::string& s)=0;
        virtual void share(const sdkbox::SocialShareInfo& info)=0;
        virtual void shareDialog(const sdkbox::SocialShareInfo& info)=0;
        virtual void shareResponed(sdkbox::SocialShareState state, sdkbox::SocialPlatform platform, const std::string& error = "")=0;
        virtual void shareResponed(const sdkbox::SocialShareResponse& resp)=0;
        virtual void shareToPlatform(sdkbox::SocialPlatform platform)=0;
    };

    class ShareWrapperDisabled : public ShareWrapper {
    public:
        bool init(const char* jsonConfig = 0) { return false; }
        void setListener(ShareListener* listener) {}
        ShareListener* getListener() { return NULL; }
        void removeListener() {}
        void setSharePanelTitle(const std::string& s) {};
        void setSharePanelCancel(const std::string& s) {};
        void share(const sdkbox::SocialShareInfo& info) {}
        void shareDialog(const sdkbox::SocialShareInfo& info) {}
        void shareResponed(sdkbox::SocialShareState state, sdkbox::SocialPlatform platform, const std::string& error = ""){};
        void shareResponed(const sdkbox::SocialShareResponse& resp){};
        void shareToPlatform(sdkbox::SocialPlatform platform){};
    };


    class ShareWrapperEnabled : public ShareWrapper {
    public:
        ShareWrapperEnabled();

        bool init(const char* jsonConfig = 0);
        void setListener(ShareListener* listener);
        ShareListener* getListener();
        void removeListener();

        void setSharePanelTitle(const std::string& s);
        void setSharePanelCancel(const std::string& s);

        void share(const sdkbox::SocialShareInfo& info);
        void shareDialog(const sdkbox::SocialShareInfo& info);

        void shareResponed(sdkbox::SocialShareState state, sdkbox::SocialPlatform platform, const std::string& error = "");
        void shareResponed(const sdkbox::SocialShareResponse& resp);
        void shareToPlatform(sdkbox::SocialPlatform platform);

    protected:
        bool nativeInit(const Json& config);
        std::string nativeSDKVersion();
        void regEventForAndroid();
        void showSharePanel(const std::vector<sdkbox::SocialPlatform>& platforms);
        std::vector<sdkbox::SocialPlatform> getValidPlatform();
        ShareConnector* getConnector(sdkbox::SocialPlatform platform);

        ShareListener* _listener;

        std::map<std::string, ShareConnector*> connectors;
        sdkbox::SocialShareInfo shareInfo;
        std::string panelTitle;
        std::string panelCancel;
    };
}

#endif
