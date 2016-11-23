/****************************************************************************

 Copyright (c) 2014-2016 SDKBOX Inc

 ****************************************************************************/

#ifndef _PLUGIN_SHARE_H_
#define _PLUGIN_SHARE_H_

#include <string>
#include <map>
#include <sdkbox/Sdkbox.h>

namespace sdkbox {

    class ShareListener;
    class PluginShare {
    public:

        /*** ENUM CHANGE ****/
        /*
         *
         * USE sdkbox::SocialShareInfo instead of sdkbox::PluginShare::ShareInfo
         * USE sdkbox::SocialShareState instead of sdkbox::PluginShare::ShareState
         * USE sdkbox::SocialShareResponse instead of sdkbox::PluginShare::ShareResponse
         *
         * enum define in sdkbox/Sdkbox.h
         */
        // enum ShareState {
        //     ShareStateNone,
        //     ShareStateUnkonw,
        //     ShareStateBegin,
        //     ShareStateSuccess,
        //     ShareStateFail,
        //     ShareStateCancelled
        // };

        // struct ShareResponse {
        //     ShareState state;
        //     std::string error;
        //     //SharePlatform platform;
        // };

        // struct ShareInfo {
        //     std::string text;
        // };

        /**
         *  initialize the plugin instance.
         */
        static bool init(const char* jsonConfig = 0);

        /**
         * Set listener to listen for share events
         */
        static void setListener(ShareListener* listener);

        /**
         * Get the listener
         */
        static ShareListener* getListener();

        /**
         * Remove the listener, and can't listen to events anymore
         */
        static void removeListener();

        /**
         * set custome share platform choose panel title, default is "Share to"
         */
        static void setSharePanelTitle(const std::string& s);

        /**
         * set custome share platform choose panel cancel button, default is "Cancel"
         */
        static void setSharePanelCancel(const std::string& s);

        /**
         * Share content
         */
        static void share(const sdkbox::SocialShareInfo& info);

        };

    class ShareListener {
    public:

        /**
         * Notifies the delegate that share completion
         */
        virtual void onShareState(const sdkbox::SocialShareResponse& response) {};
    };
}

#endif
