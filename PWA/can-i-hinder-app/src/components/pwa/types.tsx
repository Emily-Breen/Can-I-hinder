// Types for game actions sent over WebSocket
export type GameAction =
  | { user: string; action: "help"; effect: HelpEffect }
  | { user: string; action: "hinder"; effect: HinderEffect };

export type HinderEffect =
  | "spawn_enemy"
  | "drop_trap"
  | "slow_player"
  | "steal_power";

export type HelpEffect =
  | "heal_player"
  | "speed_up_player"
  | "power_boost"
  | "shield_player";