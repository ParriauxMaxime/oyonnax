const express = require('express');

const config = {
  port: 3000,
};

const app = express();

app.get('/', (req, res) => {
  res.send("Hello world");
});

app.listen(config.port, () => {
  console.info(`Server launched on ${config.port}`);
});