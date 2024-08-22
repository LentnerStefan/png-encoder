package com.pngencoder;

import androidx.annotation.NonNull;

import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.module.annotations.ReactModule;
import com.facebook.react.bridge.ReactMethod;

import java.io.File;

@ReactModule(name = PngEncoderModule.NAME)
class PngEncoderModule extends ReactContextBaseJavaModule {
  public static final String NAME = "PngEncoder";
  private  native void initialize(long jsiPtr, String cachePath);

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
      // Get the cache directory path
      File cacheDir = context.getCacheDir();
      String cachePath = cacheDir.getAbsolutePath();

      initialize(
        context.getJavaScriptContextHolder().get(),
        cachePath
      );
      return true;
    } catch (Exception exception) {
      return false;
    }
  }
}
