import http from "http";
import { WebSocketServer } from "ws";
import { URL } from "url";

const port = process.env.PORT || 8080;

function broadcast(clients, data) {
  const text = JSON.stringify(data);

  for (const client of clients) {
    if (client.readyState === 1) {
      client.send(text);
    }
  }
}

const server = http.createServer((req, res) => {
  if (req.url === "/health") {
    res.writeHead(200, { "Content-Type": "text/plain" });
    return res.end("ok");
  }

  res.writeHead(200, { "Content-Type": "text/plain" });
  res.end("WebSocket server is running");
});

const wss = new WebSocketServer({ server });

const sessions = new Map();

wss.on("connection", (ws, req) => {
  const url = new URL(req.url, `http://${req.headers.host}`);
  const sessionId = url.searchParams.get("session") || "default";

  console.log("Client connected to session:", sessionId);

  if (!sessions.has(sessionId)) {
    sessions.set(sessionId, {
      clients: new Set(),
      hinderCount: 0,
      helpCount: 0,
      hinderUnlocked: false,
      helpUnlocked: false
    });
  }

  const session = sessions.get(sessionId);
  session.clients.add(ws);

  ws.on("message", (message) => {
    const text = message.toString();
    console.log(`Session ${sessionId} received:`, text);

    let data;
    try {
      data = JSON.parse(text);
    } catch {
      return;
    }

    if (data.action === "hinder" && data.effect === "spawn_brute") {
      session.hinderCount = 0;
      session.hinderUnlocked = false;

      broadcast(session.clients, {
        type: "reset",
        action: "hinder"
      });

      broadcast(session.clients, data);
      return;
    }

    
    if (data.action === "help" && data.effect === "god_mode") {
      session.helpCount = 0;
      session.helpUnlocked = false;

      broadcast(session.clients, {
        type: "reset",
        action: "help"
      });

      broadcast(session.clients, data);
      return;
    }

   
    if (data.action === "hinder") {
      session.hinderCount++;

      const response = {
        type: "progress",
        hinderCount: session.hinderCount
      };

      if (session.hinderCount >= 5 && !session.hinderUnlocked) {
        response.unlock = "spawn_brute";
        session.hinderUnlocked = true;
      }

      broadcast(session.clients, response);
    }

    
    if (data.action === "help") {
      session.helpCount++;

      const response = {
        type: "progress",
        helpCount: session.helpCount
      };

      if (session.helpCount >= 5 && !session.helpUnlocked) {
        response.unlock = "god_mode";
        session.helpUnlocked = true;
      }

      broadcast(session.clients, response);
    }

    
    broadcast(session.clients, data);
  });

  ws.on("close", () => {
    session.clients.delete(ws);

    if (session.clients.size === 0) {
      sessions.delete(sessionId);
      console.log("Session removed:", sessionId);
    }
  });
});

server.listen(port, () => {
  console.log(`Server listening on port ${port}`);
});