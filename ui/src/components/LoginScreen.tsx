import { useEffect, useRef, useState } from "react";
import type { TelegramAuthPayload, PasswordAuthPayload } from "../core/transport";
import "./LoginScreen.css";

type AuthMode = "telegram" | "password";

interface LoginScreenProps {
  onTelegramLogin: (data: TelegramAuthPayload) => void;
  onPasswordLogin: (creds: PasswordAuthPayload) => Promise<void>;
}

export default function LoginScreen({ onTelegramLogin, onPasswordLogin }: LoginScreenProps) {
  const [mode, setMode] = useState<AuthMode>("telegram");

  return (
    <div className="login-container">
      <div className="ink-splatter-1"></div>
      <div className="ink-splatter-2"></div>

      <div className="login-card">
        <div className="login-header">
          <h1 className="login-title">漢字</h1>
          <p className="login-subtitle">Review</p>
        </div>

        <div className="login-divider"></div>

        <div className="auth-tabs" role="tablist">
          <button
            role="tab"
            aria-selected={mode === "telegram"}
            className={`auth-tab ${mode === "telegram" ? "auth-tab-active" : ""}`}
            onClick={() => setMode("telegram")}
          >
            Telegram
          </button>
          <button
            role="tab"
            aria-selected={mode === "password"}
            className={`auth-tab ${mode === "password" ? "auth-tab-active" : ""}`}
            onClick={() => setMode("password")}
          >
            Password
          </button>
          <div className={`auth-tab-indicator auth-tab-indicator-${mode}`} />
        </div>

        <div className="auth-panel">
          {mode === "telegram" ? (
            <TelegramPanel onLogin={onTelegramLogin} />
          ) : (
            <PasswordPanel onLogin={onPasswordLogin} />
          )}
        </div>

        <div className="login-seal">印</div>
      </div>
    </div>
  );
}

function TelegramPanel({ onLogin }: { onLogin: (data: TelegramAuthPayload) => void }) {
  const widgetRef = useRef<HTMLDivElement>(null);

  useEffect(() => {
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (window as any).onTelegramAuth = (data: TelegramAuthPayload) => onLogin(data);

    const script = document.createElement("script");
    script.src = "https://telegram.org/js/telegram-widget.js?22";
    script.setAttribute(
      "data-telegram-login",
      import.meta.env.VITE_TELEGRAM_BOT_USERNAME
    );
    script.setAttribute("data-size", "large");
    script.setAttribute("data-onauth", "onTelegramAuth(user)");
    script.setAttribute("data-request-access", "write");
    script.async = true;
    widgetRef.current?.appendChild(script);

    return () => {
      // eslint-disable-next-line @typescript-eslint/no-explicit-any
      delete (window as any).onTelegramAuth;
    };
  }, [onLogin]);

  return (
    <>
      <p className="login-prompt">Sign in with Telegram to continue</p>
      <div ref={widgetRef} className="telegram-widget-container"></div>
    </>
  );
}

function PasswordPanel({ onLogin }: { onLogin: (creds: PasswordAuthPayload) => Promise<void> }) {
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [submitting, setSubmitting] = useState(false);

  const handleSubmit = async (e: React.FormEvent) => {
    e.preventDefault();
    if (!username.trim() || !password) return;
    setSubmitting(true);
    setError(null);
    try {
      await onLogin({ username: username.trim(), password });
    } catch (err) {
      setError(err instanceof Error ? err.message : "Login failed");
    } finally {
      setSubmitting(false);
    }
  };

  return (
    <form className="password-form" onSubmit={handleSubmit}>
      <label className="auth-field">
        <span className="auth-field-label">Username</span>
        <input
          type="text"
          autoComplete="username"
          className="auth-input"
          value={username}
          onChange={(e) => setUsername(e.target.value)}
          disabled={submitting}
          required
        />
      </label>

      <label className="auth-field">
        <span className="auth-field-label">Password</span>
        <input
          type="password"
          autoComplete="current-password"
          className="auth-input"
          value={password}
          onChange={(e) => setPassword(e.target.value)}
          disabled={submitting}
          required
        />
      </label>

      {error && <p className="auth-error">{error}</p>}

      <button type="submit" className="auth-submit" disabled={submitting}>
        {submitting ? "Signing in…" : "Sign in"}
      </button>
    </form>
  );
}
