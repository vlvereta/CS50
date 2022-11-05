const blinkedElements = document.querySelectorAll(".blinked");

window.setInterval(() => {
    blinkedElements.forEach(elem => {
        const currentVisibility = elem.style.visibility;
        elem.style.visibility = currentVisibility === "hidden" ? "" : "hidden";
    });
}, 500);
