const express = require('express');
const  {addGPS, getGPS, editGPS } = require('../controllers/GPSController');
const router = express.Router();

router.post('/', addGPS);
router.get('/', getGPS);
router.put('/', editGPS);

module.exports = router;