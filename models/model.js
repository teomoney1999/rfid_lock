// import { Model, DataTypes as types } from "sequelize";
import pkg from "sequelize";
import db from "../database/index";

const { Model, DataTypes: types } = pkg;

export class User extends Model {}
User.init(
  {
    id: { type: types.STRING, defaultValue: db.UUIDV4, primaryKey: true },
    username: types.STRING,
    password: types.STRING,
    salt: types.STRING,
  },
  { sequelize: db }
);

// const User = db.define(
//   "User",
//   {
//     id: { type: types.STRING, defaultValue: db.UUIDV4, primaryKey: true },
//     username: types.STRING,
//     password: types.STRING,
//     salt: types.STRING,
//   },
//   {}
// );

export class Rfid extends Model {}
Rfid.init(
  {
    id: { type: types.STRING, defaultValue: db.UUIDV4, primaryKey: true },
    card: types.STRING,
    current: { defaultValue: false, type: types.BOOLEAN },
  },
  { sequelize: db }
);

class Log extends Model {}
Log.init(
  {
    id: { type: types.STRING, defaultValue: db.UUIDV4, primaryKey: true },
    status: types.STRING, // open or close
    user: types.STRING,
  },
  { sequelize: db }
);

// Association
User.hasOne(Rfid, { foreignKey: "userId", type: types.STRING });
Rfid.belongsTo(User);

User.hasMany(Log, { foreignKey: "userId", type: types.STRING });
Log.belongsTo(User);

export default User;
