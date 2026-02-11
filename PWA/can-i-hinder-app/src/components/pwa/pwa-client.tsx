import { WS_URL } from "../../config";
import { getSession } from "../auth/auth";
import type { GameAction, HelpEffect, HinderEffect } from "./types";
// WebSocket connection using URL from config, which can be switched between local and prod servers
const ws = new WebSocket(WS_URL);

// WebSocket events
ws.onopen = () => {
  console.log("Connected to WebSocket server ");
};

ws.onmessage = (event: MessageEvent) => {
  console.log("Message from server:", event.data);
  try {
    const data: GameAction = JSON.parse(event.data);
   
    console.log(`Server event: ${data.user} ${data.action} ${data.effect}`);
  } catch (err) {
    console.error("Failed to parse message:", err);
  }
};
// Log disconnections and errors for debugging
ws.onclose = () => {
  console.log(" Disconnected from WebSocket server");
};
// Log errors for debugging
ws.onerror = (err) => {
  console.error("WebSocket error:", err);
};

// Send a game action to the server
function sendAction(action: GameAction) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify(action));
    console.log("Sent:", action);
  } else {
    console.warn("WebSocket not ready");
  }
}
// Public functions to send help/hinder actions with user info from session
export function sendHelp(effect: HelpEffect) {
  const session = getSession();
  const user = session?.username ?? "Guest";

  sendAction({
    user,
    action: "help",
    effect,
  });
}

export function sendHinder(effect: HinderEffect) {
  const session = getSession();
  const user = session?.username ?? "Guest";

  sendAction({
    user,
    action: "hinder",
    effect,
  });
}