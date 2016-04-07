

#ifndef __js__PluginShareJSHelper_H_
#define __js__PluginShareJSHelper_H_

#include "jsapi.h"
#include "jsfriendapi.h"

#if MOZJS_MAJOR_VERSION >= 31
void register_all_PluginShareJS_helper(JSContext* cx, JS::HandleObject global);
#else
void register_all_PluginShareJS_helper(JSContext* cx, JSObject* global);
#endif

#endif /* defined(__PluginShareJSHelper_H_) */
