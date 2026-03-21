# Can I Hinder?

An interactive multiplayer game experience where spectators can influence gameplay in real time through a web-based Progressive Web App (PWA). The project now combines a C++ SFML game client, a session-based WebSocket relay, an ASP.NET Core API, and a React-based control interface to create a crowd-interactive gaming experience.

## 🎮 Project Overview

**Can I Hinder?** is a crowd-interactive game where viewers can either help or hinder the player in real time. The project currently consists of four main components:

1. **C++ Game Client** - A 2.5D SFML game with menus, combat/gameplay systems, session creation, and live reaction to spectator commands
2. **WebSocket Server** - A Node.js relay that routes live gameplay actions to the correct game session
3. **REST API** - An ASP.NET Core backend for authentication, session code generation, password reset flow, and persistent user data
4. **PWA Control Interface** - A React-based web app with login/register flows and radial spectator controls for help/hinder actions

## 🏗️ Project Structure

```
Can I hinder/
├── Can I hinder/
│   ├── Can I hinder/               # C++ Game Client (Visual Studio project)
│   │   ├── Game.cpp/h              # Main game loop, menu state, gameplay effects, session display
│   │   ├── main.cpp                # Game entry point
│   │   ├── Player.cpp/h            # Player movement, combat, health, progression state
│   │   ├── NPC.cpp/h               # Enemy entities and AI-driven interactions
│   │   ├── Entity.cpp/h            # Shared entity base class
│   │   ├── Map.cpp/h               # Tiled map loading, rendering, collisions
│   │   ├── Camera.cpp/h            # Camera and viewport management
│   │   ├── Input Handler.cpp/h     # Keyboard/input handling
│   │   ├── AnimationHandler.cpp/h  # Animation system for sprites/entities
│   │   ├── WebSocketClient.cpp/h   # Secure/local WebSocket client + API session creation
│   │   ├── AIBehaviour.cpp/h       # NPC movement behaviours and steering logic
│   │   ├── HUD.cpp/h               # On-screen UI, bars, and gameplay feedback
│   │   ├── Items.cpp/h             # Collectibles, item state, and item animation logic
│   │   ├── Audio.cpp/h             # Audio system for music and SFX
│   │   ├── FinalLevel.cpp/h        # Final level / boss battle flow
│   │   ├── Puzzles.cpp/h           # Puzzle mechanic barebones
│   │   ├── MathUtils.cpp/h         # Utility helpers
│   │   ├── Obstacles.h             # Obstacle definitions for gameplay hazards
│   │   ├── ASSETS/
│   │   │   ├── AUDIO/              # Sound files
│   │   │   ├── certs/              # TLS certificate bundle for secure WebSocket/API usage
│   │   │   ├── FONTS/              # Font files
│   │   │   ├── IMAGES/             # Sprites, UI textures, tilesets
│   │   │   ├── LEVELS/             # Tiled map files (.tmx)
│   │   │   └── LICENSES/           # Asset licenses
│   │   ├── Libs/
│   │   │   └── tmxlite-master/     # TMX map parsing library
│   │   ├── vcpkg.json              # C++ dependency manifest placeholder
│   │   └── Can I hinder.vcxproj    # Visual Studio project file
│   └── Can I hinder.sln            # Visual Studio solution
│
├── Can I HinderAPI/
│   ├── Can I HinderAPI.sln         # ASP.NET Core API solution
│   └── Can I HinderAPI/
│       ├── Program.cs              # API startup, JWT auth, CORS, Swagger, migrations
│       ├── Controllers/
│       │   ├── AuthController.cs   # Register, login, forgot password, reset password
│       │   ├── GameController.cs   # Game session code generation
│       │   └── UserController.cs   # Authenticated user profile endpoint
│       ├── Data/
│       │   └── AppDbContext.cs     # EF Core database context
│       ├── Models/
│       │   ├── User.cs             # User credentials and password reset fields
│       │   ├── AnalyticsEvent.cs   # Analytics event storage model
│       │   └── UserPreference.cs   # User preference storage model
│       ├── Services/
│       │   └── EmailService.cs     # Password reset email delivery
│       ├── Migrations/             # EF Core migration history
│       └── appsettings.json        # Local DB, JWT, and CORS configuration
│
├── can-i-hinder-server/            # Session-based WebSocket relay
│   ├── server.js                   # HTTP + WebSocket server with per-session fanout
│   └── package.json                # Node.js dependencies
│
└── PWA/
    └── can-i-hinder-app/
        ├── src/
        │   ├── App.tsx             # Main spectator UI and radial action menus
        │   ├── main.tsx            # Router and protected route setup
        │   ├── config.ts           # API/WebSocket endpoint configuration
        │   ├── global.css          # Shared styling
        │   ├── assets/             # Web images and branding assets
        │   └── components/
        │       ├── auth/           # Login, register, forgot/reset password flows
        │       ├── pwa/            # WebSocket client, cooldowns, action types
        │       └── radial-menu/    # Spectator radial menu UI
        ├── public/                 # Static public assets
        ├── server.js               # Express server for serving production build output
        ├── vite.config.ts          # Vite + PWA configuration
        └── package.json            # Frontend dependencies and scripts
```

