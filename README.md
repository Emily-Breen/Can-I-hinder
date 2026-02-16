# Can I Hinder?

An interactive multiplayer game experience where spectators can influence gameplay in real-time through a web-based Progressive Web App (PWA). The game combines a C++ SFML game client with a WebSocket server and React-based control interface to create a crowd-interactive gaming experience.

## 🎮 Project Overview

**Can I Hinder?** is a crowd-interactive game where viewers can either help or hinder the player in real-time. The project consists of three main components:

1. **C++ Game Client** - A 2D game built with SFML that responds to real-time commands
2. **WebSocket Server** - A Node.js server that facilitates communication between the game and web clients
3. **PWA Control Interface** - A React-based web app that allows users to send help/hinder commands

## 🏗️ Project Structure

```
Can I hinder/
├── Can I hinder/                   # C++ Game Client (Visual Studio Project)
│   ├── Can I hinder/               # Source files
│   │   ├── Game.cpp/h              # Game.cpp/h # Main loop + applies events to gameplay (spawn/heal)
│   │   ├── main.cpp                # Entry point
│   │   ├── Player.cpp/h            # Player entity
│   │   ├── NPC.cpp/h               # Non-player characters (Enemies)
│   │   ├── Entity.cpp/h            # Base entity class
│   │   ├── Map.cpp/h               # Map rendering and management
│   │   ├── Camera.cpp/h            # Camera system
│   │   ├── Input Handler.cpp/h     # Input management
│   │   ├── AnimationHandler.cpp/h  # Animation system
│   │   ├── WebSocketClient.cpp/h   # WebSocketClient.cpp/h # Receives JSON events (help/hinder) from server
|   |   ├── AIBehaviour.cpp/h       # Movement behaviours for the NPC
|   |   ├── Audio.cpp/h             # Music/sound effects for in game actions/ambience
|   |   ├── HUD.cpp/h               # UI on screen (Health bar, chat box, hotbar, current weapon)
|   |   ├── Items.cpp/h             # Item loading, animation, control flags
│   │   ├── MathUtils.h             # Utility functions
│   │   ├── Obstacles.h             # Obstacle definitions
│   │   ├── ASSETS/                 # Game assets
│   │   │   ├── AUDIO/              # Sound files
│   │   │   ├── FONTS/              # Font files
│   │   │   ├── IMAGES/             # Sprites and textures
│   │   │   ├── LEVELS/             # Tiled map files (.tmx)
│   │   │   └── LICENSES/           # Asset licenses
│   │   └── Libs/                   # External libraries
│   │       └── tmxlite-master/     # TMX map parsing library
│   └── Can I hinder.sln            # Visual Studio solution
│
├── can-i-hinder-server/            # WebSocket Server
│   ├── server.js                   # WebSocket server implementation
│   └── package.json                # Node.js dependencies
│
└── PWA/                             # Progressive Web App
    └── can-i-hinder-app/
        ├── src/
        │   ├── App.tsx             # Main app component
        │   ├── main.tsx            # Entry point
        │   ├── components/
        │   
        │   │   ├── pwa-client.tsx  # WebSocket client logic
        │   │   └
        │   └── assets/             # Web assets
        ├── index.html              # HTML template
        ├── vite.config.ts          # Vite configuration
        └── package.json            # NPM dependencies
```

## 🔧 Technology Stack

### Game Client (C++)
- **SFML 3.0** - Graphics, audio, and windowing
- **Boost.Beast** - WebSocket client implementation
- **Boost.Asio** - Asynchronous I/O
- **nlohmann/json** - JSON parsing
- **TMXLite** - Tiled map format parsing
- **vcpkg** - C++ package management

### WebSocket Server
- **Node.js** - JavaScript runtime
- **ws** - WebSocket server library

### PWA Control Interface
- **React 19.1** - UI framework
- **TypeScript 5.9** - Type-safe JavaScript
- **Vite 7.1** - Build tool and dev server

## 🚀 Getting Started

### Prerequisites

- **For the Game Client:**
  - Visual Studio 2022 (with C++ development tools)
  - vcpkg package manager
  - SFML 3.0
  - Boost libraries
  - nlohmann/json

- **For the WebSocket Server:**
  - Node.js 18+ and npm

- **For the PWA:**
  - Node.js 18+ and npm

### Installation & Setup

#### 1. WebSocket Server

