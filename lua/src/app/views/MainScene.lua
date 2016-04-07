
local MainScene = class("MainScene", cc.load("mvc").ViewBase)

function MainScene:onCreate()
    print("Sample Startup")

    local label = cc.Label:createWithSystemFont("QUIT", "sans", 32)
    local quit = cc.MenuItemLabel:create(label)
    quit:onClicked(function()
        os.exit(0)
    end)
    local size = label:getContentSize()
    local menu = cc.Menu:create(quit)
    menu:setPosition(display.right - size.width / 2 - 16, display.bottom + size.height / 2 + 16)
    self:addChild(menu)

    self:setupTestMenu()
end

function MainScene:setupTestMenu()
    sdkbox.PluginShare:init()
    sdkbox.PluginShare:setListener(function(responsed)
        dump(responsed)
    end)

    local menu = cc.Menu:create()

    local label1 = cc.Label:createWithSystemFont("Share", "sans", 28)
    local item1 = cc.MenuItemLabel:create(label1)
    item1:onClicked(function()
        print("Share")
        local shareInfo = {}
        shareInfo.text = '#sdkbox(www.sdkbox.com) - the cure for sdk fatigue - from lua - '
        shareInfo.title = "sdkbox"
        shareInfo.image = "http://www.sdkbox.com/assets/images/logo.png"
        shareInfo.link = "http://www.sdkbox.com"
        shareInfo.platform = sdkbox.SocialPlatform.Platform_Select
        printf("sdkbox.SocialPlatform.Platform_Select = %s", tostring(sdkbox.SocialPlatform.Platform_Select))
        sdkbox.PluginShare:share(shareInfo)
    end)
    menu:addChild(item1)

    menu:alignItemsVerticallyWithPadding(24)
    self:addChild(menu)
end

return MainScene
