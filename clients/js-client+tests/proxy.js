const http = require('http');
const net = require('net');

const HTTP_PORT = 3000;     // Порт для HTTP-прокси
const CPP_SERVER_PORT = 8080; // Порт вашего C++ сервера

// Создаем HTTP-сервер
const server = http.createServer((req, res) => {
    let body = '';

    req.on('data', chunk => {
        body += chunk;
    });

    req.on('end', () => {
        // Создаем сокет для подключения к C++ серверу
        const client = new net.Socket();
        client.connect(CPP_SERVER_PORT, '127.0.0.1', () => {
            console.log(`Connected to C++ server: ${body}`);
            client.write(body);
        });

        client.on('data', data => {
            res.writeHead(200, { 'Content-Type': 'text/plain', 'Access-Control-Allow-Origin': '*' });
            res.end(data.toString());
            client.end();
        });

        client.on('error', err => {
            console.error('Error connecting to C++ server:', err.message);
            res.writeHead(500, { 'Content-Type': 'text/plain', 'Access-Control-Allow-Origin': '*' });
            res.end('Error communicating with C++ server');
        });
    });
});

server.listen(HTTP_PORT, () => {
    console.log(`Proxy server running at http://localhost:${HTTP_PORT}/`);
});
