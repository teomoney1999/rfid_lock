export const ledSocket = (socket, io, ...args) => {
  socket.on("led-status", (status) => {
    console.log(`[${new Date()}] Led status: ${JSON.stringify(status)}`);
  });

  socket.on("led-blue", (ledStatus) => {
    console.log(`[${new Date()}] Led blue`, ledStatus ? "on" : "off");
    io.emit("led-blue", { ledBlue: ledStatus });
  });

  socket.on("led-red", (ledStatus) => {
    console.log(`[${new Date()}] Led red`, ledStatus ? "on" : "off");
    io.emit("led-red", { ledRed: ledStatus });
  });
};
