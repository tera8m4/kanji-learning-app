import { Transport } from "./transport";
import { MockTransport } from "./mockTransport";

export function createTransport() {
  if (import.meta.env.VITE_USE_MOCK === "true") {
    console.info("[mock] using in-memory transport (VITE_USE_MOCK=true)");
    return new MockTransport();
  }
  return new Transport();
}
