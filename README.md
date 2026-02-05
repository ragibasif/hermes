# hermes

A lightweight HTTP web server built from scratch using **Node.js** and **TypeScript**.

**For recreational and educational purposes only.**

## Prerequisites

* **Node.js** (v18 or higher recommended)
* **npm** (comes with Node)

## Setup Instructions

1. **Install Dependencies**
Run the following command to install TypeScript and the development tools:
```bash
npm install

```

2. **Project Structure**
* `src/index.ts`: The main server logic.
* `tsconfig.json`: TypeScript configuration.
* `package.json`: Project scripts and dependencies.


## How to Run

### Development Mode

To start the server with "live reload" (it will restart whenever you save a file):

```bash
npm run dev

```

### Production Build

To compile the code to JavaScript and run the standard way:

```bash
# Build the project
npx tsc

# Run the compiled code
node dist/index.js

```

## API Routes

Once the server is running at `http://localhost:3000`, you can visit:

* `/` - Returns a welcome message.
* `/about` - Returns info about the server.
* Any other URL - Returns a 404 error.

