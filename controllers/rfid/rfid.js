import express from "express";
// HELPER
import { apiLog, errorResponse } from "../../helper/logging";
// FUNC
import { post, getAll, getOne, deleteOne } from "./func";

const router = express.Router();

router.post("/", post, (req, res) => {
  const response = req.data;
  if (!response) {
    return res
      .status(400)
      .json(errorResponse("Bad request!", "BAD_REQUEST_ERROR"));
  }
  res.status(201).json(response);
  apiLog(req, res);
});

router.get("/", getAll, (req, res) => {
  const response = { results: req.data };
  res.status(200).json(response);
  apiLog(req, res);
});

router.get("/:id", getOne, (req, res) => {
  const response = req.data;
  if (!response || !response.length) {
    return res
      .status(200)
      .json(errorResponse("No result found!", "NO_RESULT_FOUND"));
  }
  res.status(200).json(response);
  apiLog(req, res);
});

router.patch("/:id", (req, res) => {
  res.send("This is rfid api!");
  apiLog(req, res);
});

router.delete("/:id", deleteOne, (req, res) => {
  // const response = req/
  if (!req.error) {
    res.status(200).json([]);
    apiLog(req, res);
  } else {
    return res
      .status(400)
      .json(errorResponse("Bad request!", "BAD_REQUEST_ERROR"));
  }
});

export default router;
