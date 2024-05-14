// Complete Project Details at: https://RandomNerdTutorials.com/

// Database Paths
var dataTempPath = 'Hodnoty/teplota';
var dataHumiPath = 'Hodnoty/vlhkost';

// Get a database reference 
const databaseTemperature = database.ref(dataTempPath);
const databaseHumidity = database.ref(dataHumiPath);

// Variables to save database current values
var tempReading = 0;
var humReading = 0;

// Attach an asynchronous callback to read the data
databaseHumidity.on('value', (snapshot) => {
  humReading = snapshot.val(); // Assign the retrieved value to humReading
  if (humReading !== null) {
    console.log(humReading);
    document.getElementById("reading-float").innerHTML = humReading;
  }
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});

databaseTemperature.on('value', (snapshot) => {
  tempReading = snapshot.val(); // Assign the retrieved value to tempReading
  if (tempReading !== null) {
    console.log(tempReading);
    document.getElementById("reading-int").innerHTML = tempReading;
  }
}, (errorObject) => {
  console.log('The read failed: ' + errorObject.name);
});