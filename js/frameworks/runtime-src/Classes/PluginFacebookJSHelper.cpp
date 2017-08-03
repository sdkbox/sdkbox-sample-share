
#include "PluginFacebookJSHelper.h"
#include "SDKBoxJSHelper.h"
#include "PluginFacebook/PluginFacebook.h"

using namespace sdkbox;

static const std::string FBGraphUser_ID          ("uid");
static const std::string FBGraphUser_NAME        ("name");
static const std::string FBGraphUser_FIRST_NAME  ("firstName");
static const std::string FBGraphUser_LAST_NAME   ("lastName");
static const std::string FBGraphUser_INSTALLED   ("isInstalled");

static JSContext* s_cx = nullptr;


JSOBJECT* FBGraphUserToJS(JSContext* cx, const sdkbox::FBGraphUser& info) {

    JS_INIT_CONTEXT_FOR_UPDATE(cx);

    JSOBJECT* jsobj= JS_NEW_OBJECT(cx);

    for( std::pair<std::string, std::string> _p : info.getFields() ) {
        addProperty( cx, jsobj, _p.first.c_str(), _p.second );
    }

    return jsobj;
}

JSOBJECT* FBInvitableFriendsInfoToJS( JSContext* cx, const sdkbox::FBInvitableFriendsInfo& ifap ) {

    JS_INIT_CONTEXT_FOR_UPDATE(cx);

    // collection of FBGraphUser
    JSOBJECT *jarr= JS_NEW_ARRAY(cx, ifap.getNumInvitationTokens());

    uint32_t index=0;
    for( auto u= ifap.begin(); u!=ifap.end(); ++u ) {
        JSOBJECT* gu= FBGraphUserToJS( cx, *(u) );
        JS_ARRAY_SET( cx, jarr, index, gu );
        index++;
    }

    // cursor data
    JSOBJECT* jsobj= JS_NEW_OBJECT(cx);
    addProperty( cx, jsobj, "next_url", ifap.getNextURL() );
    addProperty( cx, jsobj, "prev_url", ifap.getPrevURL() );
    addProperty( cx, jsobj, "prev_cursor", ifap.getPrevCursor() );
    addProperty( cx, jsobj, "next_cursor", ifap.getNextCursor() );

    // data
    JSOBJECT* ret= JS_NEW_OBJECT(cx);
    addProperty( cx, ret, "data", jarr );
    addProperty( cx, ret, "paging", jsobj );

    return ret;
}

class FacebookListenerJsHelper : public sdkbox::FacebookListener, public sdkbox::JSListenerBase
{
public:
    FacebookListenerJsHelper():sdkbox::JSListenerBase() {
    }

private:
    void invokeDelegate(std::string& fName, JS::Value dataVal[], int argc) {
        if (!s_cx) {
            return;
        }
        JSContext* cx = s_cx;
        const char* func_name = fName.c_str();

        JS::RootedObject obj(cx, getJSDelegate());
        JSAutoCompartment ac(cx, obj);

#if MOZJS_MAJOR_VERSION >= 31
        bool hasAction;
        JS::RootedValue retval(cx);
        JS::RootedValue func_handle(cx);
#elif MOZJS_MAJOR_VERSION >= 28
        bool hasAction;
        jsval retval;
        JS::RootedValue func_handle(cx);
#else
        JSBool hasAction;
        jsval retval;
        jsval func_handle;
#endif

        if (JS_HasProperty(cx, obj, func_name, &hasAction) && hasAction) {
            if(!JS_GetProperty(cx, obj, func_name, &func_handle)) {
                return;
            }
            if(func_handle == JS::NullValue()) {
                return;
            }

#if MOZJS_MAJOR_VERSION >= 31
            if (0 == argc) {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::empty(), &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, JS::HandleValueArray::fromMarkedLocation(argc, dataVal), &retval);
            }
#else
            if (0 == argc) {
                JS_CallFunctionName(cx, obj, func_name, 0, nullptr, &retval);
            } else {
                JS_CallFunctionName(cx, obj, func_name, argc, dataVal, &retval);
            }
#endif
        }
    }

