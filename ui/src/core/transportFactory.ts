import { Transport } from "./transport";
import { MockTransport } from "./mockTransport";

// Use mock transport in development when window API is not available
export function createTransport() {
  const useMock = import.meta.env.MODE === 'development' && !(window as any).GetKanjis;

  if (useMock) {
    console.log("Using MockTransport (development mode without backend)");
    return new MockTransport();
  }

  console.log("Using real Transport");
  return new Transport();
}
