#include <node.h>
#include <windows.h>

namespace addon {

using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Integer;
using v8::Value;

const enum RotationType {
  CW,
  CCW,
  FULL,
  NONE
};

DEVMODE GetDevMode() {
  DEVMODE dm;
  ZeroMemory(&dm, sizeof(dm));
  dm.dmSize = sizeof(dm);
  return dm;
}

int32_t GetRotationInteger(DWORD rotation) {
  switch (rotation) {
    case DMDO_DEFAULT:  return 0;
    case DMDO_90:       return 90;
    case DMDO_180:      return 180;
    case DMDO_270:      return 270;
    default:            return -1;
  }
}

DWORD CurrentRotation(DEVMODE dm) {
  if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) != 0) {
    return dm.dmDisplayOrientation;
  }
  return 0;
}

DWORD TranslateCW(DWORD from) {
  switch (from) {
    case DMDO_DEFAULT:  return DMDO_90;
    case DMDO_90:       return DMDO_180;
    case DMDO_180:      return DMDO_270;
    case DMDO_270:      return DMDO_DEFAULT;
    default:            return 0;
  }
}

DWORD TranslateCCW(DWORD from) {
  switch (from) {
    case DMDO_DEFAULT:  return DMDO_270;
    case DMDO_90:       return DMDO_DEFAULT;
    case DMDO_180:      return DMDO_90;
    case DMDO_270:      return DMDO_180;
    default:            return 0;
  }
}

DWORD Translate180(DWORD from) {
  switch (from) {
    case DMDO_DEFAULT:  return DMDO_180;
    case DMDO_90:       return DMDO_270;
    case DMDO_180:      return DMDO_DEFAULT;
    case DMDO_270:      return DMDO_90;
    default:            return 0;
  }
}

bool ChangeRotation(DEVMODE dm, DWORD rotation) {
  dm.dmDisplayOrientation = rotation;
  long lRet = ChangeDisplaySettings(&dm, 0);
  return lRet == DISP_CHANGE_SUCCESSFUL;
}

void GetRotation(const FunctionCallbackInfo<Value>& args) {
}

void RotateCW(const FunctionCallbackInfo<Value>& args) {
}

void RotateCCW(const FunctionCallbackInfo<Value>& args) {
}

void Rotate180(const FunctionCallbackInfo<Value>& args) {
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "getRotation", GetRotation);
  NODE_SET_METHOD(exports, "rotateCW", RotateCW);
  NODE_SET_METHOD(exports, "rotateCCW", RotateCCW);
  NODE_SET_METHOD(exports, "rotate180", Rotate180);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace addon
