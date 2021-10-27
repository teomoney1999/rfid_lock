import Config from "../config/index";
import { Sequelize } from "sequelize";
import eMess from '../config/error.json';

const db = new Sequelize(Config.SQLALCHEMY_DATABASE_URI, {
  logging: (...msg) => console.log(`[${new Date()}] Sequalize logging: ${msg}`),
  define: {
    freezeTableName: true, // table name = model name
  },
});

export const createTables = async (sequelize) => {
  try {
    await sequelize.sync();
    console.log('Model created!');
  } catch (error) {
    console.error(eMess.DB_NOT_SYNC, error);
  }
}

const connectionCheck = async (sequelize) => {
  try {
    await sequelize.authenticate();
    console.log('Connection to database has been established successfully.');
  } catch (error) {
    console.error(eMess.DB_NOT_CONNECTED, error);
  }
}

export default db;
