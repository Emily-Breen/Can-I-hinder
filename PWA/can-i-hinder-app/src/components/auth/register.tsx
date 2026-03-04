import { useState } from "react";
import { useNavigate } from "react-router-dom";
import { register } from "./auth";
import "./styled.css";
// Validation for registration form
function validate(username: string, email: string, password: string): string[] {
  const errors: string[] = [];

  if (!username.trim()) errors.push("Username is required.");
  if (!email.trim()) errors.push("Email is required.");

  if (!/\S+@\S+\.\S+/.test(email))
    errors.push("Enter a valid email address.");

  if (password.length < 6)
    errors.push("Password must be at least 6 characters.");

  return errors;
}
// Registration component with form fields for username, email, and password, and client-side validation before making API call to register the user
export default function Register() {
  const navigate = useNavigate();
// Form fields
  const [username, setUsername] = useState("");
  const [password, setPassword] = useState("");
  const [email, setEmail] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
  const [showErrors, setShowErrors] = useState(false);
  const errors = validate(username, email, password);
  const canSubmit = errors.length === 0 && !loading;
// Handle form submission for registration
 async function handleSubmit(e: React.FormEvent) {
  e.preventDefault();
// Show validation errors if any, and clear previous API errors
  setShowErrors(true);
  setError(null);
// If validation fails, do not proceed with API call
  if (!canSubmit) return;
// Set loading state while making API call
  setLoading(true);
// Make API call to register the user, and on success, save the returned token and username to localStorage for session persistence, then navigate to the main app page. Handle any errors that occur during registration.
  try {
    await register(username.trim(), email.trim(), password);
    navigate("/", { replace: true });
  } catch (err: any) {
    setError(err?.message ?? "Registration failed");
  } finally {
    setLoading(false);
  }
}
// Render the registration form with fields for username, email, and password, and display validation errors or API errors as needed
  return (
    <div className="login-page">
      <div className="login-wrapper">
        <form className="login-card" onSubmit={handleSubmit}>
          <h1 className="login-title">Create Account</h1>

          <label className="login-label">
            Username
            <input
              className="login-input"
              value={username}
              onChange={(e) => setUsername(e.target.value)}
            />
          </label>
          <label className="login-label">
            Email
            <input
              className="login-input"
              type="email"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              placeholder="you@example.com"
            />
          </label>
          <label className="login-label">
            Password
            <input
              className="login-input"
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
            />
          </label>

          {error && <div className="login-error">{error}</div>}
          {showErrors && errors.length > 0 && (
            <div className="login-error">
              {errors.map((msg) => (
                <div key={msg}>• {msg}</div>
              ))}
            </div>
          )}
          <button className="login-button" type="submit" disabled={!canSubmit}>
           {loading ? "Creating account..." : "Register"}
          </button>
        </form>
      </div>
    </div>
  );
}