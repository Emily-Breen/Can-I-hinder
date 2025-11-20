import React from "react";
import { sendHelp, sendHinder } from "./pwa-client";

const PwaControls: React.FC = () => {
  return (
    <div className="controls">
      <h2>Can I Hinder?</h2>
      <button onClick={sendHelp}>  Help Player</button>
      <button onClick={sendHinder}>Hinder Player</button>
    </div>
  );
};

export default PwaControls;