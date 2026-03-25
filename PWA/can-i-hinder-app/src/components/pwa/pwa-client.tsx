import { WS_URL } from "../../config";
import { getUsername } from "../auth/auth";
import type { GameAction, HelpEffect, HinderEffect, IncomingGameAction } from "./types";
import { canSend, recordSend } from "./rate-limit";
import { getSession, setSession } from "../auth/auth";


const params = new URLSearchParams(window.location.search);
// Callback to handle progress updates from the server, such as hinder count and unlocks, which can be set by the UI to update the display accordingly
let onProgressCallback: ((data: IncomingGameAction) => void) | null = null;

// Helper function to set the progress callback, which allows the UI to react to progress updates from the server such as hinder count and unlocks
export function setOnProgressCallback(
  cb: (data: IncomingGameAction) => void
) {
  onProgressCallback = cb;
}

let sessionId =
  params.get("session") ||
  getSession() ||
  "";

if (sessionId) {
  setSession(sessionId);
}
console.log("Session from storage:", sessionId);
// WebSocket connection using URL from config, which can be switched between local and prod servers
const ws = sessionId
  ? new WebSocket(`${WS_URL}?session=${sessionId}`)
  : null;

// WebSocket events log server events for debugging safeguarded against possibly null error
if (ws) {
  ws.onopen = () => {
    console.log(`Connected to WebSocket session: ${sessionId}`);
  };

  ws.onmessage = (event: MessageEvent) => {
  console.log("Message from server:", event.data);

  try {
    const data: IncomingGameAction = JSON.parse(event.data);

    // Handle normal action messages
    if (data.type === "action") {
      console.log(`Server event: ${data.user} ${data.action} ${data.effect}`);
    }

    // Handle progress messages
    if (data.type === "progress") {
      console.log(`Hinder count: ${data.hinderCount}`);
      // Log any unlocks that come in the progress updates for debugging
      if (data.unlock) {
        console.log(`Unlocked: ${data.unlock}`);
      }

      // Call the progress callback if it’s set, so that the UI can update with the new progress information
      if (onProgressCallback) {
        onProgressCallback(data);
      }
    }

  } catch (err) {
    console.error("Failed to parse message:", err);
  }
};
}

// Send a game action to the server
function sendAction(action: GameAction) {
  if (!ws || ws.readyState !== WebSocket.OPEN) {
    console.warn("WebSocket not ready");
    return;
  }

  ws.send(JSON.stringify(action));
  console.log("Sent:", action);
}
// Public functions to send help/hinder actions with user info from session
export function sendHelp(effect: HelpEffect) {
  const user = getUsername() ?? "Guest";
 // Check cooldown status before sending, and log a warning if the action is currently on cooldown with time left
  const check = canSend("help", effect);
  if (!check.ok) {
  console.warn(
    `On cooldown. Wait ${Math.ceil(check.waitMs / 1000)}s`
  );
  return;
}
 // Record the action in the rate limiter to start the cooldown, so that the UI can reflect the cooldown status immediately without waiting for server confirmation
  recordSend("help", effect);

  sendAction({
    user,
    action: "help",
    effect,
  });
}
export function sendHinder(effect: HinderEffect) {
  const user = getUsername() ?? "Guest";
// Check cooldown status before sending, and log a warning if the action is currently on cooldown with time left
  const check = canSend("hinder", effect);
 if (!check.ok) {
  console.warn(
    `On cooldown. Wait ${Math.ceil(check.waitMs / 1000)}s`
  );
  return;
}
// Record the action in the rate limiter to start the cooldown, so that the UI can reflect the cooldown status immediately without waiting for server confirmation
  recordSend("hinder", effect);

  sendAction({
    user,
    action: "hinder",
    effect,
  });
}
