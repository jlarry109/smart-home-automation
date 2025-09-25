#!/bin/bash

# Smart Home Development Startup Script

echo "🏠 Starting Smart Home Development Environment..."

# Check if Node.js is installed
if ! command -v node &> /dev/null; then
    echo "❌ Node.js is not installed. Please install Node.js first."
    exit 1
fi

# Start backend in background
echo "🔧 Starting Node.js backend..."
cd backend
if [ ! -f ".env" ]; then
    echo "⚠️  No .env file found. Copying from .env.example..."
    cp .env.example .env
    echo "📝 Please edit backend/.env with your AWS credentials and IoT endpoint"
fi

npm install > /dev/null 2>&1
npm run dev &
BACKEND_PID=$!
cd ..

# Wait for backend to start
sleep 3

# Start React frontend
echo "⚛️  Starting React frontend..."
cd react-frontend
npm install > /dev/null 2>&1
npm run dev &
FRONTEND_PID=$!
cd ..

echo ""
echo "✅ Development environment started!"
echo "📊 Frontend: http://localhost:5173"
echo "🔌 Backend:  http://localhost:3001"
echo ""
echo "Press Ctrl+C to stop all services"

# Wait for user interrupt
trap "echo '🛑 Stopping services...'; kill $BACKEND_PID $FRONTEND_PID; exit" INT
wait