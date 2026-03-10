import { API_BASE } from "../../config";

const TOKEN_KEY = "auth_token_v1";
const USER_KEY = "auth_user_v1";
// API URL is derived from config, which can be switched between local and prod servers
const API_URL = API_BASE;

const SESSION_KEY = "game_session";
//functions to manage sessions ids in local storage
export function setSession(id: string) {
  localStorage.setItem(SESSION_KEY, id);
}

export function getSession(): string | null {
  return localStorage.getItem(SESSION_KEY);
}

export function clearSession() {
  localStorage.removeItem(SESSION_KEY);
}
// Functions to manage auth token and user info in localStorage, and to perform login/register API calls
export function getToken(): string | null {
  return localStorage.getItem(TOKEN_KEY);
}
// Save the token to localStorage for session persistence across page reloads
export function setToken(token: string) {
  localStorage.setItem(TOKEN_KEY, token);
}
// Clear the token and user info from localStorage on logout
export function clearToken() {
  localStorage.removeItem(TOKEN_KEY);
  localStorage.removeItem(USER_KEY);
}
// Check if the user is authenticated by verifying the presence of a token in localStorage
export function isAuthed(): boolean {
  return getToken() !== null;
}
// Get the username from localStorage, which is set on successful login or registration, or return null if not found
export function getUsername(): string | null {
  return localStorage.getItem(USER_KEY);
}
// Perform login by sending credentials to the API, and on success, save the returned token and username to localStorage for session persistence
export async function signIn(username: string, password: string) {
  const res = await fetch(`${API_URL}/api/auth/login`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ username, password }),
  });

  if (!res.ok) {
    const msg = await res.text().catch(() => "");
    throw new Error(msg || "Login failed");
  }

  const data = (await res.json()) as { token: string };
  setToken(data.token);
  localStorage.setItem(USER_KEY, username);
}

// Perform registration by sending credentials to the API, and on success, save the returned token and username to localStorage for session persistence
export async function register(username: string, email: string, password: string) {
  const res = await fetch(`${API_URL}/api/auth/register`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify({ username, email, password }),
  });

  if (!res.ok) {
    const msg = await res.text().catch(() => "");
    throw new Error(msg || "Register failed");
  }

  const data = (await res.json()) as { token: string };
  setToken(data.token);
  localStorage.setItem(USER_KEY, username);
}

export function signOut() {
  clearToken();
}