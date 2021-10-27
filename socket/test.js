export const testSocket = (socket, io, ...args) => {
  socket.on("atime", (data) => {
    let json = {
      name: "teomoney",
      age: 22,
      time: new Date(),
    };
    io.emit("atime", JSON.stringify(json));
    // socket.emit("atime", JSON.stringify(json));
    console.log(`[${new Date()}] Timing data: ${JSON.stringify(data)}`);
  });

  let led = [true, false];

  let interval1 = setInterval(() => {
    led = led.map(val => !val);

    let json = {"led": led};

    socket.emit("LED", json);
    console.log(`[${new Date()}] Led json sent`);

  }, 2000);

  socket.on("disconnect", () => {
    console.log(`[${new Date()}] Socket disconnected!`);
    clearInterval(interval1);
  });
};
