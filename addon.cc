#define ERR_FAILED_TO_ENUMERATE -1
#define ERR_FAILED_TO_ROTATE -2
#define ERR_INVALID_ROTATION_RESULT -100

#include <iostream>
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

// PRIVATE FUNCTIONS

void SwapWidthHeight(DEVMODE &dm) {
  DWORD dwTemp = dm.dmPelsHeight;
  dm.dmPelsHeight= dm.dmPelsWidth;
  dm.dmPelsWidth = dwTemp;
}

void ChangeOrientationCW(DEVMODE &dm) {
  switch (dm.dmDisplayOrientation) {
    case DMDO_DEFAULT:  dm.dmDisplayOrientation = DMDO_270;     return;
    case DMDO_90:       dm.dmDisplayOrientation = DMDO_DEFAULT; return;
    case DMDO_180:      dm.dmDisplayOrientation = DMDO_90;      return;
    case DMDO_270:      dm.dmDisplayOrientation = DMDO_180;     return;
  }
}

void ChangeOrientationCCW(DEVMODE &dm) {
  switch (dm.dmDisplayOrientation) {
    case DMDO_DEFAULT:  dm.dmDisplayOrientation = DMDO_90;      return;
    case DMDO_90:       dm.dmDisplayOrientation = DMDO_180;     return;
    case DMDO_180:      dm.dmDisplayOrientation = DMDO_270;     return;
    case DMDO_270:      dm.dmDisplayOrientation = DMDO_DEFAULT; return;
  }
}

void ChangeOrientation180(DEVMODE &dm) {
  switch (dm.dmDisplayOrientation) {
    case DMDO_DEFAULT:  dm.dmDisplayOrientation = DMDO_180;     return;
    case DMDO_90:       dm.dmDisplayOrientation = DMDO_270;     return;
    case DMDO_180:      dm.dmDisplayOrientation = DMDO_DEFAULT; return;
    case DMDO_270:      dm.dmDisplayOrientation = DMDO_90;      return;
  }
}

bool UpdateDisplaySettings(DEVMODE &dm) {
  long lRet = ChangeDisplaySettings(&dm, 0);
  switch (lRet) {
    case DISP_CHANGE_BADDUALVIEW:
      std::cerr << "The settings change was unsuccessful because the system is DualView capable." << std::endl;
      break;
    case DISP_CHANGE_BADFLAGS:
      std::cerr << "An invalid set of flags was passed in." << std::endl;
      break;
    case DISP_CHANGE_BADMODE:
      std::cerr << "The graphics mode is not supported." << std::endl;
      break;
    case DISP_CHANGE_BADPARAM:
      std::cerr << "An invalid parameter was passed in. This can include an invalid flag or combination of flags." << std::endl;
      break;
    case DISP_CHANGE_FAILED:
      std::cerr << "The display driver failed the specified graphics mode." << std::endl;
      break;
    case DISP_CHANGE_NOTUPDATED:
      std::cerr << "Unable to write settings to the registry." << std::endl;
      break;
    case DISP_CHANGE_RESTART:
      std::cerr << "The computer must be restarted for the graphics mode to work." << std::endl;
      break;
  }
  return lRet == DISP_CHANGE_SUCCESSFUL;
}

int32_t GetRotationResult(RotationType type) {
  DEVMODE dm;
  ZeroMemory(&dm, sizeof(dm));
  dm.dmSize = sizeof(dm);

  if (EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm) == 0) {
    return ERR_FAILED_TO_ENUMERATE;
  }

  DWORD originalOrientation = dm.dmDisplayOrientation;

  switch (type) {
    case CW:
      SwapWidthHeight(dm);
      ChangeOrientationCW(dm);
      break;
    case CCW:
      SwapWidthHeight(dm);
      ChangeOrientationCCW(dm);
      break;
    case FULL:
      ChangeOrientation180(dm);
      break;
  }

  if (dm.dmDisplayOrientation != originalOrientation && !UpdateDisplaySettings(dm)) {
    std::cerr << "Failed to rotate display" << std::endl;
    return ERR_FAILED_TO_ROTATE;
  }

  switch (dm.dmDisplayOrientation) {
    case DMDO_DEFAULT:  return 0;
    case DMDO_90:       return 90;
    case DMDO_180:      return 180;
    case DMDO_270:      return 270;
  }

  return ERR_INVALID_ROTATION_RESULT;
}

// EXPOSED FUNCTIONS

void GetRotation(const FunctionCallbackInfo<Value>& args) {
  return args.GetReturnValue().Set(Integer::New(args.GetIsolate(), GetRotationResult(NONE)));
}

void RotateCW(const FunctionCallbackInfo<Value>& args) {
  return args.GetReturnValue().Set(Integer::New(args.GetIsolate(), GetRotationResult(CW)));
}

void RotateCCW(const FunctionCallbackInfo<Value>& args) {
  return args.GetReturnValue().Set(Integer::New(args.GetIsolate(), GetRotationResult(CCW)));
}

void Rotate180(const FunctionCallbackInfo<Value>& args) {
  return args.GetReturnValue().Set(Integer::New(args.GetIsolate(), GetRotationResult(FULL)));
}

// INITIALIZE

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "getRotation", GetRotation);
  NODE_SET_METHOD(exports, "rotateCW", RotateCW);
  NODE_SET_METHOD(exports, "rotateCCW", RotateCCW);
  NODE_SET_METHOD(exports, "rotate180", Rotate180);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}  // namespace addon
