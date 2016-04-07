
#include "HelloWorldScene.h"
#include "PluginShare/PluginShare.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();

    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    CCLOG("Sample Startup");

    // add logo
    auto winsize = Director::getInstance()->getWinSize();
    auto logo = Sprite::create("Logo.png");
    auto logoSize = logo->getContentSize();
    logo->setPosition(Vec2(logoSize.width / 2,
                           winsize.height - logoSize.height / 2));
    addChild(logo);

    // add quit button
    auto label = Label::createWithSystemFont("QUIT", "sans", 32);
    auto quit = MenuItemLabel::create(label, [](Ref*){
        exit(0);
    });
    auto labelSize = label->getContentSize();
    quit->setPosition(Vec2(winsize.width / 2 - labelSize.width / 2 - 16,
                           -winsize.height / 2 + labelSize.height / 2 + 16));
    addChild(Menu::create(quit, NULL));

    // add test menu
    createTestMenu();

    return true;
}

void HelloWorld::createTestMenu()
{
    sdkbox::PluginShare::setListener(this);

    auto menu = Menu::create();

    menu->addChild(MenuItemLabel::create(Label::createWithSystemFont("Share", "sans", 24), [](Ref*){
        CCLOG("Share");
        sdkbox::SocialShareInfo info;
        info.text = "#sdkbox(www.sdkbox.com) - the cure for sdk fatigue ";
        info.title = "sdkbox";
        info.image = "http://www.sdkbox.com/assets/images/logo.png";
        info.link = "http://www.sdkbox.com";
        info.platform = sdkbox::SocialPlatform::Platform_Select;
        sdkbox::PluginShare::share(info);
    }));

    menu->alignItemsVerticallyWithPadding(10);
    addChild(menu);
}

void HelloWorld::onShareState(const sdkbox::SocialShareResponse& response)
{
    switch (response.state) {
        case sdkbox::SocialShareState::SocialShareStateNone: {
            CCLOG("SharePlugin::onShareState none");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateUnkonw: {
            CCLOG("SharePlugin::onShareState unkonw");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateBegin: {
            CCLOG("SharePlugin::onShareState begin");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateSuccess: {
            CCLOG("SharePlugin::onShareState success");
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateFail: {
            CCLOG("SharePlugin::onShareState fail, error:%s", response.error.c_str());
            break;
        }
        case sdkbox::SocialShareState::SocialShareStateCancelled: {
            CCLOG("SharePlugin::onShareState cancelled");
            break;
        }
        case sdkbox::SocialShareStateSelectShow: {
            CCLOG("SharePlugin::onShareState show pancel %d", response.platform);
            break;
        }
        case sdkbox::SocialShareStateSelectCancelled: {
            CCLOG("SharePlugin::onShareState show pancel cancelled %d", response.platform);
            break;
        }
        case sdkbox::SocialShareStateSelected: {
            CCLOG("SharePlugin::onShareState show pancel selected %d", response.platform);
            break;
        }
        default: {
            CCLOG("SharePlugin::onShareState");
            break;
        }
    }
}

