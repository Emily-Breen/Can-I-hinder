import React from "react";
import { Navigate } from "react-router-dom";
import { isAuthed } from "./auth";

//if not autehenticated, navigate to login, else render children
export function ProtectedRoute({ children }: { children: React.ReactNode }) {
  if (!isAuthed()) 
    return <Navigate to="/login" replace />;
  return <>{children}</>;
}