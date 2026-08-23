import { Transport } from "./transport";
import type {
  KanjiData,
  KanjiListEntry,
  RequestAnswers,
  ResponseReviews,
} from "./transport";

// A small hand-written deck. Meanings use the same comma-separated format the
// real API returns, so alternative answers are exercised too.
const MOCK_KANJI: KanjiData[] = [
  {
    id: 1,
    kanji: "日",
    meaning: "sun, day",
    examples: [
      { word: "日本", reading: "にほん" },
      { word: "毎日", reading: "まいにち" },
    ],
  },
  {
    id: 2,
    kanji: "人",
    meaning: "person",
    examples: [
      { word: "人口", reading: "じんこう" },
      { word: "one person", reading: "ひとり" },
    ],
  },
  {
    id: 3,
    kanji: "水",
    meaning: "water",
    examples: [
      { word: "水曜日", reading: "すいようび" },
      { word: "水着", reading: "みずぎ" },
    ],
  },
  {
    id: 4,
    kanji: "山",
    meaning: "mountain",
    examples: [
      { word: "火山", reading: "かざん" },
      { word: "山道", reading: "やまみち" },
    ],
  },
];

const LATENCY_MS = 150;

function delay<T>(value: T): Promise<T> {
  return new Promise((resolve) => setTimeout(() => resolve(value), LATENCY_MS));
}

/**
 * In-memory Transport used when VITE_USE_MOCK is set. Reports auth as not
 * required, so useAuth marks the session authenticated and the login screen is
 * skipped entirely.
 */
export class MockTransport extends Transport {
  // Batches still pending review. Each getReviews() call serves one batch.
  private remaining: KanjiData[][] = [MOCK_KANJI.slice(0, 2), MOCK_KANJI.slice(2)];

  public override async getAuthConfig(): Promise<{ auth_required: boolean }> {
    return delay({ auth_required: false });
  }

  public override async login(): Promise<void> {
    return delay(undefined);
  }

  public override async loginWithPassword(): Promise<void> {
    return delay(undefined);
  }

  public override async getReviews(): Promise<ResponseReviews> {
    const batch = this.remaining[0] ?? [];
    const total = this.remaining.reduce((sum, b) => sum + b.length, 0);
    return delay({ kanjis: batch, total });
  }

  public override async sendAnswers(req: RequestAnswers): Promise<void> {
    console.info("[mock] sendAnswers", req.answers);
    // The batch just reviewed is done; the next getReviews() serves the next one.
    this.remaining = this.remaining.slice(1);
    return delay(undefined);
  }

  public override async learnMoreKanjis(): Promise<void> {
    // Re-queue the full deck so "learn more" always has something to show.
    this.remaining = [...this.remaining, MOCK_KANJI];
    return delay(undefined);
  }

  public override async getKanjiList(): Promise<KanjiListEntry[]> {
    const now = Math.floor(Date.now() / 1000);
    return delay(
      MOCK_KANJI.map((k, i) => ({
        id: k.id,
        kanji: k.kanji,
        meaning: k.meaning,
        level: (i % 5) + 1,
        next_review_date: now + i * 86400,
      })),
    );
  }
}
