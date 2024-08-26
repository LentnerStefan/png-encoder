import { NativeModules, Platform } from 'react-native';

const LINKING_ERROR =
  "The package 'react-native-png-encoder' doesn't seem to be linked. Make sure: \n\n" +
  Platform.select({ ios: "- You have run 'pod install'\n", default: '' }) +
  '- You rebuilt the app after installing the package\n' +
  '- You are not using Expo Go\n';

const PngEncoderModule =
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

const installResult = PngEncoderModule.install() as boolean;

if (!installResult) {
  console.error('PngEncoder bindings failed to install');
}
console.log('PngEncoder bindings installed!');

export type SaveRgbAsPngFunction = (
  buffer: ArrayBuffer,
  width: number,
  height: number
) => string;

//@ts-ignore
export const saveRgbAsPng: SaveRgbAsPngFunction = __saveRgbAsPng;

export type DeleteCacheFilesFunction = () => number;

//@ts-ignore
export const deleteCacheFiles: DeleteCacheFilesFunction = __deleteCacheFiles;
