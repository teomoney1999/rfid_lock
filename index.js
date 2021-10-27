import express from 'express';
import http from 'http';
import ip from 'ip';
import socketio from 'socket.io';
import cors from 'cors';
import Config from './config/index';
import initSocket from './socket/_index';
import initApi from './controllers/_index';
// import eMess from './config/error.json';
import db, {createTables} from './database/index';

// import User from './models/model';
// import { log } from 'console';

const app = express(); 
const server = http.createServer(app);
export const io = socketio(server);
const PORT = Config.SOCKET_PORT;

server.listen(PORT, () => {
  console.log(`[${new Date()}] Server running at: ${ip.address()}:${PORT}`);  
});

initSocket(io);

initApi(app);

// createTables(db);


