import { BrowserRouter, Routes, Route, Navigate } from "react-router-dom";
import "./App.css";
import LoginScreen from "./LoginScreen";
import KanjiListScreen from "./KanjiListScreen";
import ReviewScreen from "./ReviewScreen";
import { useAuth } from "../hooks/useAuth";
import type { Transport } from "../core/transport";

interface AppProps {
  transport: Transport;
}

export default function App({ transport }: AppProps) {
  const { isAuthenticated, handleTelegramLogin } = useAuth(transport);

  if (!isAuthenticated) {
    return <LoginScreen onLogin={handleTelegramLogin} />;
  }

  return (
    <BrowserRouter>
      <Routes>
        <Route path="/" element={<ReviewScreen transport={transport} />} />
        <Route path="/list" element={<KanjiListScreen transport={transport} />} />
        <Route path="*" element={<Navigate to="/" replace />} />
      </Routes>
    </BrowserRouter>
  );
}
