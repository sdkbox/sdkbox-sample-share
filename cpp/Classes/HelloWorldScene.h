#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "PluginShare/PluginShare.h"

class HelloWorld : public cocos2d::Layer, public sdkbox::ShareListener
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    void afterCaptureScreen(bool yes, const std::string &outputFilename);

private:
    void createTestMenu();

    virtual void onShareState(const sdkbox::SocialShareResponse& response);
    
    std::string _captureFilename;
};

#endif // __HELLOWORLD_SCENE_H__
