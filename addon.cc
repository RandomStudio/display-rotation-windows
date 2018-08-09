#include <node.h>

namespace addon {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;

void Initialize(Local<Object> exports) {
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace addon
