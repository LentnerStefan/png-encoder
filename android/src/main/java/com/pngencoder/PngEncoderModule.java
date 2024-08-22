package com.pngencoder;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.JavaScriptContextHolder;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.ReactMethod;

@ReactModule(name = PngEncoderModule.NAME)
class PngEncoderModule extends ReactContextBaseJavaModule {
  public static final String NAME = "PngEncoder";
  private  native void initialize(long jsiPtr);

  public PngEncoderModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @NonNull
  @Override
  public String getName() {
    return "PngEncoder";
  }

  @ReactMethod(isBlockingSynchronousMethod = true)
  public boolean install() {
    try {
      System.loadLibrary("pngencoder");
      ReactApplicationContext context = getReactApplicationContext();
      initialize(
        context.getJavaScriptContextHolder().get()
      );
      return true;
    } catch (Exception exception) {
      return false;
    }
  }
}
