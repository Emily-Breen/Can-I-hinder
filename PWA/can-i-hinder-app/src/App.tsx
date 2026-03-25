import { useMemo, useState, useEffect } from "react";
import { sendHelp, sendHinder, setOnProgressCallback } from "./components/pwa/pwa-client";
import RadialMenu from "./components/radial-menu/radial-menu";
import type { RadialItem } from "./components/radial-menu/types";
import { signOut } from "./components/auth/auth";
import { useNavigate } from "react-router-dom";
import "./global.css";
import Boo from "./assets/Boo.png";
import Bee from "./assets/Bee.png";

const title = "Choose your path";

function App() {
  const navigate = useNavigate();
  //state for which menu is open and where it’s anchored (null if closed)
  const [hinderOpen, setHinderOpen] = useState(false);
  const [helpOpen, setHelpOpen] = useState(false);
  const [anchor, setAnchor] = useState<{ x: number; y: number } | null>(null);
  const [unlocks, setUnlocks] = useState<string[]>([]);
  const [hinderCount, setHinderCount] = useState(0);

  // Set up the progress callback once on mount to receive updates from the server about hinder count and unlocks, which will update the UI as needs be.
  useEffect(() => {
  setOnProgressCallback((data) => {
  if (data.type === "progress") {
  setHinderCount(data.hinderCount);
  const unlock = data.unlock;
  if (unlock) {
  setUnlocks(prev =>
    prev.includes(unlock)
      ? prev
      : [...prev, unlock]
     );
    }
  }
  });
}, 
[]);

 function handleLogout() {
  // Always close menus immediately
  setHelpOpen(false);
  setHinderOpen(false);
  setAnchor(null);

  // Then logout immediately
  signOut();
  navigate("/login");
}
  // define menu items using useMemo so the dont get recreated every render, which would break the menu’s open state
const helpItems: RadialItem[] = useMemo(
    () => [
      { id: "heal", label: "Heal player", action: "help", effect: "heal_player", onClick: () => sendHelp("heal_player") },
      { id: "speed", label: "Speed up player", action: "help", effect: "speed_up_player", onClick: () => sendHelp("speed_up_player") },
      { id: "power", label: "Power boost", action: "help", effect: "power_boost", onClick: () => sendHelp("power_boost") },
      { id: "shield", label: "Shield player", action: "help", effect: "shield_player", onClick: () => sendHelp("shield_player") },
    ],
    []
  );
  const hinderItems: RadialItem[] = useMemo(
  () => [
    { id: "spawn", label: "Spawn enemy", action: "hinder", effect: "spawn_enemy", onClick: () => sendHinder("spawn_enemy") },
    { id: "trap",  label: "Drop trap", action: "hinder", effect: "drop_trap", onClick: () => sendHinder("drop_trap") },
    { id: "slow",  label: "Slow player", action: "hinder", effect: "slow_player", onClick: () => sendHinder("slow_player") },
    { id: "steal", label: "Steal power", action: "hinder", effect: "steal_power", onClick: () => sendHinder("steal_power") },

      // Unlock spawn brute hinder if it’s in the unlocks array from progress updates coming from the gameclient, which means required hinder amount has been reached 
    ...(unlocks.includes("spawn_brute")
      ? [{
          id: "brute",
          label: "Spawn Brute",
          action: "hinder" as const,
          effect: "spawn_brute" as const,
          onClick: () => sendHinder("spawn_brute")
        }]
      : [])
  ],
  [unlocks]
);
  
  return (
    <div className="app">
       <button
            className="logout-button"
            onClick={handleLogout}
           >
           Logout
       </button>
      <h1 className="wave-title" aria-label={title}>
             {title.split("").map((ch, i) => (
            <span
             key={i}
              className="wave-char"
              style={{ ["--i" as any]: i }}
              aria-hidden="true"
            >
           {ch === " " ? "\u00A0" : ch}
           </span>
          ))}
      </h1>
       <p>Hinders used: {hinderCount}</p>
      <div className="button-group">
        <div className="action">
          <img src={Bee} alt="Bee" className="character bee" />
           <button
            className="help"
            onClick={(e) => {
              const rect = (e.currentTarget as HTMLButtonElement).getBoundingClientRect();
              // menu pops from button center
              setAnchor({ x: rect.left + rect.width / 2, y: rect.top + rect.height / 2 });
              setHinderOpen(false); 
              setHelpOpen(true);
            }}
          >
            Help Player
          </button>
        </div>

        <div className="action">
          <img src={Boo} alt="Boo" className="character boo" />
          <button
            className="hinder"
            onClick={(e) => {
              const rect = (e.currentTarget as HTMLButtonElement).getBoundingClientRect();
              // menu pops from button center
              setAnchor({ x: rect.left + rect.width / 2, y: rect.top + rect.height / 2 });
               setHelpOpen(false); 
              setHinderOpen(true);
            }}
          >
            Hinder Player
          </button>
        </div>
      </div>
     <RadialMenu
       open={helpOpen}
       items={helpItems}
       anchor={anchor}
       onClose={() => setHelpOpen(false)}
       />
      <RadialMenu
        open={hinderOpen}
        items={hinderItems}
        anchor={anchor}
        onClose={() => setHinderOpen(false)}
      />
    </div>
  );
}

export default App;
