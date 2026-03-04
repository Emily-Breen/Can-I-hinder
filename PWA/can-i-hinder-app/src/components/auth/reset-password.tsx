import { useState } from "react";
import { useSearchParams, useNavigate } from "react-router-dom";
import { API_BASE } from "../../config";
import "./styled.css";
// Validation for reset password form
export default function ResetPassword() {
  const [searchParams] = useSearchParams();
  const navigate = useNavigate();
// Get token from URL query parameters
  const token = searchParams.get("token");
// Form fields
  const [password, setPassword] = useState("");
  const [confirm, setConfirm] = useState("");
  const [error, setError] = useState<string | null>(null);
  const [loading, setLoading] = useState(false);
// Handle form submission for password reset
  async function handleSubmit(e: React.FormEvent) {
    e.preventDefault();

    if (!password || password.length < 6) {
      setError("Password must be at least 6 characters.");
      return;
    }
// Check if password and confirm password match
    if (password !== confirm) {
      setError("Passwords do not match.");
      return;
    }
// Clear previous errors and set loading state
    setLoading(true);
    setError(null);
// Make API call to reset password using the token and new password
    try {
      const res = await fetch(`${API_BASE}/api/auth/reset-password`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({
          token,
          newPassword: password
        })
      });
// Handle API response
      if (!res.ok) {
        throw new Error("Password reset failed.");
      }
// On success, alert the user and navigate to login page
      alert("Password successfully reset!");
      navigate("/login");

    } catch (err: any) {
      setError(err?.message ?? "Something went wrong.");
    } finally {
      setLoading(false);
    }
  }
// Render the reset password form
  return (
    <div className="login-page">
      <div className="login-wrapper">

        <form className="login-card" onSubmit={handleSubmit}>
          <h1 className="login-title">Reset Password</h1>

          <label className="login-label">
            New Password
            <input
              className="login-input"
              type="password"
              value={password}
              onChange={(e) => setPassword(e.target.value)}
            />
          </label>

          <label className="login-label">
            Confirm Password
            <input
              className="login-input"
              type="password"
              value={confirm}
              onChange={(e) => setConfirm(e.target.value)}
            />
          </label>

          {error && <div className="login-error">{error}</div>}

          <button className="login-button" disabled={loading}>
            {loading ? "Updating..." : "Reset Password"}
          </button>
        </form>
      </div>
    </div>
  );
}