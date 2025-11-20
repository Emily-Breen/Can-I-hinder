
interface GameAction {
  action: "help" | "hinder";
  effect: string;
}


const ws = new WebSocket("ws://localhost:8080");

// WebSocket events
ws.onopen = () => {
  console.log("Connected to WebSocket server ");
};

ws.onmessage = (event: MessageEvent) => {
  console.log("Message from server:", event.data);
  try {
    const data: GameAction = JSON.parse(event.data);
   
    console.log(`Server event: ${data.action} → ${data.effect}`);
  } catch (err) {
    console.error("Failed to parse message:", err);
  }
};

ws.onclose = () => {
  console.log(" Disconnected from WebSocket server");
};

ws.onerror = (err) => {
  console.error("WebSocket error:", err);
};


function sendAction(action: GameAction) {
  if (ws.readyState === WebSocket.OPEN) {
    ws.send(JSON.stringify(action));
    console.log("Sent:", action);
  } else {
    console.warn("WebSocket not ready");
  }
}


export function sendHelp() {
  sendAction({ action: "help", effect: "heal_player" });
}

export function sendHinder() {
  sendAction({ action: "hinder", effect: "spawn_enemy" });
}