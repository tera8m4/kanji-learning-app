import { useState, useEffect } from "react";
import { getToken, clearToken } from "../core/auth";
import type { Transport, TelegramAuthPayload, PasswordAuthPayload } from "../core/transport";

export function useAuth(transport: Transport) {
  // null while we're still asking the backend whether auth is required.
  const [authRequired, setAuthRequired] = useState<boolean | null>(null);
  const [hasToken, setHasToken] = useState(() => !!getToken());

  useEffect(() => {
    let cancelled = false;
    transport.getAuthConfig()
      .then((cfg) => { if (!cancelled) setAuthRequired(cfg.auth_required); })
      .catch(() => { if (!cancelled) setAuthRequired(true); }); // fail safe: require auth
    return () => { cancelled = true; };
  }, [transport]);

  const logout = () => {
    clearToken();
    setHasToken(false);
  };

  useEffect(() => {
    transport.onSessionExpired = logout;
    return () => { transport.onSessionExpired = null; };
  }, [transport]);

  const handleTelegramLogin = async (data: TelegramAuthPayload) => {
    await transport.login(data);
    setHasToken(true);
  };

  const handlePasswordLogin = async (creds: PasswordAuthPayload) => {
    await transport.loginWithPassword(creds);
    setHasToken(true);
  };

  // Still determining whether auth is needed -> don't flash the login screen.
  const isLoading = authRequired === null;
  // Authenticated if the backend doesn't require auth, or we hold a token.
  const isAuthenticated = authRequired === false || hasToken;

  return { isLoading, isAuthenticated, handleTelegramLogin, handlePasswordLogin, logout };
}
