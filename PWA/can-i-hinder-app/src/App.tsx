import { useMemo, useState } from "react";
import { sendHelp, sendHinder } from "./components/pwa/pwa-client";
import RadialMenu from "./components/radial-menu/radial-menu";
import type { RadialItem } from "./components/radial-menu/types";
import "./global.css";
import Boo from "./assets/Boo.png";
import Bee from "./assets/Bee.png";
const title = "Choose your path";
function App() {
  //state for which menu is open and where it’s anchored (null if closed)
  const [hinderOpen, setHinderOpen] = useState(false);
  const [helpOpen, setHelpOpen] = useState(false);
  const [anchor, setAnchor] = useState<{ x: number; y: number } | null>(null);
  // define menu items using useMemo so the dont get recreated every render, which would break the menu’s open state
const helpItems: RadialItem[] = useMemo(
    () => [
      { id: "heal", label: "Heal player", onClick: () => sendHelp("heal_player") },
      { id: "speed", label: "Speed up player", onClick: () => sendHelp("speed_up_player") },
      { id: "power", label: "Power boost", onClick: () => sendHelp("power_boost") },
      { id: "shield", label: "Shield player", onClick: () => sendHelp("shield_player") },
    ],
    []
  );
  const hinderItems: RadialItem[] = useMemo(
    () => [
      { id: "spawn", label: "Spawn enemy", onClick: () => sendHinder("spawn_enemy") },
      { id: "trap",  label: "Drop trap",   onClick: () => sendHinder("drop_trap") },
      { id: "slow",  label: "Slow player", onClick: () => sendHinder("slow_player") },
      { id: "steal", label: "Steal power", onClick: () => sendHinder("steal_power") },
    ],
    []
  );
  return (
    <div className="app">
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
