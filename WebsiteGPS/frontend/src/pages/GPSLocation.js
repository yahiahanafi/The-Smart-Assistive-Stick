import React, { useEffect, useState } from 'react';
import axios from 'axios';
import { GoogleMap, LoadScript, Marker } from '@react-google-maps/api';

const GPSDisplay = () => {
  const [gpsData, setGpsData] = useState(null);

  useEffect(() => {
    const fetchGPS = async () => {
      try {
        const response = await axios.get('/api/GPSRoute');
        const { Latitude, Longitude, Time } = response.data;
        if (Latitude && Longitude) {
          setGpsData({ lat: Number(Latitude), lng: Number(Longitude), Time });
        } else {
          console.error('Invalid GPS data:', response.data);
        }
      } catch (error) {
        console.error('Error fetching GPS data:', error);
      }
    };

    fetchGPS();
  }, []);

  const mapContainerStyle = {
    width: '100%',
    height: '90vh',
  };

  return (
    <div>
      {gpsData ? (
        <>
          <LoadScript googleMapsApiKey={"AIzaSyCCRnucTA5OUTpb-NYZkImovLsYLoWsjBs"}>
            <GoogleMap
              mapContainerStyle={mapContainerStyle}
              center={{ lat: gpsData.lat, lng: gpsData.lng }}
              zoom={15}
            >
              <Marker position={{ lat: gpsData.lat, lng: gpsData.lng }} />
            </GoogleMap>
          </LoadScript>
          <p>Time: {gpsData.Time}</p>
        </>
      ) : (
        <div>Loading...</div>
      )}
    </div>
  );
};

export default GPSDisplay;
