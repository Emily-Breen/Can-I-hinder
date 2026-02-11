import { defineConfig } from 'vite'
import react from '@vitejs/plugin-react'
import { VitePWA } from "vite-plugin-pwa";


export default defineConfig({
  base: "/",
  plugins: [react(), 
    VitePWA({
      registerType: "autoUpdate", // Automatically update the service worker when a new version is available
      includeAssets: ["favicon.ico", "apple-touch-icon.png", "maskable-icon.svg"], // Include additional assets in the PWA
      manifest: { // Defines the web app manifest
        name: "Can I Hinder?",
        short_name: "CanIHinder?",
        description: "A interface to directly influence a gamers experience",
        start_url: "/",
        scope: "/",
        display: "standalone",
        background_color: "#ffffff",
        theme_color: "#0ea5e9",
        icons: [ {
          src: "pwa-192x192.png",
          sizes: "192x192",
          type: "image/png"
        }, {
          src: "pwa-512x512.png",
          sizes: "512x512",
          type: "image/png"
        },
        {
          src: "/maskable-icon.png",
          sizes: "512x512",
          type: "image/png",
          purpose: "maskable"
         },
        ],
        },
        workbox: {
          navigateFallback: "/index.html", // Fallback to index.html for single page application routing 
          runtimeCaching: [
            {
              urlPattern: ({request}) => request.destination === "document", // Cache HTML documents
              handler: "NetworkFirst",
              options: {
                cacheName: "pages",
              }
            },
              {
                urlPattern: ({request}) => ["style", "script", "worker"].includes(request.destination), // Cache CSS, JS, and Worker files
                handler: "StaleWhileRevalidate",
                options: {
                  cacheName: "assets",
              }
            },
            {
              urlPattern: ({request}) => request.destination === "image",
              handler: "CacheFirst",
              options: {
                cacheName: "images",
                expiration: {
                  maxEntries: 100, // Maximum number of images to cache
                  maxAgeSeconds: 30 * 24 * 60 * 60, // 30 days
                },
            },
          },
        ],
      },
      
      devOptions: {
        enabled: process.env.NODE_ENV === "development",
        type: "module",
      },
    }),
  ],
});