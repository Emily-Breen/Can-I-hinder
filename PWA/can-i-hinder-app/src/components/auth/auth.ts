const KEY = "mock_auth_v1";
import type { MockSession } from "./types";


export function getSession(): MockSession | null {
  const raw = localStorage.getItem(KEY);
  if (!raw) return null;
  try {
    return JSON.parse(raw) as MockSession;
  } catch {
    return null;
  }
}

export function signIn(username: string) {
  const session: MockSession = { username, loggedInAt: Date.now() };
  localStorage.setItem(KEY, JSON.stringify(session));
}

export function signOut() {
  localStorage.removeItem(KEY);
}

export function isAuthed(): boolean {
  return getSession() !== null;
}