```powershell
cd can-i-hinder-server
npm install
node server.js
```

The server will start on `ws://localhost:8080`

#### 2. PWA Control Interface

```powershell
cd PWA\can-i-hinder-app
npm install
npm run dev
```

The web interface will be available at `http://localhost:5173` (or another port shown in terminal)

#### 3. C++ Game Client

1. Open `Can I hinder.sln` in Visual Studio 2022
2. Ensure all dependencies are installed via vcpkg
3. Build the solution (F7)
4. Run the game (F5)

## 🎯 How It Works

### Game Flow

1. **Server Launch**: The WebSocket server starts and listens for connections
2. **Web Client Connection**: Users open the PWA in their browser and connect to the WebSocket server
3. **Game Client Connection**: The C++ game client connects to the same WebSocket server
4. **Interaction**: Users click "Help" or "Hinder" buttons in the PWA
5. **Action Processing**: 
   - "Help" sends a `heal_player` effect
   - "Hinder" sends a `spawn_enemy` effect
6. **Game Response**: The game client receives commands and updates gameplay

### WebSocket Communication

Messages are sent as JSON objects:

```json
{
  "action": "help" | "hinder",
  "effect": "heal_player" | "spawn_enemy"
}
```

## 📁 Key Components

### Game Client Components

- **Game**: Main game loop, event processing, and rendering
- **Player**: Player character with movement and timers
- **NPC**: Enemy entities that interact with the player
- **WebSocketClient**: Handles real-time communication with the server
- **MapRenderer**: Renders Tiled maps & AABB collisions (.tmx files)
- **Camera**: Manages viewport and camera movement
- **AnimationHandler**: Manages sprite animations
- **HUD**: Manages UI elements in game
- **Items**: Handles item rendering and animations 
- **AIBehaviour**: Manages NPC movement logic seperation and wall avoidance
  
- **NOT ACTIVE IN GAME YET**:
- **Audio**: Will manage all sound effects/music in game
- **Obstacles**: Will manage obstacle spawning in game

### Server

- Broadcasts all messages to all connected clients
- Maintains active WebSocket connections
- Logs connection/disconnection events

### PWA Components

- **App.tsx**: Main UI with Help/Hinder buttons
- **pwa-client.tsx**: WebSocket client logic and message handling
- **pwa-controls.tsx**: Control interface components

## 🎨 Assets

The game uses custom assets organized in:
- **AUDIO**: Sound effects and music
- **FONTS**: Text rendering fonts
- **IMAGES**: Character sprites, tilesets, and UI elements
- **LEVELS**: Tiled map files (.tmx) for level design

## 📝 Development Notes

### Building the Game

The project uses Visual Studio's build system with:
- Debug and Release configurations
- x86 architecture (32bit)
- SFML libraries linked via vcpkg
- TMXLite library included in `Libs/`

### Map Editing

Maps are created using [Tiled Map Editor] and saved as `.tmx` files in `ASSETS/LEVELS/`.

### WebSocket Configuration

Default settings:
- Server: `localhost:8080`
- Protocol: `ws://` (WebSocket, not secure)

## 🐛 Troubleshooting

### Game won't connect to server
- Ensure the WebSocket server is running on port 8080
- Check firewall settings
- Verify the connection string in `WebSocketClient.cpp`

### PWA connection issues
- Check that the WebSocket URL in `pwa-client.tsx` matches the server address
- Ensure CORS is properly configured if hosting on different domains

### Build errors
- Verify all vcpkg packages are installed
- Check that SFML 3.0 is properly linked
- Ensure Boost libraries are available

## 📜 License

See `ASSETS/LICENSES/` for asset licensing information.


 ### TODO:
 **Networking**
 - Begin work in a RESTful Api to log usernames, pass and analyytics (how many times which button is pressed)
 - Further work to be done on PWA (service worker, Login page, Radial menu for Spectator choices, connection status, Mobile phone responsiveness, Identifiers)
 - Move PWA and server.js to a P.H.E and change ports to new end point(https).
  **SFML**
  - Implement puzzle mechanic logic.
  - Updaye so items effects apply to health bar.
  - Implement level progression (Map1-Map2-Map3-super secret ending).
  - Implement Obstacles that spawn when action is sent via websocket message
  - Ractor code to remove any dead redundant code (better performance)
  - Begin inserting audio into game.
---

**Built using C++, Node.js, TypeScript & React**
