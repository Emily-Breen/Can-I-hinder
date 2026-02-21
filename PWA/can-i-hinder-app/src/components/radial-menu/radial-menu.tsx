import { useEffect, useState } from "react";
import "./styled.css";
import type { RadialMenuProps } from "./types";
import { getActionStatus } from "../pwa/rate-limit";

export default function RadialMenu({ open, items, anchor, onClose }: RadialMenuProps) {
  // Close the menu when user presses ESC
  useEffect(() => {
    if (!open) return;

    function handleKeyDown(e: KeyboardEvent) {
      if (e.key === "Escape") onClose();
    }

    window.addEventListener("keydown", handleKeyDown);
    return () => window.removeEventListener("keydown", handleKeyDown);
  }, [open, onClose]);
 //Ticker to update buttons during cooldown without closing the menu
  const [, setTick] = useState(0);
  useEffect(() => {
    if (!open) return;
    const id = window.setInterval(() => setTick((t) => t + 1), 250);
    return () => window.clearInterval(id);
  }, [open]);

  // Don’t render anything if menu is closed as it doesn’t know where to place it
  if (!open || !anchor) return null;

    // Detect mobile every render 
    //menu resizing
  const isMobile = window.innerWidth <= 768;
  const menuSize = isMobile ? 420 : 700;             // big circle diameter depending on screen size
  const itemSize = isMobile ? 120 : 200;             // each button diameter
  const itemRingRadius = isMobile ? 130 : 200;       // distance from center to each button
  // Mobile always center screen 
  // Desktop centered
 const centerX = window.innerWidth / 2;
 const centerY = window.innerHeight / 2;

  // Compute where each button sits around the circle
  const placedItems = items.map((item, index) => {
    // angle goes around the circle, starting at the top looks nicer
    const angle = (index / items.length) * Math.PI * 2 - Math.PI / 2;

    return {
      item,
      tx: Math.cos(angle) * itemRingRadius,
      ty: Math.sin(angle) * itemRingRadius,
    };
  });

  // Type helper so TS accepts CSS variables in the style prop
  const cssVars: React.CSSProperties & Record<string, string> = {
    "--radial-size": `${menuSize}px`,
    "--item-size": `${itemSize}px`,
  };

  return (
    <div className="radial-overlay" onMouseDown={onClose}>
      <div
        className="radial-circle"
        // Position the menu based on the anchor (button center) for desktop, or screen center for mobile
        style={{ ...cssVars, left: centerX, top: centerY }}
        onMouseDown={(e) => e.stopPropagation()}
      >
        {placedItems.map(({ item, tx, ty }) => {
         
         // Check cooldown status for this item if it has an associated action/effect, used to disable buttons and show cooldown timers in the UI
         const status =
         item.action && item.effect ? getActionStatus(item.action, item.effect) : null;
         // If the item is on cooldown, calculate how much time is left to show in the tooltip and cooldown timer
         const blockedByCooldown = status ? !status.canSend : false;
           // Calculate seconds left for cooldown timer display
          const secondsLeft = status ? Math.ceil(status.cooldownMsLeft / 1000) : 0;
          // An item is disabled if it’s explicitly marked as disabled or if it’s currently blocked by cooldown
          const isDisabled = Boolean(item.disabled) || blockedByCooldown;

          return (
            <button
              key={item.id}
              className="radial-item"
              // Disable the button if it’s on cooldown or explicitly disabled, and show a tooltip with cooldown time if blocked by cooldown
              disabled={isDisabled}
              style={
                {
                  "--tx": `${tx}px`,
                  "--ty": `${ty}px`,
                  transform: `translate(${tx}px, ${ty}px)`,
                } as React.CSSProperties & Record<string, string>
              }
              onClick={() => {
                if (isDisabled) return;
                item.onClick();
                onClose();
              }}
              title={blockedByCooldown ? `Wait ${secondsLeft}s` : undefined}
            >
              <div style={{ display: "flex", flexDirection: "column", gap: 6 }}>
                <span>{item.label}</span>
                {blockedByCooldown && (
                  <span style={{ fontSize: 12, opacity: 0.85 }}>
                    {secondsLeft}s
                  </span>
                )}
              </div>
            </button>
          );
        })}

        <button className="radial-center" onClick={onClose} />
      </div>
    </div>
  );
}
