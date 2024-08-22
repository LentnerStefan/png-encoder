import { NativeModules } from 'react-native';

declare global {
  const multiply: (
    buffer: ArrayBuffer,
    width: number,
    height: number
  ) => ArrayBuffer;
}

console.log('PngEncoder bindings installing ...');
const result = NativeModules.PngEncoder.install() as boolean;

if (!result) {
  console.error('PngEncoder bindings failed to install');
}

console.log('PngEncoder bindings installed');

export const usePngEncoder = () => {
  return { multiply };
};
