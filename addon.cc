#define ERR_FAILED_TO_ENUMERATE     -1
#define ERR_DISP_CHANGE_BADDUALVIEW -10
#define ERR_DISP_CHANGE_BADFLAGS    -11
#define ERR_DISP_CHANGE_BADMODE     -12
#define ERR_DISP_CHANGE_BADPARAM    -13
#define ERR_DISP_CHANGE_FAILED      -14
#define ERR_DISP_CHANGE_NOTUPDATED  -15
#define ERR_DISP_CHANGE_RESTART     -16
#define ERR_DISP_CHANGE_UNKNOWN     -17
#define ERR_INVALID_ROTATION_RESULT -100

#include <node.h>
#include <windows.h>

namespace addon {

using v8::FunctionCallbackInfo;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::Object;
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

  if (dm.dmDisplayOrientation != originalOrientation) {
    long lRet = ChangeDisplaySettings(&dm, 0);
    if (lRet != DISP_CHANGE_SUCCESSFUL) {
      switch (lRet) {
        case DISP_CHANGE_BADDUALVIEW: return ERR_DISP_CHANGE_BADDUALVIEW;
        case DISP_CHANGE_BADFLAGS:    return ERR_DISP_CHANGE_BADFLAGS;
        case DISP_CHANGE_BADMODE:     return ERR_DISP_CHANGE_BADMODE;
        case DISP_CHANGE_BADPARAM:    return ERR_DISP_CHANGE_BADPARAM;
        case DISP_CHANGE_FAILED:      return ERR_DISP_CHANGE_FAILED;
        case DISP_CHANGE_NOTUPDATED:  return ERR_DISP_CHANGE_NOTUPDATED;
        case DISP_CHANGE_RESTART:     return ERR_DISP_CHANGE_RESTART;
        default:                      return ERR_DISP_CHANGE_UNKNOWN;
      }
    }
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
