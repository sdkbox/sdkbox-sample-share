#include "PluginShareJSHelper.h"
#include "PluginShare/PluginShare.h"
#include "SDKBoxJSHelper.h"

#if (COCOS2D_VERSION < 0x00030000)
#define Ref CCObject
#define Director CCDirector
#define getInstance sharedDirector
#define schedule scheduleSelector
#endif

jsval shareInfo_to_jsval(JSContext* cx, const sdkbox::SocialShareResponse& response) {
    JS::RootedObject proto(cx);
    JS::RootedObject parent(cx);
#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS::RootedObject jsRet(cx, JS_NewObject(cx, NULL, proto, parent));
#else
    JSObject *jsRet = JS_NewObject(cx, NULL, NULL, NULL);
#endif

#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS::RootedValue state(cx);
#else
    jsval state;
#endif
    state = int32_to_jsval(cx, response.state);
#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS_SetProperty(cx, jsRet, "state", state);
#else
    JS_SetProperty(cx, jsRet, "state", &state);
#endif

#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS::RootedValue errStr(cx);
#else
    jsval errStr;
#endif
    errStr = std_string_to_jsval(cx, response.error);
#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS_SetProperty(cx, jsRet, "error", errStr);
#else
    JS_SetProperty(cx, jsRet, "error", &errStr);
#endif

    std::string platform_name;
#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS::RootedValue platformInt(cx);
#else
    jsval platformInt;
#endif
    platformInt = INT_TO_JSVAL(response.platform);
#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS_SetProperty(cx, jsRet, "platform", platformInt);
#else
    JS_SetProperty(cx, jsRet, "platform", &platformInt);
#endif

    return OBJECT_TO_JSVAL(jsRet);
}

extern JSObject* jsb_sdkbox_PluginShare_prototype;
static JSContext* s_cx = nullptr;

class ShareCallbackJS: public cocos2d::Ref {
public:
    ShareCallbackJS();
    void schedule();
    void notityJs(float dt);
    int transParams(jsval** pp);

    std::string _name;

    jsval _paramVal[2];
    int _paramLen;
    sdkbox::SocialShareResponse _shareResponse;
};

class ShareListenerJS : public sdkbox::ShareListener, public sdkbox::JSListenerBase
{
public:
    ShareListenerJS():sdkbox::JSListenerBase() {
    }

    void onShareState(const sdkbox::SocialShareResponse& response) {
        JSContext* cx = s_cx;
        ShareCallbackJS* cb = new ShareCallbackJS();
        cb->_shareResponse = response;
        cb->_name = "onShareState";
        cb->_paramLen = 1;
        cb->schedule();
    }

    void invokeJS(const char* func, ShareCallbackJS* cb) {
        if (!s_cx) {
            return;
        }
        JSContext* cx = s_cx;
        const char* func_name = func;
        JS::RootedObject obj(cx, getJSDelegate());
        JSAutoCompartment ac(cx, obj);

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
        bool hasAction;
        JS::RootedValue retval(cx);
        JS::RootedValue func_handle(cx);
#else
        bool hasAction;
        jsval retval;
        JS::RootedValue func_handle(cx);
#endif
#elif defined(JS_VERSION)
        JSBool hasAction;
        jsval retval;
        jsval func_handle;
#endif
        int valueSize = 0;
        jsval* pVals = nullptr;
        valueSize = cb->transParams(&pVals);

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, func_name, &func_handle)) {
                return;
            }
            if(func_handle == JSVAL_VOID) {
                return;
            }

#if MOZJS_MAJOR_VERSION >= 31
            if (0 == valueSize) {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::empty(), &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(valueSize, pVals), &retval);
            }
#else
            if (0 == valueSize) {
                JS_CallFunctionName(cx, obj, func_name, 0, nullptr, &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, valueSize, pVals, &retval);
            }
#endif
        }
    }

};


ShareCallbackJS::ShareCallbackJS():
_paramLen(0) {
}

void ShareCallbackJS::schedule() {
    retain();
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(ShareCallbackJS::notityJs), this, 0.1, 0, 0.0f, false);
    autorelease();
}

