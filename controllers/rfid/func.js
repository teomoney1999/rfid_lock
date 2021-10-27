import pkg from "sequelize";
const { Op } = pkg;

import { Rfid as Model } from "../../models/model";
// HELPER
import { generateUUID } from "../../helper/default";
import { errorLog } from "../../helper/logging";

export const getAll = async (req, res, next) => {
  const users = await Model.findAll();
  req.data = users;
  next();
};

export const getOne = async (req, res, next) => {
  const id = req.params.id;
  const data = await Model.findAll({ where: { id: id } });
  req.data = data;
  next();
};

export const post = async (req, res, next) => {
  const body = req.body;
  body["id"] = generateUUID();

  let data = {};
  try {
    data = await Model.create(body);
    req.data = data;
  } catch (error) {
    req.data = null;
    errorLog(error);
  }

  next();
};

export const deleteOne = async (req, res, next) => {
  const id = req.params.id;

  try {
    await Model.destroy({ where: { id: id } });
  } catch (error) {
    req.error = error;
    errorLog(error);
  }

  next();
};
