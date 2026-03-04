import { useState } from "react";
import { API_BASE } from "../../config";
import "./styled.css";

// Component for handling forgot password functionality, allowing users to request a password reset email by submitting their email address, and displaying a message confirming the request was sent
export default function ForgotPassword() {
  const [email, setEmail] = useState("");
  const [message, setMessage] = useState("");
// Handle form submission for forgot password
  async function handleSubmit(e: React.FormEvent) {
    e.preventDefault();
// Make API call to request password reset email, sending the provided email address in the request body
    await fetch(`${API_BASE}/api/auth/forgot-password`, {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify(email),
    });
// Regardless of success or failure, show a generic message to prevent email enumeration
    setMessage("If an account exists, a reset email has been sent.");
  }
  // Render the forgot password form with an input for the email address and a submit button, and display a message confirming the request was sent
  return (
    <div className="login-page">
      <div className="login-wrapper">
        <h1 className="login-brand">Can I Hinder?</h1>

        <form className="login-card" onSubmit={handleSubmit}>
          <h1 className="login-title">Reset Password</h1>
          <p className="login-sub">
            Enter your email and a reset link will be sent.
          </p>

          <label className="login-label">
            Email
            <input
              className="login-input"
              type="email"
              placeholder="you@example.com"
              value={email}
              onChange={(e) => setEmail(e.target.value)}
              required
            />
          </label>

          <button className="login-button" type="submit">
            Send reset link
          </button>

          {message && (
            <div className="login-note">{message}</div>
          )}
        </form>
      </div>
    </div>
  );
}