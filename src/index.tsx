import { PngEncoderModule } from './PngEncoderModule';

declare global {
  //@ts-ignore
  var encode: (buffer: ArrayBuffer, width: number, height: number) => string;
}

console.log('PngEncoder bindings installing ...');
const result = PngEncoderModule.install() as boolean;

if (!result) {
  console.error('PngEncoder bindings failed to install');
}

console.log('PngEncoder bindings installed');

export type Encode = (
  buffer: ArrayBuffer,
  width: number,
  height: number
) => string;

export type UsePngEncoderType = () => {
  encode: Encode;
};

export const usePngEncoder: UsePngEncoderType = () => {
  return { encode };
};
