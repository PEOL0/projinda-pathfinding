console.log("JS scriptad");

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
    klickArea.appendChild(marker);
    // GÖR ATT MAN BARA KAN SKAPA 2 PUNKTER
});


