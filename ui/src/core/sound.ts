// Safari exposes the constructor under a vendor prefix.
declare global {
  interface Window {
    webkitAudioContext?: typeof AudioContext;
  }
}

let audioContext: AudioContext | null = null;

// Lazily create the context: browsers refuse to start one before a user gesture.
function getContext(): AudioContext | null {
  if (typeof window === "undefined") return null;
  if (!audioContext) {
    const Ctor = window.AudioContext ?? window.webkitAudioContext;
    if (!Ctor) return null;
    audioContext = new Ctor();
  }
  if (audioContext.state === "suspended") {
    void audioContext.resume();
  }
  return audioContext;
}

type Note = {
  frequency: number;
  startAt: number;
  duration: number;
};

// Shared voice: quick attack, exponential decay - a soft bell rather than a beep.
function playNotes(notes: Note[], type: OscillatorType, peakGain: number) {
  const ctx = getContext();
  if (!ctx) return;

  for (const note of notes) {
    const oscillator = ctx.createOscillator();
    const gain = ctx.createGain();

    oscillator.type = type;
    oscillator.frequency.value = note.frequency;

    const start = ctx.currentTime + note.startAt;
    const end = start + note.duration;

    gain.gain.setValueAtTime(0.0001, start);
    gain.gain.exponentialRampToValueAtTime(peakGain, start + 0.012);
    gain.gain.exponentialRampToValueAtTime(0.0001, end);

    oscillator.connect(gain);
    gain.connect(ctx.destination);

    oscillator.start(start);
    oscillator.stop(end);
  }
}

// A cheerful rising two-note chime (E6 -> B6), synthesized on the fly so the
// single-file build stays free of binary audio assets.
export function playCorrectSound() {
  playNotes(
    [
      { frequency: 1318.51, startAt: 0, duration: 0.16 },
      { frequency: 1975.53, startAt: 0.09, duration: 0.26 },
    ],
    "triangle",
    0.18,
  );
}

// The counterpart: a gentle falling two-note figure (G4 -> C#4). Deliberately
// low, soft and short - a nudge rather than a buzzer, since getting an answer
// wrong is already signalled by the shake and the revealed answer.
export function playIncorrectSound() {
  playNotes(
    [
      { frequency: 392.0, startAt: 0, duration: 0.14 },
      { frequency: 277.18, startAt: 0.1, duration: 0.28 },
    ],
    "sine",
    0.12,
  );
}
