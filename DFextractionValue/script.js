const tableBody = document.getElementById("items-table-body");
let items = [];
let currentSort = { key: "name", ascending: true };

// Sort items by key (name, value, or valuePerVolume), toggle ascending/descending
function sortItems(key) {
    if (currentSort.key === key) {
        currentSort.ascending = !currentSort.ascending; // toggle order
    } else {
        currentSort.key = key;
        currentSort.ascending = true;
    }

    items.sort((a, b) => {
        let aVal, bVal;

        if (key === "valuePerVolume") {
            aVal = a.value / a.volume;
            bVal = b.value / b.volume;
        } else {
            aVal = a[key];
            bVal = b[key];
        }

        if (typeof aVal === "string") {
            aVal = aVal.toLowerCase();
            bVal = bVal.toLowerCase();
        }

        if (aVal < bVal) return currentSort.ascending ? -1 : 1;
        if (aVal > bVal) return currentSort.ascending ? 1 : -1;
        return 0;
    });

    renderItems();
}

function renderItems() {
    tableBody.innerHTML = "";
    items.forEach(({ name, value, volume, image }) => {
        const valueinThousands = value / 1000;
        const valuePerVolume = valueinThousands / volume;
        const row = document.createElement("tr");
        row.innerHTML = `
      <td><img src="${image}" alt="${name}"></td>
      <td>${name}</td>
      <td>$${valueinThousands.toFixed(1)}</td>
      <td>$${valuePerVolume.toFixed(1)}</td>
    `;
        tableBody.appendChild(row);
    });
}

// Fetch items.json and render the table
// Fetch items.json and render the table
fetch('items.json')
    .then(response => {
        if (!response.ok) {
            throw new Error(`HTTP error! status: ${response.status}`);
        }
        return response.json();
    })
    .then(data => {
        items = data;           // Assign fetched data to global 'items'
        sortItems("name");      // Sort by name and render on load
    })
    .catch(error => {
        console.error('Failed to load items:', error);
        tableBody.innerHTML = '<tr><td colspan="4">Failed to load items.</td></tr>';
    });



document.getElementById("sort-name").addEventListener("click", () => sortItems("name"));
document.getElementById("sort-value").addEventListener("click", () => sortItems("value"));
document.getElementById("sort-valuePerVolume").addEventListener("click", () => sortItems("valuePerVolume"));
