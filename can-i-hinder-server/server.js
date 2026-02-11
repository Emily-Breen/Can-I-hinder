import http from "http";
import { WebSocketServer } from "ws";

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

const clients = new Set();

wss.on("connection", (ws) => {
  console.log("New client connected");
  clients.add(ws);

  ws.on("message", (message) => {
    const text = message.toString();
    console.log("Received:", text);

    for (const client of clients) {
      if (client.readyState === ws.OPEN) client.send(text);
    }
  });

  ws.on("close", () => {
    clients.delete(ws);
    console.log("Client disconnected");
  });
});

server.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});
