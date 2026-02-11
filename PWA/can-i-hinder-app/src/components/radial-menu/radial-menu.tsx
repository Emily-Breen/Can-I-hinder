import { useEffect } from "react";
import "./styled.css";
import type { RadialMenuProps } from "./types";


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
    // Clicking outside the menu closes it
    <div className="radial-overlay" onMouseDown={onClose}>
      <div
        className="radial-circle"
        style={{
          ...cssVars,
          left: centerX,
          top: centerY,
        }}
        // Stop clicks inside the menu from closing it
        onMouseDown={(e) => e.stopPropagation()}
      >
        {placedItems.map(({ item, tx, ty }) => (
          <button
            key={item.id}
            className="radial-item"
            disabled={item.disabled}
            style={
              {
                "--tx": `${tx}px`,
                "--ty": `${ty}px`,
                transform: `translate(${tx}px, ${ty}px)`,
              } as React.CSSProperties & Record<string, string>
            }
            onClick={() => {
              if (item.disabled) return;
              item.onClick();
              onClose(); // close after choosing an action
            }}
          >
            {item.label}
          </button>
        ))}
        <button className="radial-center" onClick={onClose} />
      </div>
    </div>
  );
}
