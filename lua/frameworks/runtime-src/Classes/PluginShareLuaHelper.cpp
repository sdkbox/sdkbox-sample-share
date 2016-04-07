
#include "PluginShareLuaHelper.h"
#include "PluginShare/PluginShare.h"
#include "CCLuaEngine.h"
#include "tolua_fix.h"
#include "SDKBoxLuaHelper.h"

class ShareListenerLua : public sdkbox::ShareListener {
public:
    ShareListenerLua(): mLuaHandler(0) {
    }

    ~ShareListenerLua() {
        resetHandler();
    }

    void setHandler(int luaHandler) {
        if (mLuaHandler == luaHandler) {
            return;
        }
        resetHandler();
        mLuaHandler = luaHandler;
    }

    void resetHandler() {
        if (0 == mLuaHandler) {
            return;
        }

        LUAENGINE->removeScriptHandler(mLuaHandler);
        mLuaHandler = 0;
    }

    void onShareState(const sdkbox::SocialShareResponse& response) {
        LuaStack* stack = LUAENGINE->getLuaStack();

        LuaValueDict paramsDict;
        paramsDict.insert(std::make_pair("state", LuaValue::intValue(response.state)));
        paramsDict.insert(std::make_pair("error", LuaValue::stringValue(response.error)));
        paramsDict.insert(std::make_pair("platform", LuaValue::intValue(response.platform)));

        LuaValueDict dict;

        dict.insert(std::make_pair("event", LuaValue::stringValue("onShareState")));
        dict.insert(std::make_pair("response", LuaValue::dictValue(paramsDict)));

        stack->pushLuaValueDict(dict);
        stack->executeFunctionByHandler(mLuaHandler, 1);
    }

private:
    int mLuaHandler;
};

int lua_PluginShareLua_PluginShare_setListener(lua_State* tolua_S) {
    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginShare",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
#if COCOS2D_DEBUG >= 1
        if (!toluafix_isfunction(tolua_S, 2 , "LUA_FUNCTION",0,&tolua_err))
        {
            goto tolua_lerror;
        }
#endif
        LUA_FUNCTION handler = (  toluafix_ref_function(tolua_S,2,0));
        ShareListenerLua* lis = static_cast<ShareListenerLua*> (sdkbox::PluginShare::getListener());
        if (NULL == lis) {
            lis = new ShareListenerLua();
        }
        lis->setHandler(handler);
        sdkbox::PluginShare::setListener(lis);

        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginShare::setListener",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginShareLua_PluginShare_setListener'.",&tolua_err);
#endif
    return 0;
}

bool luaval_to_shareInfo(lua_State* L, int lo, sdkbox::SocialShareInfo* ret, const char* funcName) {
    if (nullptr == L || nullptr == ret || lua_gettop(L) < lo)
        return false;

    tolua_Error tolua_err;
    bool ok = true;
    if (!tolua_istable(L, lo, 0, &tolua_err)) {
        ok = false;
    }

    if (!ok)
        return ok;

    lua_pushnil(L);
    std::string key;
    std::string value;
    int intValue;
    while (lua_next(L, lo) != 0) {
        if (lua_isstring(L, -2)) {
            if (luaval_to_std_string(L, -2, &key)) {
                if (0 == key.compare("text")) {
                    if (luaval_to_std_string(L, -1, &value)) { ret->text = value; }
                } else if (0 == key.compare("title")) {
                    if (luaval_to_std_string(L, -1, &value)) { ret->title = value; }
                } else if (0 == key.compare("image")) {
                    if (luaval_to_std_string(L, -1, &value)) { ret->image = value; }
                } else if (0 == key.compare("link")) {
                    if (luaval_to_std_string(L, -1, &value)) { ret->link = value; }
                } else if (0 == key.compare("platform")) {
                    if (luaval_to_int32(L, -1, &intValue)) { ret->platform = (sdkbox::SocialPlatform)intValue; }
                }
            }
        } else {
            //CCASSERT(false, "string type is needed");
        }

        lua_pop(L, 1);
    }

    return ok;
}

int lua_PluginShareLua_PluginShare_share(lua_State* tolua_S) {
    int argc = 0;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginShare",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1) {
#if COCOS2D_DEBUG >= 1
        if (!tolua_istable(tolua_S, 2, 0, &tolua_err)) {
            goto tolua_lerror;
        }
#endif
        sdkbox::SocialShareInfo shareInfo;
        if (!luaval_to_shareInfo(tolua_S, 2, &shareInfo, "sdkbox.PluginShare:share")) {
            return 0;
        }
        sdkbox::PluginShare::share(shareInfo);

        return 0;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginShare::share",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginShareLua_PluginShare_share'.",&tolua_err);
