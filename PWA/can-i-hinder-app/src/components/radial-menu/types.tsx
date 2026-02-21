import type { HelpEffect, HinderEffect } from "../pwa/types";

export type RadialItem = {
  id: string;
  label: string;
  onClick: () => void;
  disabled?: boolean;
  action?: "help" | "hinder"; 
  effect?: HelpEffect | HinderEffect;
};

 type Point = { 
  x: number; 
  y: number 
};

export type RadialMenuProps = {
  open: boolean;
  items: RadialItem[];
  anchor: Point | null; // where the menu pops up (button center)
  onClose: () => void;
};