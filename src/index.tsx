declare function multiply(a: number, b: number): Promise<number>;

export function multiplyA(a: number, b: number): Promise<number> {
  return multiply(a, b);
}