public:

    virtual void onLogin(bool isLogin, const std::string& error)
    {
        std::string name("onLogin");
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::BooleanValue(isLogin);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, error);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onSharedSuccess(const std::string& message)
    {
        std::string name("onSharedSuccess");
        JS::Value dataVal[1];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = SB_STR_TO_JSVAL(s_cx, message);
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onSharedFailed(const std::string& message)
    {
        std::string name("onSharedFailed");
        JS::Value dataVal[1];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = SB_STR_TO_JSVAL(s_cx, message);
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onSharedCancel()
    {
        std::string name("onSharedCancel");
        JS::Value dataVal[0];
        invokeDelegate(name, dataVal, 0);
    }
    virtual void onAPI(const std::string& tag, const std::string& jsonData)
    {
        std::string name("onAPI");
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = SB_STR_TO_JSVAL(s_cx, tag);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, jsonData);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onPermission(bool isLogin, const std::string& error)
    {
        std::string name("onPermission");
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::BooleanValue(isLogin);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, error);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onFetchFriends(bool ok, const std::string& msg)
    {
        std::string name("onFetchFriends");
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::BooleanValue(ok);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, msg);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onInviteFriendsResult(bool ok, const std::string& msg)
    {
        std::string name("onInviteFriendsResult");
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::BooleanValue(ok);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, msg);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onInviteFriendsWithInviteIdsResult(bool ok, const std::string& msg)
    {
        std::string name("onInviteFriendsWithInviteIdsResult");
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::BooleanValue(ok);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, msg);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onRequestInvitableFriends(const sdkbox::FBInvitableFriendsInfo& invitable_friends_and_pagination )
    {
        std::string name("onRequestInvitableFriends");
        JS::Value dataVal[1];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::ObjectValue(*FBInvitableFriendsInfoToJS(s_cx, invitable_friends_and_pagination));
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onGetUserInfo(const sdkbox::FBGraphUser& userInfo )
    {
        std::string name("onGetUserInfo");
        JS::Value dataVal[1];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::ObjectValue(*FBGraphUserToJS( s_cx, userInfo ));
        invokeDelegate(name, dataVal, 1);
    }
    virtual void onRequestGiftResult(bool result, const std::string& msg)
    {
        std::string name(__FUNCTION__);
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif

        dataVal[0] = JS::BooleanValue(result);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, msg);
        invokeDelegate(name, dataVal, 2);
    }
    virtual void onSendGiftResult(bool result, const std::string& msg)
    {
        std::string name(__FUNCTION__);
        JS::Value dataVal[2];

#if MOZJS_MAJOR_VERSION < 52
        JSB_AUTOCOMPARTMENT_WITH_GLOBAL_OBJCET
#endif


        dataVal[0] = JS::BooleanValue(result);
        dataVal[1] = SB_STR_TO_JSVAL(s_cx, msg);
        invokeDelegate(name, dataVal, 2);
    }
};

static sdkbox::FBShareInfo map_to_FBShareInfo(const std::map<std::string, std::string>& dict)
{
    sdkbox::FBShareInfo info;

    if (dict.find("title") != dict.end())
    {
        info.title = dict.find("title")->second;
    }
    if (dict.find("link") != dict.end())
    {
        info.link = dict.find("link")->second;
    }
    if (dict.find("text") != dict.end())
    {
        info.text = dict.find("text")->second;
    }
    if (dict.find("image") != dict.end())
    {
        info.image = dict.find("image")->second;
    }
    if (dict.find("type") != dict.end())
    {
        std::string type = dict.find("type")->second;
        if (type.compare("link") == 0)
        {
            info.type = sdkbox::FB_LINK;
        }
        else if (type.compare("photo") == 0)
        {
            info.type = sdkbox::FB_PHOTO;
        }
    }

    return info;
}

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_setListener(JSContext *cx, uint32_t argc, JS::Value *vp)
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_setListener(JSContext *cx, unsigned argc, JS::Value *vp)
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

        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_setListener : Error processing arguments");
        FacebookListenerJsHelper* lis = new FacebookListenerJsHelper();
        lis->setJSDelegate(cx, args.get(0));
        sdkbox::PluginFacebook::setListener(lis);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_setListener : wrong number of arguments");
    return false;
}


