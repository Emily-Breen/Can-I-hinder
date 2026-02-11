export type RadialItem = {
  id: string;
  label: string;
  onClick: () => void;
  disabled?: boolean;
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