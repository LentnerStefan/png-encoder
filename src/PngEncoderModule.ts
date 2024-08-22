import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  "The package 'react-native-png-encoder' doesn't seem to be linked. Make sure: \n\n" +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

const PngEncoder =
  NativeModules.PngEncoder != null
    ? NativeModules.PngEncoder
    : new Proxy(
        {},
        {
          get() {
            throw new Error(LINKING_ERROR);
          },
        }
      );

export const PngEncoderModule = PngEncoder;
