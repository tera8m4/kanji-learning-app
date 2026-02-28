import { useEffect, useState } from "react";
import { useNavigate } from "react-router-dom";
import type { Transport, KanjiListEntry } from "../core/transport";
import "./KanjiListScreen.css";

const PAGE_SIZE = 20;

type KanjiListScreenProps = {
  transport: Transport;
};


function levelLabel(level: number): string {
  const labels = ["Apprentice 1", "Apprentice 2", "Apprentice 3", "Apprentice 4", "Guru 1", "Guru 2", "Master", "Enlightened", "Burned"];
  return labels[level] ?? `Lv${level}`;
}

function levelClass(level: number): string {
  if (level <= 3) return "apprentice";
  if (level <= 5) return "guru";
  if (level === 6) return "master";
  if (level === 7) return "enlightened";
  return "burned";
}

export default function KanjiListScreen({ transport }: KanjiListScreenProps) {
  const navigate = useNavigate();
  const [kanjis, setKanjis] = useState<KanjiListEntry[]>([]);
  const [isLoading, setIsLoading] = useState(true);
  const [page, setPage] = useState(0);

  useEffect(() => {
    transport.getKanjiList().then((data) => {
      setKanjis(data);
      setIsLoading(false);
    });
  }, [transport]);

  const totalPages = Math.max(1, Math.ceil(kanjis.length / PAGE_SIZE));
  const pageKanjis = kanjis.slice(page * PAGE_SIZE, (page + 1) * PAGE_SIZE);

  if (isLoading) {
    return (
      <div className="container">
        <header className="header">
          <h1 className="title">読み込み中...</h1>
        </header>
      </div>
    );
  }

  return (
    <div className="container">
      <div className="ink-splatter-1"></div>
      <div className="ink-splatter-2"></div>

      <header className="header">
        <h1 className="title">漢字一覧</h1>
        <p className="subtitle">All Kanji ({kanjis.length})</p>
      </header>

      <button className="back-button" onClick={() => navigate("/")}>
        Back to Review
      </button>

      <div className="kanji-grid">
        {pageKanjis.map((k) => (
          <div key={k.id} className="kanji-grid-item">
            <span className="kanji-grid-char">{k.kanji}</span>
            <span className="kanji-grid-meaning">{k.meaning}</span>
            <span className={`kanji-grid-level level-${levelClass(k.level)}`}>
              {levelLabel(k.level)}
            </span>
          </div>
        ))}
        {kanjis.length === 0 && (
          <p className="kanji-grid-empty">No kanji yet.</p>
        )}
      </div>

      {totalPages > 1 && (
        <div className="pagination">
          <button
            className="pagination-button"
            disabled={page === 0}
            onClick={() => { setPage(page - 1); window.scrollTo({ top: 0 }); }}
          >
            Prev
          </button>
          <span className="pagination-info">{page + 1} / {totalPages}</span>
          <button
            className="pagination-button"
            disabled={page >= totalPages - 1}
            onClick={() => { setPage(page + 1); window.scrollTo({ top: 0 }); }}
          >
            Next
          </button>
        </div>
      )}
    </div>
  );
}