#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_share(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_share : Error processing arguments");
        sdkbox::PluginFacebook::share(map_to_FBShareInfo(arg0));
        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_share : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_share(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::share(map_to_FBShareInfo(arg0));
        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_api(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 4) {
        std::string path;
        ok &= jsval_to_std_string(cx, args.get(0), &path);
        std::string method;
        ok &= jsval_to_std_string(cx, args.get(1), &method);
        std::map<std::string, std::string> param;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(2), &param);
        std::string tag;
        ok &= jsval_to_std_string(cx, args.get(3), &tag);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_api : Error processing arguments");
        sdkbox::PluginFacebook::api(path, method, param, tag);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_api : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_api(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 4) {
        std::string path;
        ok &= jsval_to_std_string(cx, argv[0], &path);
        std::string method;
        ok &= jsval_to_std_string(cx, argv[1], &method);
        std::map<std::string, std::string> param;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[2], &param);
        std::string tag;
        ok &= jsval_to_std_string(cx, argv[3], &tag);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::api(path, method, param, tag);
        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif


#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_dialog(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_dialog : Error processing arguments");
        sdkbox::PluginFacebook::dialog(map_to_FBShareInfo(arg0));
        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_dialog : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_dialog(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::dialog(map_to_FBShareInfo(arg0));
        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_requestReadPermissions(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= jsval_to_std_vector_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_requestReadPermissions : Error processing arguments");
        sdkbox::PluginFacebook::requestReadPermissions(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_requestReadPermissions : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_requestReadPermissions(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= sdkbox::jsval_to_std_vector_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::requestReadPermissions(arg0);
        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_requestPublishPermissions(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= jsval_to_std_vector_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_requestPublishPermissions : Error processing arguments");
        sdkbox::PluginFacebook::requestPublishPermissions(arg0);
        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_requestPublishPermissions : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_requestPublishPermissions(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::vector<std::string> arg0;
        ok &= sdkbox::jsval_to_std_vector_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::requestPublishPermissions(arg0);
        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_getPermissionList(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        std::vector<std::string> ret = sdkbox::PluginFacebook::getPermissionList();
        JS::RootedValue jsret(cx);
        sdkbox::std_vector_string_to_jsval(cx, ret, &jsret);
        args.rval().set(jsret);
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_getPermissionList : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_getPermissionList(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 0) {
        std::vector<std::basic_string<char>, std::allocator<std::basic_string<char> > > ret = sdkbox::PluginFacebook::getPermissionList();
        jsval jsret;
        jsret = sdkbox::std_vector_string_to_jsval(cx, ret);
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_getFriends(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    if (argc == 0) {
        std::vector<sdkbox::FBGraphUser> ret = sdkbox::PluginFacebook::getFriends();
        size_t size = ret.size();
#if !defined(CCArray) && (COCOS2D_VERSION >= 0x00030000)
        cocos2d::ValueVector array;
        for (int i = 0; i < size; i++)
        {
            const sdkbox::FBGraphUser& friendInfo = ret.at(i);
            cocos2d::ValueMap friendInfoDict;
            friendInfoDict[FBGraphUser_ID] = friendInfo.uid;
            friendInfoDict[FBGraphUser_NAME] = friendInfo.name;
            friendInfoDict[FBGraphUser_FIRST_NAME] = friendInfo.firstName;
            friendInfoDict[FBGraphUser_LAST_NAME] = friendInfo.lastName;
            friendInfoDict[FBGraphUser_INSTALLED] = friendInfo.isInstalled;
            array.push_back(cocos2d::Value(friendInfoDict));
        }
        JS::RootedValue jsret(cx);
#if MOZJS_MAJOR_VERSION >= 52
        ccvaluevector_to_jsval(cx, array, &jsret);
#else
        jsret = ccvaluevector_to_jsval(cx, array);
#endif
#else
        cocos2d::CCArray *array = cocos2d::CCArray::create();
        array->retain();
        for (int i = 0; i < size; i++)
        {
            const sdkbox::FBGraphUser& friendInfo = ret.at(i);
            cocos2d::CCDictionary *friendInfoDict = cocos2d::CCDictionary::create();
            friendInfoDict->setObject(cocos2d::CCString::create(friendInfo.uid), FBGraphUser_ID);
            friendInfoDict->setObject(cocos2d::CCString::create(friendInfo.name), FBGraphUser_NAME);
            friendInfoDict->setObject(cocos2d::CCString::create(friendInfo.firstName), FBGraphUser_FIRST_NAME);
            friendInfoDict->setObject(cocos2d::CCString::create(friendInfo.lastName), FBGraphUser_LAST_NAME);
            friendInfoDict->setObject(cocos2d::CCBool::create(friendInfo.isInstalled), FBGraphUser_INSTALLED);

            array->addObject(friendInfoDict);
        }

        jsval jsret = ccarray_to_jsval(cx, array);
#endif // __COCOS2D_CCDEPRECATED_H__
        args.rval().set(jsret);
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_getFriends : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_getFriends(JSContext *cx, uint32_t argc, jsval *vp)
{
    if (argc == 0) {
        std::vector<sdkbox::FBGraphUser> ret = sdkbox::PluginFacebook::getFriends();
        size_t size = ret.size();
        CCArray *array = CCArray::create();
        array->retain();
        for (int i = 0; i < size; i++)
        {
            const sdkbox::FBGraphUser& friendInfo = ret.at(i);
            CCDictionary *friendInfoDict = CCDictionary::create();
            friendInfoDict->setObject(CCString::create(friendInfo.uid), FBGraphUser_ID);
            friendInfoDict->setObject(CCString::create(friendInfo.name), FBGraphUser_NAME);
            friendInfoDict->setObject(CCString::create(friendInfo.firstName), FBGraphUser_FIRST_NAME);
            friendInfoDict->setObject(CCString::create(friendInfo.lastName), FBGraphUser_LAST_NAME);
            friendInfoDict->setObject(CCBool::create(friendInfo.isInstalled), FBGraphUser_INSTALLED);

            array->addObject(friendInfoDict);
        }

        jsval jsret = ccarray_to_jsval(cx, array);
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_canPresentWithFBApp(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_canPresentWithFBApp : Error processing arguments");
        bool canPresent = sdkbox::PluginFacebook::canPresentWithFBApp(map_to_FBShareInfo(arg0));

        args.rval().set(JS::BooleanValue(canPresent));

        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_canPresentWithFBApp : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_canPresentWithFBApp(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        bool canPresent = sdkbox::PluginFacebook::canPresentWithFBApp(map_to_FBShareInfo(arg0));

        jsval jsret = BOOLEAN_TO_JSVAL(canPresent);
        JS_SET_RVAL(cx, vp, jsret);
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif

#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_inviteFriends(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 2) {
        std::string app_link_url;
        ok &= jsval_to_std_string(cx, args.get(0), &app_link_url);
        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments js_PluginFacebookJS_PluginFacebook_inviteFriends");

        std::string preview_image_url;
        ok &= jsval_to_std_string(cx, args.get(1), &preview_image_url);
        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments js_PluginFacebookJS_PluginFacebook_inviteFriends");

        sdkbox::PluginFacebook::inviteFriends( app_link_url, preview_image_url );

        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_inviteFriends : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_inviteFriends(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 2) {
        std::string app_link_url;
        ok &= sdkbox::jsval_to_std_string(cx, argv[0], &app_link_url);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

        std::string preview_image_url;
        ok &= sdkbox::jsval_to_std_string(cx, argv[1], &preview_image_url);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

        sdkbox::PluginFacebook::inviteFriends( app_link_url, preview_image_url );
        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif


#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_requestInvitableFriends(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, args.get(0), &arg0);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_requestInvitableFriends : Error processing arguments");

        sdkbox::PluginFacebook::requestInvitableFriends(arg0);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_requestInvitableFriends : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_requestInvitableFriends(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 1) {
        std::map<std::string, std::string> arg0;
        ok &= sdkbox::jsval_to_std_map_string_string(cx, argv[0], &arg0);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");
        sdkbox::PluginFacebook::requestInvitableFriends(arg0);

        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif


#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginFacebookJS_PluginFacebook_inviteFriendsWithInviteIds(JSContext *cx, uint32_t argc, JS::Value *vp)
{
    JS::CallArgs args = JS::CallArgsFromVp(argc, vp);
    bool ok = true;
    if (argc == 3) {
        std::vector<std::string> ids;
        ok &= jsval_to_std_vector_string(cx, args.get(0), &ids);
        JSB_PRECONDITION2(ok, cx, false, "js_PluginFacebookJS_PluginFacebook_inviteFriendsWithInviteIds : Error processing arguments");

        std::string title;
        ok &= jsval_to_std_string(cx, args.get(1), &title);
        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

        std::string text;
        ok &= jsval_to_std_string(cx, args.get(2), &text);
        JSB_PRECONDITION2(ok, cx, false, "Error processing arguments");

        sdkbox::PluginFacebook::inviteFriendsWithInviteIds(ids, title, text);

        args.rval().setUndefined();
        return true;
    }
    JS_ReportErrorUTF8(cx, "js_PluginFacebookJS_PluginFacebook_inviteFriendsWithInviteIds : wrong number of arguments");
    return false;
}
#elif defined(JS_VERSION)
JSBool js_PluginFacebookJS_PluginFacebook_inviteFriendsWithInviteIds(JSContext *cx, uint32_t argc, jsval *vp)
{
    jsval *argv = JS_ARGV(cx, vp);
    JSBool ok = JS_TRUE;
    if (argc == 3) {
        std::vector<std::string> ids;
        ok &= sdkbox::jsval_to_std_vector_string(cx, args.get(0), &ids);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

        std::string title;
        ok &= sdkbox::jsval_to_std_string(cx, argv[1], &title);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

        std::string text;
        ok &= sdkbox::jsval_to_std_string(cx, argv[2], &text);
        JSB_PRECONDITION2(ok, cx, JS_FALSE, "Error processing arguments");

        sdkbox::PluginFacebook::inviteFriendsWithInviteIds(ids, title, text);

        JS_SET_RVAL(cx, vp, JS::NullValue());
        return JS_TRUE;
    }
    JS_ReportErrorUTF8(cx, "wrong number of arguments");
    return JS_FALSE;
}
#endif

#define REGISTE_FACEBOOK_FUNCTIONS \
JS_DefineFunction(cx, pluginObj, "setListener",                 js_PluginFacebookJS_PluginFacebook_setListener,                 1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "share",                       js_PluginFacebookJS_PluginFacebook_share,                       1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "api",                         js_PluginFacebookJS_PluginFacebook_api,                         4, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "dialog",                      js_PluginFacebookJS_PluginFacebook_dialog,                      1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "requestReadPermissions",      js_PluginFacebookJS_PluginFacebook_requestReadPermissions,      1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "requestPublishPermissions",   js_PluginFacebookJS_PluginFacebook_requestPublishPermissions,   1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "getPermissionList",           js_PluginFacebookJS_PluginFacebook_getPermissionList,           0, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "getFriends",                  js_PluginFacebookJS_PluginFacebook_getFriends,                  0, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "canPresentWithFBApp",         js_PluginFacebookJS_PluginFacebook_canPresentWithFBApp,         0, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "inviteFriends",               js_PluginFacebookJS_PluginFacebook_inviteFriends,               2, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "requestInvitableFriends",     js_PluginFacebookJS_PluginFacebook_requestInvitableFriends,     1, JSPROP_READONLY | JSPROP_PERMANENT); \
JS_DefineFunction(cx, pluginObj, "inviteFriendsWithInviteIds",  js_PluginFacebookJS_PluginFacebook_inviteFriendsWithInviteIds,  3, JSPROP_READONLY | JSPROP_PERMANENT);


#if defined(MOZJS_MAJOR_VERSION)
#if MOZJS_MAJOR_VERSION >= 33
void register_all_PluginFacebookJS_helper(JSContext* cx, JS::HandleObject global) {
    // Get the ns
    JS::RootedObject pluginObj(cx);
    sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginFacebook", &pluginObj);

    REGISTE_FACEBOOK_FUNCTIONS
}
#else
void register_all_PluginFacebookJS_helper(JSContext* cx, JSObject* obj) {
    // first, try to get the ns
    JS::RootedValue nsval(cx);
    JS::RootedObject pluginObj(cx);

    std::stringstream ss("sdkbox.PluginFacebook");
    std::string sub;
    const char* subChar;

    while(getline(ss, sub, '.')) {
        if(sub.empty())continue;

        subChar = sub.c_str();

        JS_GetProperty(cx, obj, subChar, &nsval);
        if (nsval == JS::NullValue()) {
            pluginObj = JS_NewObject(cx, NULL, NULL, NULL);
            nsval = OBJECT_TO_JSVAL(pluginObj);
            JS_SetProperty(cx, obj, subChar, nsval);
        } else {
            JS_ValueToObject(cx, nsval, &pluginObj);
        }
        obj = pluginObj;
    }

    REGISTE_FACEBOOK_FUNCTIONS
}
#endif
#elif defined(JS_VERSION)
void register_all_PluginFacebookJS_helper(JSContext* cx, JSObject* global) {
    jsval pluginVal;
    JSObject* pluginObj;
    pluginVal = sdkbox::getJsObjOrCreat(cx, global, "sdkbox.PluginFacebook", &pluginObj);

    REGISTE_FACEBOOK_FUNCTIONS
}
#endif