void ShareCallbackJS::notityJs(float dt) {
    sdkbox::ShareListener* lis = sdkbox::PluginShare::getListener();
    ShareListenerJS* l = dynamic_cast<ShareListenerJS*>(lis);
    if (l) {
        l->invokeJS(_name.c_str(), this);
    }
    release();
}

int ShareCallbackJS::transParams(jsval** pp) {
    JSContext* cx = s_cx;
    _paramVal[0] = shareInfo_to_jsval(cx, _shareResponse);

    *pp = _paramVal;
    return _paramLen;
}


#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
bool js_PluginShareJS_PluginShare_setListener(JSContext *cx, uint32_t argc, jsval *vp)
#else
bool js_PluginShareJS_PluginShare_setListener(JSContext *cx, uint32_t argc, jsval *vp)
#endif
#elif defined(JS_VERSION)
JSBool js_PluginShareJS_PluginShare_setListener(JSContext *cx, uint32_t argc, jsval *vp)
#endif
{
    s_cx = cx;
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    if (argc == 1) {

        if (!args.get(0).isObject())
        {
            ok = false;
        }

        JSB_PRECONDITION2(ok, cx, false, "js_PluginShareJS_PluginShare_setIAPListener : Error processing arguments");
        ShareListenerJS* wrapper = new ShareListenerJS();
        wrapper->setJSDelegate(args.get(0));
        sdkbox::PluginShare::setListener(wrapper);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportError(cx, "js_PluginShareJS_PluginShare_setIAPListener : wrong number of arguments");
    return false;
}

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
bool js_PluginShareJS_PluginShare_share(JSContext *cx, uint32_t argc, jsval *vp)
#else
bool js_PluginShareJS_PluginShare_share(JSContext *cx, uint32_t argc, jsval *vp)
#endif
#elif defined(JS_VERSION)
JSBool js_PluginShareJS_PluginShare_share(JSContext *cx, uint32_t argc, jsval *vp)
#endif
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;

    do {
        if (argc == 1) {
            sdkbox::SocialShareInfo shareInfo;
            cocos2d::ValueMap map;
            ok &= jsval_to_ccvaluemap(cx, args.get(0), &map);
            shareInfo.text = map["text"].asString();
            shareInfo.title = map["title"].asString();
            shareInfo.image = map["image"].asString();
            shareInfo.link = map["link"].asString();
            shareInfo.showDialog = map["showDialog"].asBool();
            shareInfo.platform = (sdkbox::SocialPlatform)map["platform"].asInt();
            sdkbox::PluginShare::share(shareInfo);

            return true;
        }
    } while (0);

    JS_ReportError(cx, "js_PluginShareJS_PluginShare_share : wrong number of arguments");
    return false;
}

// #if defined(MOZJS_MAJOR_VERSION)
// #if MOZJS_MAJOR_VERSION >= 33
// bool js_PluginShareJS_PluginShare_shareDialog(JSContext *cx, uint32_t argc, jsval *vp)
// #else
// bool js_PluginShareJS_PluginShare_shareDialog(JSContext *cx, uint32_t argc, jsval *vp)
// #endif
// #elif defined(JS_VERSION)
// JSBool js_PluginShareJS_PluginShare_shareDialog(JSContext *cx, uint32_t argc, jsval *vp)
// #endif
// {
//     JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
//     bool ok = true;

//     do {
//         if (argc == 1) {
//             sdkbox::PluginShare::ShareInfo shareInfo;
//             cocos2d::ValueMap map;
//             ok &= jsval_to_ccvaluemap(cx, args.get(0), &map);
//             cocos2d::Value v = map["text"];
//             shareInfo.text = v.asString();
//             sdkbox::PluginShare::shareDialog(shareInfo);

//             return true;
//         }
//     } while (0);

//     JS_ReportError(cx, "js_PluginShareJS_PluginShare_shareDialog : wrong number of arguments");
//     return false;
// }

