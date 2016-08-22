/*
   Copyright 2016 Nidium Inc. All rights reserved.
   Use of this source code is governed by a MIT license
   that can be found in the LICENSE file.
*/

#include "Binding/JSEvents.h"
#include "Binding/JSExposer.h"

namespace Nidium {
namespace Binding {

// {{{ Preamble
static const JSClass JSEvent_class = { "NidiumEvent",
                                       0,
                                       JS_PropertyStub,
                                       JS_DeletePropertyStub,
                                       JS_PropertyStub,
                                       JS_StrictPropertyStub,
                                       JS_EnumerateStub,
                                       JS_ResolveStub,
                                       JS_ConvertStub,
                                       NULL,
                                       nullptr,
                                       nullptr,
                                       nullptr,
                                       nullptr,
                                       JSCLASS_NO_INTERNAL_MEMBERS };

static const JSClass JSErrorEvent_class = { "NidiumErrorEvent",
                                            0,
                                            JS_PropertyStub,
                                            JS_DeletePropertyStub,
                                            JS_PropertyStub,
                                            JS_StrictPropertyStub,
                                            JS_EnumerateStub,
                                            JS_ResolveStub,
                                            JS_ConvertStub,
                                            NULL,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            nullptr,
                                            JSCLASS_NO_INTERNAL_MEMBERS };

static bool
Nidium_jsevents_stopPropagation(JSContext *cx, unsigned argc, JS::Value *vp)
{
    JS::RootedObject thisobj(cx, JS_THIS_OBJECT(cx, vp));
    if (!thisobj) {
        JS_ReportError(cx, "Illegal invocation");
        return false;
    }
    if (!JS_InstanceOf(cx, thisobj, &JSEvent_class, NULL)) {
        JS_ReportError(cx, "Illegal invocation");
        return false;
    }
    JS::RootedValue cancelBubble(cx, JS::BooleanValue(true));
    JS_SetProperty(cx, thisobj, "cancelBubble", cancelBubble);

    return true;
}

static bool Nidium_jsevents_stub(JSContext *cx, unsigned argc, JS::Value *vp)
{
    return true;
}

static JSFunctionSpec JSEvents_funcs[]
    = { JS_FN("stopPropagation",
              Nidium_jsevents_stopPropagation,
              0,
              JSPROP_ENUMERATE | JSPROP_PERMANENT /*| JSPROP_READONLY*/),
        JS_FN("preventDefault",
              Nidium_jsevents_stub,
              0,
              JSPROP_ENUMERATE | JSPROP_PERMANENT /*| JSPROP_READONLY*/),
        JS_FN("forcePropagation",
              Nidium_jsevents_stub,
              0,
              JSPROP_ENUMERATE | JSPROP_PERMANENT /*| JSPROP_READONLY*/),
        JS_FS_END };
// }}}

// {{{ JSEvents
JSObject *JSEvents::CreateEventObject(JSContext *cx)
{
    JS::RootedObject ret(
        cx, JS_NewObject(cx, &JSEvent_class, JS::NullPtr(), JS::NullPtr()));
    JS_DefineFunctions(cx, ret, JSEvents_funcs);

    return ret;
}

JSObject *
JSEvents::CreateErrorEventObject(JSContext *cx, int code, const char *err)
{
    JS::RootedObject ret(cx, JS_NewObject(cx, &JSErrorEvent_class,
                                          JS::NullPtr(), JS::NullPtr()));
    JS_DefineFunctions(cx, ret, JSEvents_funcs);

    JSContext *m_Cx = cx;

    NIDIUM_JSOBJ_SET_PROP_INT(ret, "errorCode", code);
    NIDIUM_JSOBJ_SET_PROP_CSTR(ret, "error", err);

    return ret;
}
// }}}

} // namespace Binding
} // namespace Nidium
