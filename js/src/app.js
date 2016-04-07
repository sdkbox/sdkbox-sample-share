
var HelloWorldLayer = cc.Layer.extend({
    sprite:null,
    ctor:function () {
        //////////////////////////////
        // 1. super init first
        this._super();

        cc.log("Sample Startup")

        this.createTestMenu();

        var winsize = cc.winSize;

        var logo = new cc.Sprite("res/Logo.png");
        var logoSize = logo.getContentSize();
        logo.x = logoSize.width / 2;
        logo.y = winsize.height - logoSize.height / 2;
        this.addChild(logo);

        // var quit = new cc.MenuItemLabel(new cc.LabelTTF("QUIT", "sans", 32), function() {
        //     cc.log("QUIT");
        // });
        // var menu = new cc.Menu(quit);
        // var size = quit.getContentSize();
        // menu.x = winsize.width - size.width / 2 - 16;
        // menu.y = size.height / 2 + 16;
        // this.addChild(menu);

        return true;
    },

    createTestMenu:function() {
        sdkbox.PluginShare.init();
        sdkbox.PluginShare.setListener({
            onShareState: function(response) {
                cc.log("PluginShare onSharestate:" + response.state + " error:" + response.error);
                if (response.state == sdkbox.SocialShareState.SocialShareStateSuccess) {
                    cc.log("post success");
                }
            }
        })

        var menu = new cc.Menu();

        var item1 = new cc.MenuItemLabel(new cc.LabelTTF("Share", "sans", 28), function() {
            cc.log("Share");
            var shareInfo = {};
            shareInfo.text = "#sdkbox(www.sdkbox.com) - the cure for sdk fatigue - from js - ";
            shareInfo.title = "sdkbox";
            shareInfo.image = "http://www.sdkbox.com/assets/images/logo.png";
            shareInfo.link = "http://www.sdkbox.com";
            shareInfo.platform = sdkbox.SocialPlatform.Platform_Select;
            sdkbox.PluginShare.share(shareInfo);
        });
        menu.addChild(item1);

        var winsize = cc.winSize;
        menu.x = winsize.width / 2;
        menu.y = winsize.height / 2;
        menu.alignItemsVerticallyWithPadding(20);
        this.addChild(menu);
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter:function () {
        this._super();
        var layer = new HelloWorldLayer();
        this.addChild(layer);
    }
});

