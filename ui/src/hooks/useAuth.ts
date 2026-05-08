import { useState, useEffect } from "react";
import { getToken, clearToken } from "../core/auth";
import type { Transport, TelegramAuthPayload, PasswordAuthPayload } from "../core/transport";

export function useAuth(transport: Transport) {
  const [isAuthenticated, setIsAuthenticated] = useState(() => !!getToken());

  const logout = () => {
    clearToken();
    setIsAuthenticated(false);
  };

  useEffect(() => {
    transport.onSessionExpired = logout;
    return () => { transport.onSessionExpired = null; };
  }, [transport]);

  const handleTelegramLogin = async (data: TelegramAuthPayload) => {
    await transport.login(data);
    setIsAuthenticated(true);
  };

  const handlePasswordLogin = async (creds: PasswordAuthPayload) => {
    await transport.loginWithPassword(creds);
    setIsAuthenticated(true);
  };

  return { isAuthenticated, handleTelegramLogin, handlePasswordLogin, logout };
}
