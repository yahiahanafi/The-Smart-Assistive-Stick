
const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const GPSSchema = new Schema({
  Time: {
    type: String,
    required: true
  },
  Latitude: {
    type: String,
    required: true
  },
  Longitude :{
    type:String,
    required: true
  },
}, { timestamps: true });

const GPS = mongoose.model('GPS', GPSSchema);
module.exports = GPS;
