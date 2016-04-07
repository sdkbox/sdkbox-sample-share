#ifndef __PluginShareJS_h__
#define __PluginShareJS_h__

#include "jsapi.h"
#include "jsfriendapi.h"


extern JSClass  *jsb_sdkbox_PluginShare_class;
extern JSObject *jsb_sdkbox_PluginShare_prototype;

#if MOZJS_MAJOR_VERSION >= 33
void js_register_PluginShareJS_PluginShare(JSContext *cx, JS::HandleObject global);
void register_all_PluginShareJS(JSContext* cx, JS::HandleObject obj);
#else
void js_register_PluginShareJS_PluginShare(JSContext *cx, JSObject* global);
void register_all_PluginShareJS(JSContext* cx, JSObject* obj);
#endif

bool js_PluginShareJS_PluginShare_constructor(JSContext *cx, uint32_t argc, jsval *vp);
void js_PluginShareJS_PluginShare_finalize(JSContext *cx, JSObject *obj);
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginShareJS_PluginShare_setSharePanelTitle(JSContext *cx, uint32_t argc, jsval *vp);
#elif defined(JS_VERSION)
JSBool js_PluginShareJS_PluginShare_setSharePanelTitle(JSContext *cx, uint32_t argc, jsval *vp);
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginShareJS_PluginShare_setSharePanelCancel(JSContext *cx, uint32_t argc, jsval *vp);
#elif defined(JS_VERSION)
JSBool js_PluginShareJS_PluginShare_setSharePanelCancel(JSContext *cx, uint32_t argc, jsval *vp);
#endif
#if defined(MOZJS_MAJOR_VERSION)
bool js_PluginShareJS_PluginShare_init(JSContext *cx, uint32_t argc, jsval *vp);
#elif defined(JS_VERSION)
JSBool js_PluginShareJS_PluginShare_init(JSContext *cx, uint32_t argc, jsval *vp);
#endif
#endif

