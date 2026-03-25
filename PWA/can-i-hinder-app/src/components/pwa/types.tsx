// Types for game actions sent over WebSocket
export type GameAction =
  | { user: string; action: "help"; effect: HelpEffect }
  | { user: string; action: "hinder"; effect: HinderEffect };
// Types for messages received from the gameclient over WebSocket
export type IncomingGameAction = 
  | { type: "action"; user: string; action: "help" | "hinder"; effect: string }
  | { type: "progress"; user: string; hinderCount: number; unlock?: string };

export type HinderEffect =
  | "spawn_enemy"
  | "drop_trap"
  | "slow_player"
  | "steal_power"
  | "spawn_brute";

export type HelpEffect =
  | "heal_player"
  | "speed_up_player"
  | "power_boost"
  | "shield_player";