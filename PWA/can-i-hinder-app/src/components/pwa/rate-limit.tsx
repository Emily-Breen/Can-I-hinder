import type { GameAction, HelpEffect, HinderEffect } from "./types";
// Cooldown durations for each effect, used by the limiter to determine how long to block an action after it’s used
const HELP_COOLDOWN_MS: Record<HelpEffect, number> = {
  heal_player: 10_000,
  speed_up_player: 12_000,
  shield_player: 15_000,
  power_boost: 8_000,
};
// Cooldown durations for each effect, used by the limiter to determine how long to block an action after it’s used
const HINDER_COOLDOWN_MS: Record<HinderEffect, number> = {
  spawn_enemy: 15_000,
  drop_trap: 12_000,
  slow_player: 10_000,
  steal_power: 18_000,
};
//client side rate limiter to prevent spamming and overwhelming the server
const STORAGE_KEY = "pwa_rate_limit_state";

type Effect = HelpEffect | HinderEffect;
type ActionKey = `${GameAction["action"]}:${Effect}`;

const cooldownUntil = new Map<ActionKey, number>();
// Helper to get cooldown duration for a given action/effect, used when recording a send to set the correct cooldown time
function cooldownFor(
  action: GameAction["action"],
  effect: Effect
) {
  return action === "help"
    ? HELP_COOLDOWN_MS[effect as HelpEffect]
    : HINDER_COOLDOWN_MS[effect as HinderEffect];
}
// Helper to create a consistent key for the cooldown map based on action and effect
function keyFor(
  action: GameAction["action"],
  effect: Effect
): ActionKey {
  return `${action}:${effect}` as ActionKey;
}

// Load the cooldown state from localStorage so it persists across page reloads
function loadState() {
  try {
    const raw = localStorage.getItem(STORAGE_KEY);
    if (!raw) return;

    const parsed = JSON.parse(raw);

    if (parsed.cooldownUntil) {
      for (const [k, v] of Object.entries(parsed.cooldownUntil)) {
        cooldownUntil.set(k as ActionKey, v as number);
      }
    }
  } catch {
    // ignore corrupt state
  }
}
// Save the cooldown state to localStorage so it persists across page reloads
function saveState() {
  const cooldownObj: Record<string, number> = {};
  cooldownUntil.forEach((v, k) => {
    cooldownObj[k] = v;
  });

  localStorage.setItem(
    STORAGE_KEY,
    JSON.stringify({
      cooldownUntil: cooldownObj,
    })
  );
}

loadState();
// Save state on unload so cooldowns persist if the tab is accidentally refreshed or closed or sneaky sneaky user tries to bypass cooldown by opening a new tab
export function canSend(
  action: GameAction["action"],
  effect: Effect
) {
  const now = Date.now();
  const k = keyFor(action, effect);
  const until = cooldownUntil.get(k) ?? 0;

  if (now < until) {
    return {
      ok: false as const,
      waitMs: until - now,
    };
  }

  return { ok: true as const };
}
//function for radial menu to check cooldown status of an action/effect, used to disable buttons and show cooldown timers in the UI
export function recordSend(
  action: GameAction["action"],
  effect: Effect
) {
  const now = Date.now();
  const k = keyFor(action, effect);

  cooldownUntil.set(k, now + cooldownFor(action, effect));
  saveState();
}
//Function for radial menu to check cooldown status of an action/effect, used to disable buttons and show cooldown timers in the UI
export function getActionStatus(
  action: GameAction["action"],
  effect: Effect
) {
  const now = Date.now();
  const k = keyFor(action, effect);
  const until = cooldownUntil.get(k) ?? 0;

  const cooldownMsLeft = Math.max(0, until - now);

  return {
    canSend: cooldownMsLeft === 0,
    cooldownMsLeft,
  };
}