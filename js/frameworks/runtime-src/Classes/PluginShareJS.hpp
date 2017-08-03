#ifndef __PluginShareJS_h__
#define __PluginShareJS_h__

#include "jsapi.h"
#include "jsfriendapi.h"


#if MOZJS_MAJOR_VERSION >= 33
void js_register_PluginShareJS_PluginShare(JSContext *cx, JS::HandleObject global);
void register_all_PluginShareJS(JSContext* cx, JS::HandleObject obj);
#else
void js_register_PluginShareJS_PluginShare(JSContext *cx, JSObject* global);
void register_all_PluginShareJS(JSContext* cx, JSObject* obj);
#endif
#endif

