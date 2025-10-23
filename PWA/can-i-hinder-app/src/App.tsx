import React from "react";
import { sendHelp, sendHinder } from "./components/pwa-client";
import "./App.css";
import Boo from "./assets/Boo.png";
import Bee from "./assets/Bee.png";

function App() {
  return (
    <div className="app">
      <h1>Can I Hinder?</h1>
      <p>Control the player’s fate in real-time!</p>

      <div className="button-group">
        {/* Help section */}
        <div className="action">
          <img src={Bee} alt="Bee" className="character bee" />
          <button className="help" onClick={sendHelp}>Help Player</button>
        </div>

        {/* Hinder section */}
        <div className="action">
         <img src={Boo} alt="Boo" className="character boo" />
          <button className="hinder" onClick={sendHinder}>Hinder Player</button>
        </div>
      </div>

      <p className="status">Connected via WebSocket...</p>
    </div>
  );
}

export default App;