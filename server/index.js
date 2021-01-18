const path = require('path');
const fs = require('fs');
const express = require('express');
const cors = require('cors');
const mongoose = require('mongoose');
const bodyParser = require('body-parser');

const config = {
  port: 3000,
};

const isProd = () => process.env.NODE_ENV === 'production';

const recordSchema = new mongoose.Schema({
  timestamp: Number,
  temperature: Number,
});
const Record = mongoose.model('Record', recordSchema);

const app = express();

app.use(bodyParser.json());
app.use(cors());

let cache = null;

app.get('/', (req, res) => {
  if (!cache) {
    cache = fs.readFileSync(path.join(__dirname, '..', 'client', 'build', 'index.html'), 'utf-8');
    cache = cache.replace('__INTERNAL_HOSTNAME__', `"${isProd() ? '18.205.246.58' : 'localhost'}"`);
  }

  return res.send(cache);
});

app.use(express.static(path.join(__dirname, '..', 'client', 'build')));

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

app.post('/reset', (req, res) => {
  Record.deleteMany({}).then(() => {
    res.status(203).end();
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
