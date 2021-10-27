import { rfidSocket } from "./rfid";
import { ledSocket } from "./led";
import { connectSocket } from "./connection"; 
import { testSocket } from "./test";

const initSocket = (io) => {
  io.on("connection", (socket) => {
    console.log(`[${new Date()}] CONNECTED`);
    connectSocket(socket, io);
    
    ledSocket(socket, io);

    rfidSocket(socket, io);
  });
};


export default initSocket;