## 🔧 Technology Stack

### Game Client (C++)
- **SFML 3.0** - Graphics, audio, input, and windowing
- **Boost.Beast** - HTTP/WebSocket client implementation
- **Boost.Asio** - Async I/O and socket handling
- **OpenSSL** - TLS support for secure API/WebSocket connections
- **nlohmann/json** - JSON parsing
- **TMXLite** - Tiled map format parsing
- **Visual Studio 2022** - Primary build environment

### WebSocket Server
- **Node.js** - JavaScript runtime
- **ws** - WebSocket server library
- **HTTP module** - Health endpoint and shared server host

### REST API
- **ASP.NET Core** - Backend web API
- **Entity Framework Core** - Database access and migrations
- **SQL Server / LocalDB** - User and analytics storage
- **JWT Bearer Auth** - Authentication for protected endpoints
- **Swagger / OpenAPI** - API documentation and testing

### PWA Control Interface
- **React 19** - UI framework
- **TypeScript 5.9** - Type-safe frontend code
- **React Router 7** - Routing and protected pages
- **Vite 7** - Build tool and dev server
- **vite-plugin-pwa** - Service worker and installable PWA support
- **Express** - Production hosting for built frontend assets

## 🚀 Getting Started

### Prerequisites

- **For the Game Client:**
  - Visual Studio 2022 with Desktop development for C++
  - SFML 3.0 and Boost libraries available to the project
  - OpenSSL support for secure WebSocket/API requests
  - TMXLite sources in `Libs/tmxlite-master`

- **For the REST API:**
  - .NET SDK 8+
  - SQL Server LocalDB or another SQL Server instance
  - A configured JWT signing key in `appsettings.json` or user secrets for auth flows

- **For the WebSocket Server:**
  - Node.js 18+ and npm

- **For the PWA:**
  - Node.js 18+ and npm

### Installation & Setup - LOCAL ONLY

#### 1. REST API

```powershell
cd "Can I HinderAPI/Can I HinderAPI"
dotnet restore
dotnet run
```

By default the API uses LocalDB from `appsettings.json` and applies EF Core migrations on startup.

Before using login/register locally, set a non-empty `Jwt:Key` value in the API configuration.

#### 2. WebSocket Server

```powershell
cd can-i-hinder-server
npm install
npm start
```

The local relay starts on `ws://localhost:8080` and exposes `http://localhost:8080/health`.

#### 3. PWA Control Interface

```powershell
cd PWA\can-i-hinder-app
npm install
npm run dev
```

The web interface will be available at `http://localhost:5173` or another Vite port shown in the terminal.

Optional environment values used by the app:
- `VITE_WS_URL` for the WebSocket server address
- `VITE_API_URL` for the API base URL

#### 4. C++ Game Client

1. Open `Can I hinder.sln` in Visual Studio 2022.
2. Confirm the required C++ libraries are available to the project.
3. Build the solution.
4. Run the game.

For local testing, the game can be switched between secure hosted networking and local WebSocket mode through the client configuration in the C++ project.

## 🎯 How It Works

### Game Flow

1. **Game Session Creation**: The C++ game client requests a session code from the API.
2. **Server Launch**: The WebSocket relay starts and waits for connections.
3. **Web Client Connection**: Spectators open the PWA, authenticate, and join a game session using its code.
4. **Game Client Connection**: The game connects to the same session on the WebSocket server.
5. **Interaction**: Users choose help or hinder actions from the radial menus.
6. **Action Processing**:
   - Help actions currently include `heal_player`, `speed_up_player`, `power_boost`, and `shield_player`
   - Hinder actions currently include `spawn_enemy`, `drop_trap`, `slow_player`, and `steal_power`
7. **Game Response**: The game client receives the JSON payload and applies the relevant gameplay effect.

### WebSocket Communication

Messages are sent as JSON objects:

```json
{
  "user": "spectator_name",
  "action": "help",
  "effect": "heal_player"
}
```

