const express = require('express');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

const config = {
  port: 3000,
};

const recordSchema = new mongoose.Schema({
  timestamp: Number,
  temperature: Number,
});
const Record = mongoose.model('Record', recordSchema);

const app = express();

app.use(bodyParser.json());

app.get('/', (req, res) => {
  res.send('Hello world');
});

app.get('/data', (req, res) => {
  Record.find().then((data) => {
    res.status(200).send(JSON.stringify(data));
  });
});

app.post('/data', (req, res) => {
  const record = new Record({ timestamp: req.body.timestamp, temperature: req.body.temperature });
  record.save().then(() => {
    res.status(201).end();
  });
});
mongoose.connect('mongodb://localhost/test', {
  useNewUrlParser: true,
  useUnifiedTopology: true,
  user: 'root',
  pass: 'example',
  auth: {
    authSource: 'admin',
  },
}).then(() => {
  app.listen(config.port, () => {
    console.info(`Server launched on ${config.port}`);
  });
}).catch((err) => {
  console.info(err);
});
