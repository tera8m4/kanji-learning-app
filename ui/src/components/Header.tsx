import { useNavigate } from "react-router-dom";

export default function Header() {
  const navigate = useNavigate();
  return (
    <header className="header">
      <h1 className="title">漢字練習</h1>
      <p className="subtitle">Kanji Practice</p>
      <button className="header-nav-button" onClick={() => navigate("/list")}>
        Kanji List
      </button>
    </header>
  );
}
