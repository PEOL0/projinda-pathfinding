console.log("JS scriptad");
const socket = new WebSocket('ws://localhost:8080');

function createMap() {
    const selectedArea = document.getElementById('areaSelect').value;
    if (selectedArea) {
      const encodedArea = encodeURIComponent(selectedArea);
      window.location.href = `karta.html?area=${encodedArea}`;
    } else {
      alert('Du har inte valt ett område!');
    }
}

function backButton() {
    window.location.href = `index.html`;
}


function getQueryParam(name) {
    const params = new URLSearchParams(window.location.search);
    return params.get(name);
}

// Get area from URL
const area = getQueryParam("area");

// Backgrounds - can be colors or image URLs
const backgrounds = {
    "SKOG nr 1": "url('assets/bergaKarta.png')"
}

const bg = backgrounds[area];
if (bg) {
    document.body.style.backgroundImage = bg;
} else {
    //document.body.style.backgroundColor = "#222"; // fallback color
}

// Show area name in the center
//document.body.textContent = area ? `Welcome to ${area}` : "No area selected";*/


const klickArea = document.getElementById('klickArea');
let waypoints = [];

klickArea.addEventListener('click', function(event) {
    if (event.target.tagName === 'BUTTON' || event.target.closest('button')) {
        return; 
    }
    
    const rect = klickArea.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    console.log(`Klick på x: ${x}, y: ${y}`);

    // Skapa en liten markör där man klickar
    const marker = document.createElement('div');
    marker.className = 'marker';
    marker.style.left = x + 'px';
    marker.style.top = y + 'px';
    klickArea.appendChild(marker);
    
    waypoints.push([x, y]);
    
    console.log(`Added waypoint ${waypoints.length}: ${x}, ${y}`);
});

function createRoute() {
    if (waypoints.length < 2) {
        alert('Please select at least 2 points for a route');
        return;
    }
    
    console.log(`Creating route with ${waypoints.length} waypoints`);
    
    let coords = [];
    for (let i = 0; i < waypoints.length; i++) {
        coords.push(Math.round(waypoints[i][0]/2));
        coords.push(Math.round(waypoints[i][1]/2));
    }
    
    let message = coords.join(',');
    console.log(`Sending coordinates: ${message}`);
    
    socket.send(message);
}

function clearRoute() {
    const markers = document.querySelectorAll('.marker, .markerVag');
    markers.forEach(marker => marker.remove());
    waypoints = []; 
    console.log('Route cleared');
}

document.addEventListener('keydown', function(event) {
    if (event.key === 'c' || event.key === 'C') {
        clearRoute();
    }
});

function rita(x, y) {
    const markerVag = document.createElement('div');
    markerVag.className = 'markerVag';
    markerVag.style.left = x + 'px';
    markerVag.style.top = y + 'px';
    klickArea.appendChild(markerVag);
}

socket.onopen = function () {
    console.log("Socket connected");
}

function processAndDraw(input) {
    const pathMarkers = document.querySelectorAll('.markerVag');
    pathMarkers.forEach(marker => marker.remove());
    
    let points = input.split(";"); 
  
    while (points.length > 0) {
        let pair = points.shift(); 
        if (!pair) continue;
  
        let [xStr, yStr] = pair.split(","); 
        let x = parseFloat(xStr);
        let y = parseFloat(yStr);
  
        if (!isNaN(x) && !isNaN(y)) {
            rita(2*x, 2*y);
        }
    }
}

socket.onmessage = function (message) {
    console.log(message);
    processAndDraw(message.data);
}