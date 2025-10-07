# BOMBERMAN - Multiplayer Network Game

## Project Overview

This is a networked implementation of the classic Bomberman game built in C. The project features a client-server architecture where multiple players can connect and compete in real-time matches. Players navigate a grid-based arena, place bombs to eliminate opponents, and communicate via an in-game chat system.

### Key Features

- **Multiplayer Support**: Two game modes available
  - Free-for-all (4 players)
  - Team-based 2v2 (4 players in 2 teams)
- **Real-time Gameplay**: UDP multicast for efficient game state synchronization
- **Interactive Terminal UI**: Built with ncurses for a responsive gaming experience
- **In-game Chat**: Players can communicate during matches
- **Bomb Mechanics**: Strategic bomb placement with explosion patterns
- **Player Spawning**: Players spawn at grid corners for balanced gameplay

### Technical Architecture

**Communication Protocols:**
- **TCP**: Used for reliable client-server communication (game setup, chat messages, player status)
- **UDP Multicast**: Used for real-time game state updates (grid changes, player movements)
- **Multi-threading**: Server handles multiple clients concurrently using pthreads

**Network Configuration:**
- TCP Port: 4000
- UDP Port: 5000
- Multicast addresses:
  - Free-for-all: `ff12::1:2:3` on port 10000
  - 2v2 mode: `ff12::1:2:4` on port 10001

**Project Structure:**
```
.
├── client/          # Client-side code
│   ├── main_client.c
│   ├── connexion_client.c
│   ├── ncurses.c    # Terminal UI rendering
│   ├── move.c       # Player movement handling
│   ├── ready.c      # Player ready state management
│   └── ...
├── serveur/         # Server-side code
│   ├── main_server.c
│   ├── game.c       # Game loop and event handling
│   ├── launch_game.c # Grid initialization and multicast
│   ├── manage_action.c # Player action processing
│   ├── bomb.c       # Bomb explosion logic
│   └── ...
└── Makefile         # Build configuration
```

## Game Configuration

Game parameters can be modified in `serveur/globals_serv.h`:
- `NB_JOUEURS_4J`: Number of players for free-for-all (default: 2 for testing)
- `NB_JOUEURS_2V2`: Number of players for 2v2 mode (default: 4)
- `HEIGHT`: Grid height (default: 20)
- `WIDTH`: Grid width (default: 20)
- `MULTICAST`: Network interface name (default: "eth0")

## Compilation

The project uses a root-level Makefile that orchestrates compilation of both client and server components.

### Prerequisites
- GCC compiler
- ncurses library
- pthread library
- IPv6 network support

### Build Commands

**Compile everything:**
```bash
make
```

This creates two executables:
- `client/bomberman_client`
- `serveur/bomberman_serv`

**Clean build artifacts:**
```bash
make clean
```

## Running the Game

### 1. Start the Server

In one terminal, launch the server:
```bash
make server_run
```

Or directly:
```bash
cd serveur
./bomberman_serv
```

The server will:
- Bind to TCP port 4000 and UDP port 5000
- Wait for client connections
- Display connection status messages

### 2. Start Client(s)

In separate terminals (one per player), launch clients:
```bash
make client_run
```

Or directly:
```bash
cd client
./bomberman_client
```

### 3. Game Setup

Each client will be prompted to choose a game mode:
- Press `1` for free-for-all mode (every man for himself)
- Press `2` for 2v2 team mode
- Press `p` or `P` to quit

After selecting a mode, players must declare themselves ready by typing any message and pressing Enter. There's a 60-second timeout for readiness declaration.

Once all required players are ready, the game starts automatically.

## Controls

### Movement
- **Arrow Up**: Move up
- **Arrow Down**: Move down
- **Arrow Left**: Move left
- **Arrow Right**: Move right

### Actions
- **F1**: Place a bomb
- **Type text**: Compose a chat message
- **F4**: Send chat message

### Exiting
- **Ctrl+C**: Quit the game/server

## Gameplay Mechanics

### Grid Legend
- `O`: Empty space
- `X`: Indestructible wall (white)
- `x`: Destructible wall (yellow)
- `B`: Bomb (red)
- `1-4`: Players (colored by ID: cyan, green, magenta, blue)

### Game Rules
1. **Objective**: Be the last player standing (free-for-all) or eliminate the opposing team (2v2)
2. **Spawning**: Players spawn at the four corners of the grid
3. **Bombs**: 
   - Explode after 3 seconds
   - Create cross-shaped explosion patterns
   - Destroy destructible walls
   - Eliminate players caught in the blast
4. **Walls**: 
   - Indestructible walls block movement and explosions
   - Destructible walls can be blown up to create paths
5. **Victory**: Last surviving player/team wins

## Network Details

### Protocol Flow

**Game Initialization (TCP):**
1. Client connects to server
2. Client selects game mode (CODEREQ 1 or 2)
3. Server responds with game setup (CODEREQ 9 or 10) including multicast address
4. Client subscribes to multicast group
5. Client sends ready signal (CODEREQ 3 or 4)

**During Gameplay:**
- **UDP Multicast**: Server broadcasts grid updates at ~20Hz
  - Initial grid (CODEREQ 11)
  - Grid modifications (CODEREQ 12)
- **UDP Unicast**: Clients send actions to server (CODEREQ 5)
- **TCP**: Chat messages (CODEREQ 7 client → CODEREQ 13 server broadcast)

**Game End:**
- Player elimination (CODEREQ 20)
- Game victory announcement (CODEREQ 15 for free-for-all, CODEREQ 16 for 2v2)

## Troubleshooting

**Server won't start:**
- Check if ports 4000/5000/10000/10001 are available
- Ensure you have permissions to bind to these ports
- Verify IPv6 is enabled on your system

**Client can't connect:**
- Verify the server is running
- Check the `SERVER_ADDR` in `client/globals_client.h` matches your server's IPv6 address
- Ensure network interface name in `MULTICAST` constant matches your system

**Game doesn't start:**
- Verify all required players have joined and declared ready
- Check server console for error messages
- Ensure the number of players matches configuration in `globals_serv.h`

**Multicast issues:**
- Verify the network interface name (`eth0` by default) is correct for your system
- Use `ip link show` to list available interfaces
- Update `MULTICAST` constant in `serveur/globals_serv.h` if needed

## Development Notes

- The project uses IPv6 for all network communication
- Server is designed to handle one game instance at a time
- Memory management is handled explicitly (malloc/free)
- Threading is used for concurrent client handling and asynchronous bomb explosions
- Grid state is shared between threads with mutex protection

## License

This is an educational project developed for learning network programming and game development concepts.
