import { createServer, IncomingMessage, ServerResponse } from 'node:http';

const PORT = 3000;

const server = createServer((req: IncomingMessage, res: ServerResponse) => {
  // 1. Set the response header (status code and content type)
  res.writeHead(200, { 'Content-Type': 'text/plain' });

  // 2. Handle basic routing
  if (req.url === '/') {
    res.end('Welcome to the Home Page!');
  } else if (req.url === '/about') {
    res.end('This is a custom HTTP server built with TypeScript.');
  } else {
    res.writeHead(404);
    res.end('Page Not Found');
  }
});

server.listen(PORT, () => {
  console.log(`Server is running at http://localhost:${PORT}`);
});
