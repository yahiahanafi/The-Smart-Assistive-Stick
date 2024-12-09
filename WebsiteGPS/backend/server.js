require('dotenv').config();
const express = require('express');
const gpsRoute = require('./routes/GPSRoute'); // Adjust the path as necessary
const mongoose = require('mongoose');
// Create an Express application
const app = express();

// Middleware to parse JSON bodies
app.use(express.json());




// Route definitions

app.use('/api/GPSRoute', gpsRoute);

// Connect to MongoDB
mongoose.connect(process.env.MONGO_URI)
  .then(() => {
    console.log('Connected to database');

    // Start the server and listen for requests
    app.listen(process.env.PORT, () => {
      console.log(`Listening for requests on port ${process.env.PORT}`);
    });
  })
  .catch((err) => {
    console.error('Database connection error:', err);
  });

// Error handling middleware
app.use((err, req, res, next) => {
  console.error('Error:', err.message);
  res.status(500).json({ error: 'Internal Server Error' });
});
