const GPSModel = require('../models/GPSModel');

const addGPS = async (req, res) => {
    const { Time, Latitude,Longitude } = req.body;
    try {
    
        // Create the tourist with the hashed password
        const GPS = await GPSModel.create({ 
            Time, Latitude,Longitude });
        
        res.status(201).json(GPS); // Return the created tourist
      } catch (error) {
        res.status(400).json({ error: error.message });
      }
};
const getGPS = async (req, res) => {
    try {
        const GPS = await GPSModel.findOne(); // Find the first document in the collection
        if (!GPS) {
            return res.status(404).json({ message: 'GPS data not found' });
        }
        res.status(200).json(GPS); // Return the GPS data
    } catch (error) {
        res.status(400).json({ error: error.message });
    }
};
const getGPSID = async () => {
    try {
        const GPS = await GPSModel.findOne(); // Find the first document in the collection
        if (!GPS) {
            throw new Error('GPS data not found'); // Throw an error if no data is found
        }
        return GPS._id; // Return the id of the GPS data
    } catch (error) {
        throw new Error(error.message); // Catch and throw the error
    }
};

const editGPS = async (req, res) => {
    try {
        const id = await getGPSID(); // Get the ID from the getGPSID function
        const { Time, Latitude, Longitude } = req.body; // Get updated values from the request body

        // Find the GPS record by ID and update it
        const GPS = await GPSModel.findByIdAndUpdate(
            id,
            { Time, Latitude, Longitude }, // New values to update
            { new: true } // Return the updated document
        );

        if (!GPS) {
            return res.status(404).json({ message: 'GPS data not found' });
        }

        res.status(200).json(GPS); // Return the updated GPS data
    } catch (error) {
        res.status(400).json({ error: error.message });
    }
};

module.exports = { addGPS, getGPS, editGPS };
  

