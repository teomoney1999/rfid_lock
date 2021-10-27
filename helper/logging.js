export const apiLog = (req, res) => console.log(`[${new Date()}] [Status ${res.statusCode}] ${req.method} :${req.baseUrl}/${!req.params.id ? '' : req.params.id}`);

export const errorLog = (error) => console.log(`[${new Date()}] ${error}`);

export const log = (message) => console.log(`[${new Date()}] ${message}`);

export const errorResponse = (errorMessage, errorCode) => {errorMessage, errorCode};