Connections are grouped by session using the query string, for example:

```text
ws://localhost:8080/?session=ABC123
```

## 📁 Key Components

### Game Client Components

- **Game**: Main loop, menu state handling, gameplay effect application, and session display
- **Player**: Player movement, health, combat values, and buff/debuff targets
- **NPC**: Enemy entities that react to game state and AI behaviour
- **WebSocketClient**: Handles local/secure socket connections and session creation through the API
- **MapRenderer**: Loads and renders Tiled maps and collision geometry
- **Camera**: Manages viewport and camera movement
- **AnimationHandler**: Manages sprite animations
- **HUD**: Draws in-game UI, health feedback, and other overlays
- **Items**: Handles collectible/item rendering and item state
- **AIBehaviour**: Separates NPC movement logic and steering rules
- **FinalLevel**: Contains the boss/final encounter flow
- **Puzzles**: Placeholder for planned puzzle gameplay systems
- **Audio**: Audio system scaffolding for music and sound effects
- **Obstacles**: Placeholder definitions for spawned gameplay hazards

### Server

- Hosts an HTTP server and WebSocket server on the same port
- Groups clients by session ID instead of broadcasting globally
- Rebroadcasts messages only to clients in the same session
- Exposes a `/health` endpoint for status checks

### PWA Components

- **App.tsx**: Main spectator UI with Help/Hinder actions and logout
- **auth/**: Login, registration, protected route, forgot password, and reset password flows
- **pwa-client.tsx**: WebSocket connection, session binding, and action sending
- **rate-limit.tsx**: Client-side cooldown handling for spectator actions
- **radial-menu/**: Radial action selector used for help and hinder choices
- **config.ts**: Central API/WebSocket endpoint configuration

## 🎨 Assets

The game uses custom assets organized in:
- **AUDIO**: Sound effects and music
- **certs**: CA certificate bundle for secure remote connections
- **FONTS**: Text rendering fonts
- **IMAGES**: Character sprites, tilesets, and UI elements
- **LEVELS**: Tiled map files (`.tmx`) for level design
- **LICENSES**: Asset licensing information

## 📝 Development Notes

### Building the Game

The project currently uses the Visual Studio build system with:
- Debug and Release configurations
- x86 target configuration in the Visual Studio project
- SFML, Boost, JSON, and TLS-enabled networking dependencies
- TMXLite included directly under `Libs/`

### Map Editing

Maps are created using Tiled Map Editor and saved as `.tmx` files in `ASSETS/LEVELS/`.

### WebSocket Configuration

Default settings:
- Local relay: `ws://localhost:8080`
- Hosted relay support is also present through secure WebSocket (`wss://`)
- Sessions are joined with the `session` query parameter

## 🐛 Troubleshooting

### Game won't connect to server
- Ensure the WebSocket relay is running on the expected port
- Confirm the game and PWA are using the same session code
- Verify the networking mode in the C++ client matches the environment you are testing
- If using secure hosted networking, confirm the certificate bundle exists in `ASSETS/certs/`

### PWA connection issues
- Check that `VITE_WS_URL` and `VITE_API_URL` point to the correct services
- Make sure the API allows the frontend origin through `AllowedOrigins`
- If actions are not sending, confirm a session ID is present in the URL or local storage

### API/auth issues
- Set a non-empty `Jwt:Key` before testing login or registration locally
- Ensure the database connection string is valid and SQL Server/LocalDB is available
- Check whether password reset email configuration is set correctly for the API service

### Build errors
- Verify the required C++ dependencies are available in Visual Studio
- Check that SFML and Boost are correctly linked
- Ensure the .NET SDK and Node.js versions meet the project requirements

## 📜 License

See `ASSETS/LICENSES/` for asset licensing information.


 ### TODO:
 **Networking**
 - Add analytics endpoints and wire `AnalyticsEvent`/`UserPreference` models into live API features.
 - Improve spectator session UX with clearer join flow, connection state, and reconnect feedback.
 - Finalize hosted deployment wiring between the PWA, relay server, and API endpoints.
 **SFML**
 - Finish puzzle mechanic logic and integrate it into gameplay.
 - Connect remaining item effects cleanly into HUD/health feedback.
 - Finish level progression flow into the final level / ending path.
 - Implement obstacle spawning for the `drop_trap` hinder action.
 - Continue refactoring older gameplay code and dead paths for maintainability/performance.
 - Expand and fully integrate audio into the game loop.
---

**Built using C++, Node.js, ASP.NET Core, TypeScript & React**
