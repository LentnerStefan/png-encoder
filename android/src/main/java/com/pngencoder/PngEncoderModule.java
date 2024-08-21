package com.pngencoder;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;

class PngEncoderModule extends ReactContextBaseJavaModule {
  static {
    System.loadLibrary("pngencoder");
  }

  private static native void initialize(long jsiPtr);
  private static native void destruct();

  public PngEncoderModule(ReactApplicationContext reactContext) {
    super(reactContext);
  }

  @NonNull
  @Override
  public String getName() {
    return "PngEncoder";
  }


  @NonNull
  @Override
  public void initialize() {
    super.initialize();

    PngEncoderModule.initialize(
      this.getReactApplicationContext().getJavaScriptContextHolder().get()
    );
  }

  @Override
  public void onCatalystInstanceDestroy() {
    PngEncoderModule.destruct();
  }
}
