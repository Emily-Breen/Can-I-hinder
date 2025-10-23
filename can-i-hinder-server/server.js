// server.js
import { WebSocketServer } from "ws";

const wss = new WebSocketServer({ port: 8080 });
const clients = new Set();

wss.on("connection", (ws) => {
  console.log("New client connected");
  clients.add(ws);

  ws.on("message", (message) => {
    console.log("Received:", message.toString());

  
    for (const client of clients) {
      if (client.readyState === ws.OPEN) {
        client.send(message.toString());
      }
    }
  });

  ws.on("close", () => {
    clients.delete(ws);
    console.log("❌ Client disconnected");
  });
});

console.log("🌐 WebSocket server running on ws://localhost:8080");
