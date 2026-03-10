import http from "http";
import { WebSocketServer } from "ws";
import { URL } from "url";

const port = process.env.PORT || 8080;

const server = http.createServer((req, res) => {
  if (req.url === "/health") {
    res.writeHead(200, { "Content-Type": "text/plain" });
    return res.end("ok");
  }

  res.writeHead(200, { "Content-Type": "text/plain" });
  res.end("WebSocket server is running");
});

// WebSocket server piggybacks on the same HTTP server/port
const wss = new WebSocketServer({ server });

const sessions = new Map();

wss.on("connection", (ws, req) => {
  
  const url = new URL(req.url, `http://${req.headers.host}`);
  const sessionId = url.searchParams.get("session") || "default";

  console.log("Client connected to session:", sessionId);

  if (!sessions.has(sessionId)) {
    sessions.set(sessionId, new Set());
  }

  const clients = sessions.get(sessionId);
  clients.add(ws);

  ws.on("message", (message) => {
    const text = message.toString();
    console.log(`Session ${sessionId} received:`, text);

    for (const client of clients) {
      if (client.readyState === ws.OPEN) {
        client.send(text);
      }
    }
  });

  ws.on("close", () => {
    clients.delete(ws);
    console.log("Client disconnected from session:", sessionId);

    if (clients.size === 0) {
      sessions.delete(sessionId);
      console.log("Session removed:", sessionId);
    }
  });
});

server.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});