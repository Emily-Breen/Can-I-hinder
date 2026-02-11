import { useState } from "react";
import { useNavigate } from "react-router-dom";
import { signIn } from "./auth";
import "./styled.css";
//validation for login form, returns array of error messages (empty if no errors)
function validate(username: string, password: string): string[] {
  const errors: string[] = [];

  if (!username.trim()) 
    errors.push("Username is required.");
  if (password.length < 4) 
    errors.push("Password must be at least 4 characters.");

  return errors;
}
const title = "Can I Hinder?";
export default function Login() {
  const navigate = useNavigate();

  // form fields
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");

  // UI state
  const [showErrors, setShowErrors] = useState(false);
  const [isLoading, setIsLoading] = useState(false);

  const errors = validate(username, password);
  const canSubmit = errors.length === 0 && !isLoading;
 // Handle form submission
  async function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
    setShowErrors(true);

    if (!canSubmit) return;

    setIsLoading(true);

    // fake login delay
    await new Promise((r) => setTimeout(r, 500));

    signIn(username.trim());
    navigate("/", { replace: true });
  }

  return (
    <div className="login-page">
      <div className="login-wrapper">
        
       <h1 className="login-brand wave" aria-label={title}>
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
      <form className="login-card" onSubmit={handleSubmit}>
        <h1 className="login-title">Welcome!</h1>
        <p className="login-sub">Sign in to continue</p>

        <label className="login-label">
          Username
          <input
            className="login-input"
            value={username}
            onChange={(e) => setUsername(e.target.value)}
            placeholder="e.g. JaneDoe123"
          />
        </label>

        <label className="login-label">
          Password
          <input
            className="login-input"
            type="password"
            value={password}
            onChange={(e) => setPassword(e.target.value)}
            placeholder="••••"
          />
        </label>

        {showErrors && errors.length > 0 && (
          <div className="login-error">
            {errors.map((msg) => (
              <div key={msg}>• {msg}</div>
            ))}
          </div>
        )}

        <button className="login-button" type="submit" disabled={!canSubmit}>
          {isLoading ? "Signing in…" : "Sign in"}
        </button>

        <div className="login-footer">
          <label className="login-remember">
            <input type="checkbox" defaultChecked />
            Remember me
          </label>
          <span className="login-forgot">Forgot password?</span>
        </div>

        <div className="login-note">Demo login</div>
      </form>
      </div>
    </div>
  );
}
