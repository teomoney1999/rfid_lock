export const rfidSocket = (socket, io, ...args) => {
  socket.on("rfid-code", (info) => {
    // io.emit("led-red", {"ledRed": code});
    console.log(`[${new Date()}] ${JSON.stringify(info)}`);
    const rfidCode = info.substring('{"code":"'.length, info.length - 2);

    console.log(rfidCode);
    // query to database

    let result = false;
    if (rfidCode === "01 02 03 04 05 06 07 08 09 0a ff 0b 0c 0d 0e 0f") {
      result = true;
    }

    socket.emit("auth", { isAuth: result });
    console.log(`[${new Date()}] auth event sent. Result ${result}!`);
  });
};