#endif
    return 0;
}

// int lua_PluginShareLua_PluginShare_shareDialog(lua_State* tolua_S) {
//     int argc = 0;

// #if COCOS2D_DEBUG >= 1
//     tolua_Error tolua_err;
// #endif

// #if COCOS2D_DEBUG >= 1
//     if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginShare",0,&tolua_err)) goto tolua_lerror;
// #endif

//     argc = lua_gettop(tolua_S) - 1;

//     if (argc == 1) {
// #if COCOS2D_DEBUG >= 1
//         if (!tolua_istable(tolua_S, 2, 0, &tolua_err)) {
//             goto tolua_lerror;
//         }
// #endif
//         sdkbox::PluginShare::ShareInfo shareInfo;
//         if (!luaval_to_shareInfo(tolua_S, 2, &shareInfo, "sdkbox.PluginShare:shareDialog")) {
//             return 0;
//         }
//         sdkbox::PluginShare::shareDialog(shareInfo);

//         return 0;
//     }
//     luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginShare::shareDialog",argc, 1);
//     return 0;
// #if COCOS2D_DEBUG >= 1
// tolua_lerror:
//     tolua_error(tolua_S,"#ferror in function 'lua_PluginShareLua_PluginShare_shareDialog'.",&tolua_err);
// #endif
//     return 0;
// }

static int lua_createTable(lua_State* L, const std::map<std::string, int>& map) {
    lua_newtable(L);

    std::map<std::string, int>::const_iterator it = map.begin();
    while (it != map.end()) {
        lua_pushstring(L, it->first.c_str());
        lua_pushinteger(L, it->second);
        lua_settable(L, -3);
        it++;
    }

    return 1;
}

static int lua_setTable(lua_State* L, int table, const std::string& name, const std::map<std::string, int>& map) {
    if (table < 0) {
        table = lua_gettop(L) + table + 1;
    }
    lua_pushstring(L, name.c_str());
    lua_createTable(L, map);
    lua_rawset(L, table);

    return 0;
}

int lua_PluginShareLua_constants(lua_State* L) {
    if (NULL == L) {
        return 0;
    }

    std::map<std::string, int> enums;
    enums.clear();
    enums.insert(std::make_pair("SocialShareStateNone", 0));
    enums.insert(std::make_pair("SocialShareStateUnkonw", 1));
    enums.insert(std::make_pair("SocialShareStateBegin", 2));
    enums.insert(std::make_pair("SocialShareStateSuccess", 3));
    enums.insert(std::make_pair("SocialShareStateFail", 4));
    enums.insert(std::make_pair("SocialShareStateCancelled", 5));
    enums.insert(std::make_pair("SocialShareStateSelectShow", 3));
    enums.insert(std::make_pair("SocialShareStateSelected", 4));
    enums.insert(std::make_pair("SocialShareStateSelectCancelled", 5));
    lua_setTable(L, -1, "SocialShareState", enums);
    
    enums.clear();
    enums.insert(std::make_pair("Platform_Unknow", 0));
    enums.insert(std::make_pair("Platform_Twitter", 1));
    enums.insert(std::make_pair("Platform_Facebook", 2));
    enums.insert(std::make_pair("Platform_Select", 3));
    enums.insert(std::make_pair("Platform_All", 4));
    lua_setTable(L, -1, "SocialPlatform", enums);

    return 1;
}

int extern_PluginShare(lua_State* L) {
    if (NULL == L) {
        return 0;
    }

    lua_pushstring(L, "sdkbox.PluginShare");
    lua_rawget(L, LUA_REGISTRYINDEX);
    if (lua_istable(L,-1)) {
        tolua_function(L,"setListener", lua_PluginShareLua_PluginShare_setListener);
        tolua_function(L,"share", lua_PluginShareLua_PluginShare_share);
        // tolua_function(L,"shareDialog", lua_PluginShareLua_PluginShare_shareDialog);
    }
     lua_pop(L, 1);

    lua_PluginShareLua_constants(L);

    return 1;
}

TOLUA_API int register_all_PluginShareLua_helper(lua_State* L) {
    tolua_module(L,"sdkbox",0);
    tolua_beginmodule(L,"sdkbox");

    extern_PluginShare(L);

    tolua_endmodule(L);
    return 1;
}


