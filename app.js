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
    "SKOG nr 1": "url('assets/helluheightInfoBILD.png')"
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
const start = [0, 0];
const slut = [0, 0];


klickArea.addEventListener('click', function(event) {
    const rect = klickArea.getBoundingClientRect();
    const x = event.clientX - rect.left;
    const y = event.clientY - rect.top;

    console.log(`Klick på x: ${x}, y: ${y}`);

    // Skapa en liten markör där man klickar
    const marker = document.createElement('div');
    marker.className = 'marker';
    marker.style.left = x + 'px';
    marker.style.top = y + 'px';
    if (klickArea.childElementCount <=1) {
        klickArea.appendChild(marker);
        start[0] = x;
        start[1] = y;
    } else if (klickArea.childElementCount <=2) {
        klickArea.appendChild(marker);
        slut[0] = x;
        slut[1] = y;
    }
    console.log(start[0], start[1], slut[0], slut[1]);
});

function createRoute() {
    // Skicka koordiinater till Algoritm
    console.log(`KLICKAT PÅ CREATE ROTE KNAPP ${start},${slut}`);
    let message = Math.round(start[0])+","+Math.round(start[1])+ ","+ Math.round(slut[0]) + "," + Math.round(slut[1]);
    socket.send(message);
}

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
    let points = input.split(";"); 
  
    while (points.length > 0) {
        let pair = points.shift(); 
        if (!pair) continue;
  
        let [xStr, yStr] = pair.split(","); 
        let x = parseFloat(xStr);
        let y = parseFloat(yStr);
  
        if (!isNaN(x) && !isNaN(y)) {
            rita(x, y);
        }
    }
  }
  

socket.onmessage = function (message) {
    console.log(message);
    processAndDraw(message.data);
}