#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
void share_set_constants(JSContext* cx, const JS::RootedObject& obj, const std::string& name, const std::map<std::string, int>& params)
#else
void share_set_constants(JSContext* cx, JSObject* obj, const std::string& name, const std::map<std::string, int>& params)
#endif
{
    jsval val = sdkbox::std_map_string_int_to_jsval(cx, params);

    JS::RootedValue rv(cx);
    rv = val;
#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
    JS_SetProperty(cx, obj, name.c_str(), rv);
#else
    JS_SetProperty(cx, obj, name.c_str(), rv.address());
#endif
}

#if defined(MOZJS_MAJOR_VERSION) and MOZJS_MAJOR_VERSION >= 26
void share_register_constants(JSContext* cx, const JS::RootedObject& obj)
#else
void share_register_constants(JSContext* cx, JSObject* obj)
#endif
{
    std::map<std::string, int> enums;
    enums.clear();
    enums.insert(std::make_pair("SocialShareStateNone", 0));
    enums.insert(std::make_pair("SocialShareStateUnkonw", 1));
    enums.insert(std::make_pair("SocialShareStateBegin", 2));
    enums.insert(std::make_pair("SocialShareStateSuccess", 3));
    enums.insert(std::make_pair("SocialShareStateFail", 4));
    enums.insert(std::make_pair("SocialShareStateCancelled", 5));
    enums.insert(std::make_pair("SocialShareStateSelectShow", 6));
    enums.insert(std::make_pair("SocialShareStateSelected", 7));
    enums.insert(std::make_pair("SocialShareStateSelectCancelled", 8));
    share_set_constants(cx, obj, "SocialShareState", enums);

    enums.clear();
    enums.insert(std::make_pair("Platform_Unknow", 0));
    enums.insert(std::make_pair("Platform_Twitter", 1));
    enums.insert(std::make_pair("Platform_Facebook", 2));
    enums.insert(std::make_pair("Platform_SMS", 5));
    enums.insert(std::make_pair("Platform_EMail", 6));
    enums.insert(std::make_pair("Platform_Mail", 6));

    enums.insert(std::make_pair("Platform_Select", 3));
    enums.insert(std::make_pair("Platform_All", 4));
    share_set_constants(cx, obj, "SocialPlatform", enums);

}

#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
void register_all_PluginShareJS_helper(JSContext* cx, JS::HandleObject global) {
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginShare", &pluginObj);

    JS_DefineFunction(cx, pluginObj, "setListener", js_PluginShareJS_PluginShare_setListener, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, pluginObj, "share", js_PluginShareJS_PluginShare_share, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    // JS_DefineFunction(cx, pluginObj, "shareDialog", js_PluginShareJS_PluginShare_shareDialog, 1, JSPROP_READONLY | JSPROP_PERMANENT);

    sdkbox::getJsObjOrCreat(cx, global, "sdkbox", &pluginObj);
    share_register_constants(cx, pluginObj);
}
#else
void register_all_PluginShareJS_helper(JSContext* cx, JSObject* global) {
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, JS::RootedObject(cx, global), "sdkbox.PluginShare", &pluginObj);

    JS_DefineFunction(cx, pluginObj, "setListener", js_PluginShareJS_PluginShare_setListener, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, pluginObj, "share", js_PluginShareJS_PluginShare_share, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    // JS_DefineFunction(cx, pluginObj, "shareDialog", js_PluginShareJS_PluginShare_shareDialog, 1, JSPROP_READONLY | JSPROP_PERMANENT);

    share_register_constants(cx, pluginObj);
}
#endif
#elif defined(JS_VERSION)
void register_all_PluginShareJS_helper(JSContext* cx, JSObject* global) {
    jsval pluginVal;
    JSObject* pluginObj;
    pluginVal = sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginShare", &pluginObj);

    JS_DefineFunction(cx, pluginObj, "setListener", js_PluginShareJS_PluginShare_setListener, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    JS_DefineFunction(cx, pluginObj, "share", js_PluginShareJS_PluginShare_share, 1, JSPROP_READONLY | JSPROP_PERMANENT);
    // JS_DefineFunction(cx, pluginObj, "shareDialog", js_PluginShareJS_PluginShare_shareDialog, 1, JSPROP_READONLY | JSPROP_PERMANENT);

    share_register_constants(cx, pluginObj);
}
#endif
