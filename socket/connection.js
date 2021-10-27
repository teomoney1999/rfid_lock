import { log } from "../helper/logging";

export const connectSocket = (socket, io, ...args) => {
  socket.emit("welcome", {
    message: "Welcome to Socket",
  });

  // socket.on("connection", (message) => {
  //   console.log(`[${new Date()}] Teomoney message: ${JSON.stringify(message)}`);
  //   // io.sockets.emit("connection", "SOCKET IS WORKING!!!");
  // });
  socket.on("connection", () => {
    log("CONNECTED");
  });

  socket.on("disconnect", () => {
    log("DISCONNECTED");
  });

  socket.on("error", (error) => {
    log(error);
  });
};
