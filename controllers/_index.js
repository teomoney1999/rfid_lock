import express from "express";
// ROUTER
import userRouter from "./user/user";
import rfidRouter from "./rfid/rfid";

const initApi = (app) => {
  app.use(express.urlencoded());
  app.use(express.json());

  // ROUTER
  app.use("/users", userRouter);
  app.use("/rfid", rfidRouter);
  
};

export default initApi;
