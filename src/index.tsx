declare function multiply(array: ArrayBuffer): Promise<number>;

export function multiplyA(array: Uint8Array): Promise<number> {
  return multiply(array.buffer);
}
