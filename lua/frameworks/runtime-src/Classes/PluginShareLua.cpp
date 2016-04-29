#include "PluginShareLua.hpp"
#include "PluginShare/PluginShare.h"
#include "tolua_fix.h"
#include "SDKBoxLuaHelper.h"
#include "sdkbox/Sdkbox.h"



int lua_PluginShareLua_PluginShare_setSharePanelTitle(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginShare",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "sdkbox.PluginShare:setSharePanelTitle");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_PluginShareLua_PluginShare_setSharePanelTitle'", nullptr);
            return 0;
        }
        sdkbox::PluginShare::setSharePanelTitle(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginShare:setSharePanelTitle",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginShareLua_PluginShare_setSharePanelTitle'.",&tolua_err);
#endif
    return 0;
}
int lua_PluginShareLua_PluginShare_setSharePanelCancel(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginShare",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 1)
    {
        std::string arg0;
        ok &= luaval_to_std_string(tolua_S, 2,&arg0, "sdkbox.PluginShare:setSharePanelCancel");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_PluginShareLua_PluginShare_setSharePanelCancel'", nullptr);
            return 0;
        }
        sdkbox::PluginShare::setSharePanelCancel(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginShare:setSharePanelCancel",argc, 1);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginShareLua_PluginShare_setSharePanelCancel'.",&tolua_err);
#endif
    return 0;
}
int lua_PluginShareLua_PluginShare_init(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"sdkbox.PluginShare",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_PluginShareLua_PluginShare_init'", nullptr);
            return 0;
        }
        bool ret = sdkbox::PluginShare::init();
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    if (argc == 1)
    {
        const char* arg0;
        std::string arg0_tmp; ok &= luaval_to_std_string(tolua_S, 2, &arg0_tmp, "sdkbox.PluginShare:init"); arg0 = arg0_tmp.c_str();
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_PluginShareLua_PluginShare_init'", nullptr);
            return 0;
        }
        bool ret = sdkbox::PluginShare::init(arg0);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "sdkbox.PluginShare:init",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_PluginShareLua_PluginShare_init'.",&tolua_err);
#endif
    return 0;
}
static int lua_PluginShareLua_PluginShare_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (PluginShare)");
    return 0;
}

int lua_register_PluginShareLua_PluginShare(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"sdkbox.PluginShare");
    tolua_cclass(tolua_S,"PluginShare","sdkbox.PluginShare","",nullptr);

    tolua_beginmodule(tolua_S,"PluginShare");
        tolua_function(tolua_S,"setSharePanelTitle", lua_PluginShareLua_PluginShare_setSharePanelTitle);
        tolua_function(tolua_S,"setSharePanelCancel", lua_PluginShareLua_PluginShare_setSharePanelCancel);
        tolua_function(tolua_S,"init", lua_PluginShareLua_PluginShare_init);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(sdkbox::PluginShare).name();
    g_luaType[typeName] = "sdkbox.PluginShare";
    g_typeCast["PluginShare"] = "sdkbox.PluginShare";
    return 1;
}
TOLUA_API int register_all_PluginShareLua(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"sdkbox",0);
	tolua_beginmodule(tolua_S,"sdkbox");

	lua_register_PluginShareLua_PluginShare(tolua_S);

	tolua_endmodule(tolua_S);

	sdkbox::setProjectType("lua");
	return 1;
}

