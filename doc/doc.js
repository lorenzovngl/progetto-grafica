let ctx = document.getElementById("lifecycle").getContext("2d");
let c = {x: 0, y: 100};      // cursor
ctx.font = "14px Arial";
const headlen = 10; // length of head in pixels
const state_radius = 20;

ctx.beginPath();
canvas_arrow(ctx, c.x, c.y + 80, c.x + 115, c.y + 80);
ctx.stroke();
ctx.fillText("Lancio", c.x + 30, c.y + 50);
ctx.fillText("dell'applicazione", c.x, c.y + 70);
ctx.beginPath();
ctx.arc(c.x + 130, c.y + 80, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 145, c.y + 180, 100, Math.PI, 1.4 * Math.PI);
arrowhead(ctx, headlen, 1.95*Math.PI, c.x + 131, c.y + 85);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 30, c.y + 95, 100, 2* Math.PI, 0.4 * Math.PI);
ctx.stroke();
ctx.fillText("Scelta del", c.x, c.y + 230);
ctx.fillText("numero di boe", c.x, c.y + 250);
ctx.beginPath();
ctx.arc(c.x + 45, c.y + 195, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
canvas_arrow(ctx, c.x + 145, c.y + 80, c.x + 255, c.y + 80);
ctx.stroke();
ctx.fillText("Inizio della", c.x + 150, c.y + 50);
ctx.fillText("sessione di gioco", c.x + 140, c.y + 70);
ctx.beginPath();
ctx.arc(c.x + 270, c.y + 80, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 340, c.y + 150, 100, 0.8 * Math.PI, 1.2 * Math.PI);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 200, c.y + 150, 100, 1.8 * Math.PI, 0.2 * Math.PI);
ctx.stroke();
ctx.fillText("Gioco in", c.x + 180, c.y + 240);
ctx.fillText("pausa", c.x + 190, c.y + 260);
ctx.beginPath();
ctx.arc(c.x + 270, c.y + 220, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 310, c.y + 50, 180, 0.6 * Math.PI, 0.92 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 340, c.y + 290, 100, 0.8 * Math.PI, 1.2 * Math.PI);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 200, c.y + 290, 100, 1.8 * Math.PI, 0.2 * Math.PI);
ctx.stroke();
ctx.fillText("Visualizzazione", c.x + 130, c.y + 350);
ctx.fillText("dei comandi", c.x + 140, c.y + 370);
ctx.beginPath();
ctx.arc(c.x + 270, c.y + 360, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 340, c.y + 290, 100, 1.3 * Math.PI, 1.7 * Math.PI);
ctx.stroke();
ctx.beginPath();
ctx.arc(c.x + 340, c.y + 150, 100, 0.3 * Math.PI, 0.7 * Math.PI);
ctx.stroke();
ctx.fillText("Visualizzazione", c.x + 405, c.y + 180);
ctx.fillText("dello leaderboard", c.x + 400, c.y + 200);
ctx.beginPath();
ctx.arc(c.x + 410, c.y + 220, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
canvas_arrow(ctx, c.x + 285, c.y + 80, c.x + 395, c.y + 80);
ctx.stroke();
ctx.fillText("Fine della", c.x + 300, c.y + 50);
ctx.fillText("sessione di gioco", c.x + 280, c.y + 70);
ctx.beginPath();
ctx.arc(c.x + 410, c.y + 80, state_radius, 0, 2 * Math.PI);
ctx.stroke();

ctx.beginPath();
ctx.arc(c.x + 270, c.y + 80, 140, 1.03 * Math.PI, 1.97 * Math.PI);
arrowhead(ctx, headlen, Math.PI/2, c.x + 131, c.y + 69);
ctx.stroke();

ctx.beginPath();
canvas_arrow(ctx, c.x + 425, c.y + 80, c.x + 540, c.y + 80);
ctx.stroke();
ctx.fillText("Chiusura", c.x + 450, c.y + 50);
ctx.fillText("dell'applicazione", c.x + 430, c.y + 70);

// From https://stackoverflow.com/questions/808826/draw-arrow-on-canvas-tag
function canvas_arrow(context, fromx, fromy, tox, toy) {
    const dx = tox - fromx;
    const dy = toy - fromy;
    const angle = Math.atan2(dy, dx);
    context.moveTo(fromx, fromy);
    context.lineTo(tox, toy);
    arrowhead(context, headlen, angle, tox, toy);
}

function arrowhead(context, headlen, angle, tox, toy) {
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
    context.moveTo(tox, toy);
    context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
}